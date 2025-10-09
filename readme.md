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

## 🛠 Getting Started  

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
GPIOx_MODER: Register hold the information about the I/O configuration of the pin. 
//add the image of the MODER Register , and highlight the Bit position to be altered if we are configuring I/O for pin number 1.
GPIOx_OTYPER : To select an output configuration - Push Pull or Open Drain config.
GPIOx_SPEEDR : To configure the I/O speed.
GPIOx_PUPDR  : To configure I/O to pull up or pull down configuration.
| Control Register| 
|-----------------|                    
| GPIOx_MODER     |       
| GPIOx_OTYPER    |  
| GPIOx_OSPEEDR   |   
| GPIOx_PUPDR     |
| GPIOx_AFRL      |
| GPIOx_AFRH      |
| GPIOx_LCKR      |

## Data Registers
GPIOx_IDR : Input Data Register - Read only registers, which is read by the processor to get an input from an external source. 
Example: A button is set as an input to one of the GPIO pins, whose IDR reset value is 0. When the button is pressed, IDR captures the value to be 1. The processor reads this register value for further processing,
GPIOx_ODR : Output Data Register - The 
|  Data Register  | 
|-----------------|
|   GPIOx_IDR     |
|   GPIOx_ODR     |

## I/O Atomic R/W
| I/O Atomic R/W  |
|-----------------|
|   GPIOx_BSRR    |
