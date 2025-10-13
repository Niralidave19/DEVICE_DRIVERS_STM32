# STM32 Device Drivers – Bare-Metal HAL Layer  

This project implements device drivers and builds a custom HAL Layer for STM32F446RE microcontroller, as part of learning **bare-metal programming**. 
Currently this repo holds Device Driver implementation for GPIO and USART.

---
## MAIN MOTIVE   
- To understand the interaction between the **Hardware peripherals** of microcontrollers and the **software implementation**.
- To understand the hardware dependency, and link the Embedded software to the actual CPU architecture
- To not use any **STM32 HAL Libraries** and Build the entire HAL layer from scratch.
- To strengthen **register-level programming** and **low-level driver development** skills  
- To create **modular, reusable drivers** (GPIO, USART, SPI, I²C, Timers, etc.) as a learning base for future embedded projects
---
## GPIO Device driver
General Purpose Input Output : GPIO are pins in a microcontroller which can either be configured as an input of an output pin. 
- Input: To read signals Eg: Button peripheral would be an input.
- Output: To send signals Eg: LED blinking peripheral would be an output.
In STM32 F446RE, 
- 8 GPIO Ports 
- Each Port has 16 I/O pins. In total, 128 I/O pins are available. 

### Dive into the Architecture and how GPIOS are activated 
#### Where do these GPIO peripherals sit in F446RE??
AHB1 Bus of the controller is connected to GPIO peripherals. Address range for which AHB1 bus is connected: 0x4002000 - 0x4007FFFF
All the GPIO ports are connected to the AHB1 Bus of the controller and the clock to the GPIO port is activated by altering the specific bit of the RCC register.
#### Activation of these peripherals
RCC_AHB1ENR Register:
If Pin 1 of GPIO Port is connected to an I/O interface, then in the RCC register GPIOAEN bit should be set to 1. 
Note: Even though only Pin 1 of GPIOA is connected to an I/O interface , we would require to enable the clock for GPIOA. (Clock is enabled for the Port and not for I/O pin)

<img width="952" height="145" alt="image" src="https://github.com/user-attachments/assets/58c1e939-5ed2-4a15-8eef-e4b17be59116" />

### GPIO Port Base Addresses (STM32F4 Series) 
| GPIO Port | Base Address | Bus   |   
|-----------|--------------|-------|   
| GPIOA     | `0x40020000` | AHB1  |  
| GPIOB     | `0x40020400` | AHB1  |       
| GPIOC     | `0x40020800` | AHB1  |      
| GPIOD     | `0x40020C00` | AHB1  |           
| GPIOE     | `0x40021000` | AHB1  |       
| GPIOF     | `0x40021400` | AHB1  |     
| GPIOG     | `0x40021800` | AHB1  |       
| GPIOH     | `0x40021C00` | AHB1  |           

Each port has 10 registers : 8 Control registers, 2 data registers.
### Configuring a GPIO Pin
While configuring a given pin as a GPIO pin, the software needs to initialise the control registers accordingly. This will enable the GPIO pin as an Input configuration mode, Output configuration mode, Analog Mode or Alternate functionality mode based on developers choice and activate the clock to the specific GPIO Port as well. 

#### GPIO Control Registers
| Control Register        | Function                                | Description                                                                                                                                                                                                 |
|--------------------------|-------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `GPIOx_MODER`           | Mode Register                            | Configures each pin as **Input**, **Output**, **Alternate Function**, or **Analog**. Each pin uses 2 bits. (`00` = Input, `01` = Output, `10` = Alternate, `11` = Analog)                                    |
| `GPIOx_OTYPER`          | Output Type Register                      | Selects the output driver type when the pin is configured as output. (`0` = Push-pull, `1` = Open-drain)                                                                                                     |
| `GPIOx_OSPEEDR`         | Output Speed Register                     | Controls the output speed for each pin. Options: **Low**, **Medium**, **High**, and **Very High** speed — affects rise/fall time and power consumption.                                                      |
| `GPIOx_PUPDR`           | Pull-up/Pull-down Register                | Configures internal **pull-up** or **pull-down** resistors for each pin. (`00` = No pull, `01` = Pull-up, `10` = Pull-down)                                                                                   |
| `GPIOx_AFRL` / `AFRH`   | Alternate Function Registers (Low/High)   | Selects the **alternate function mapping** for each pin when configured in Alternate Function mode. `AFRL` for pins 0–7 and `AFRH` for pins 8–15.                                                            |
| `GPIOx_BSRR`           | Bit Set/Reset Register                    | Allows **atomic set or reset** of output pins. Writing `1` to bits 0–15 sets the pin; writing `1` to bits 16–31 resets the pin. Prevents race conditions.                                                     |
| `GPIOx_LCKR`           | Configuration Lock Register               | Locks the configuration of the GPIO pins until the next reset. Prevents accidental changes to critical pins after setup.                                                                                    |

GPIOx_MODER Register:
If the mode for Pin 1 GPIOA register, has to be set as Output Mode : The first two bits of GPIOA_MODER register has to be set to 01.
<img width="982" height="218" alt="image" src="https://github.com/user-attachments/assets/7d3ecdf2-1cab-4aa7-9b7c-0eaac8340ee1" />

#### Data Registers
|  Data Register  |      Function       |                Description                                                 |
|-----------------|---------------------|----------------------------------------------------------------------------|
|   GPIOx_IDR     | Input Data Register |Captures the logic level present on the GPIO pins configured as inputs.     |
|                 |                     |The CPU reads this register to know whether the pin is high (1) or low (0). |
|                 |                     |                                                                            |
|   GPIOx_ODR     | Output Data Register|Holds the logic value to be driven on GPIO pins configured as outputs.      | 
|                 |                     |Writing 1 or 0 here sets the output level on the corresponding pin.         |

##### Is this an Atomic Operation??
No, reading from the GPIOx_IDR and writing into GPIOx_ODR is an non-atomic operation.
This means the CPU performs two separate steps:
  - **Read** the current state of the input register.
  - **Write** the result into the output register.
  - If an **interrupt** or **another peripheral** modifies the register between these two steps, the output might not reflect the intended state — leading to race conditions.

**STM32 provides a Bit Set/Reset Register (BSSR) to perform atomic operation**
  - Writing into this register directly sets/resets the value in GPIOx_ODR
  - It doesn't require writing into ODR, hence race condition is avoided.

#### To Initialise a GPIO Port:
1. Enable the clock to the port by altering the RCC_AHB1ENR register
2. Set the mode of the GPIO Pin : In the MODER Register for the specific pin
3. Set the pin to Pull-up / Pull-down type - Required only if the pin is configured in Input Mode
4. Set the output speed of the Pin - Not required if pin configured in INPUT Mode.
5. Set the output type of the I/O port - Output Push Pull / Output open drain - Not required if pin is configured in INPUT Mode.
---
## UART/USART DRIVERS
- UART/USART stands for universal synchronous synchronous/asynchronous receiver transmitter. This is a hardware module that facilitates serial communication.
- The controller needs to have two pins: TX and RX pin on which it can either transmit data (TX Pin) or receive data (RX Pin).
### UART (Asynchronous communication) 
No shared clock between the client and server, then **how do they talk?** They agree upon a pre-defined baud rate, and synchronization is achieved using start and stop bits. 

