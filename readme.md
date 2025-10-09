# STM32 Device Drivers – Bare-Metal HAL Layer  

This project implements device drivers and builds a custom HAL Layer for STM32F446RE microcontroller, as part of learning **bare-metal programming**. 
Currently this repo holds Device Driver implementation for GPIO and USART.

---
## Main Motive   
- To understand the interaction between the **Hardware peripherals** of microcontrollers and the **software implementation**.
- To understand the hardware dependency, and link the Embedded software to the actual CPU architecture
- To not use any **STM32 HAL Libraries** and Build the entire HAL layer from scratch.
- To strengthen **register-level programming** and **low-level driver development** skills  
- To create **modular, reusable drivers** (GPIO, USART, SPI, I²C, Timers, etc.) as a learning base for future embedded projects  
---

## Getting Started  

### Prerequisites  

- STM32F446RE Nucleo board (or compatible STM32 Cortex-M4)  
- STM32CubeIDE installed  
- Datasheet 

### GPIO Device driver

General Purpose Input Output : GPIO are pins in a microcontroller which can either be configured as an input of an output pin. 
- Input: To read signals Eg: Button peripheral would be an input.
- Output: To send signals Eg: LED blinking peripheral would be an output.
In STM32 F446RE, 
- 8 GPIO Ports 
- Each Port has 16 I/O pins. In total, 128 I/O pins are available. 

### Dive into the Architecture and how GPIOS are activated 
AHB1 Bus of the controller is connected to GPIO peripherals. Address range for which AHB1 bus is connected: 0x4002000 - 0x4007FFFF
All the GPIO ports are connected to the AHB1 Bus of the controller and the clock to the GPIO port is activated by altering the specific bit of the RCC register.

## GPIO Port Base Addresses (STM32F4 Series) 
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

Each port has 10 registers : 7 Control registers, 2 data registers , 1 I/O bit handling atomic R/W register.

## Control Registers
- GPIOx_MODER: Register hold the information about the I/O configuration of the pin. 
//add the image of the MODER Register , and highlight the Bit position to be altered if we are configuring I/O for pin number 1.
- GPIOx_OTYPER : To select an output configuration - Push Pull or Open Drain config. This is used when we configure the pin as Output.
- GPIOx_SPEEDR : To configure the I/O speed.
- GPIOx_PUPDR  : To configure I/O to pull up or pull down configuration. This is used when we configure the pin as Input.
- GPIOx_AFLR   : To configure the I/O pin with an alternate functionailty. The Alternate functionality low register, will configure the functionalities for the first 8 pins of the GPIO port.
- GPIOx_AFHR   : To configure the I/O pin with an alternate functionailty. The Alternate functionality high register, will configure the functionalities for pins 8 - 16 pins of the GPIO port.
## GPIO Control Registers

| Control Register        | Function                                | Description                                                                                                                                                                                                 |
|--------------------------|-------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `GPIOx_MODER`           | Mode Register                            | Configures each pin as **Input**, **Output**, **Alternate Function**, or **Analog**. Each pin uses 2 bits. (`00` = Input, `01` = Output, `10` = Alternate, `11` = Analog)                                    |
| `GPIOx_OTYPER`          | Output Type Register                      | Selects the output driver type when the pin is configured as output. (`0` = Push-pull, `1` = Open-drain)                                                                                                     |
| `GPIOx_OSPEEDR`         | Output Speed Register                     | Controls the output speed for each pin. Options: **Low**, **Medium**, **High**, and **Very High** speed — affects rise/fall time and power consumption.                                                      |
| `GPIOx_PUPDR`           | Pull-up/Pull-down Register                | Configures internal **pull-up** or **pull-down** resistors for each pin. (`00` = No pull, `01` = Pull-up, `10` = Pull-down)                                                                                   |
| `GPIOx_AFRL` / `AFRH`   | Alternate Function Registers (Low/High)   | Selects the **alternate function mapping** for each pin when configured in Alternate Function mode. `AFRL` for pins 0–7 and `AFRH` for pins 8–15.                                                            |
| `GPIOx_BSRR`           | Bit Set/Reset Register                    | Allows **atomic set or reset** of output pins. Writing `1` to bits 0–15 sets the pin; writing `1` to bits 16–31 resets the pin. Prevents race conditions.                                                     |
| `GPIOx_LCKR`           | Configuration Lock Register               | Locks the configuration of the GPIO pins until the next reset. Prevents accidental changes to critical pins after setup.                                                                                    |

//add image of the AFLR and AFHR   
## Data Registers
- GPIOx_IDR : Input Data Register - Captures the logic level present on the GPIO pins configured as inputs. The CPU reads this register to know whether the pin is high (1) or low (0). Example: A button is set as an input to one of the GPIO pins, whose IDR reset value is 0. When the button is pressed, IDR captures the value to be 1. 
- GPIOx_ODR : Output Data Register - Holds the logic value to be driven on GPIO pins configured as outputs. Writing 1 or 0 here sets the output level on the corresponding pin

|  Data Register  |      Function       |                Description                                                 |
|-----------------|---------------------|----------------------------------------------------------------------------|
|   GPIOx_IDR     | Input Data Register |Captures the logic level present on the GPIO pins configured as inputs.     |
|                 |                     |The CPU reads this register to know whether the pin is high (1) or low (0). |
|                 |                     |                                                                            |
|   GPIOx_ODR     | Output Data Register|Holds the logic value to be driven on GPIO pins configured as outputs.      | 
|                 |                     |Writing 1 or 0 here sets the output level on the corresponding pin.         |
