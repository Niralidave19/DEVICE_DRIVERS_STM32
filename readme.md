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
- Input: To read signals Eg: Button peripheral would be an input
- Output: To send signals Eg: LED blinking peripheral would be an output
In STM32 F446RE, 
- 8 GPIO Ports 
- Each Port has 16 I/O pins. In total, 128 I/O pins are available. 

### Dive into the Architecture and how GPIOS are activated 
AHB1 Bus of the controller is connected to GPIO peripherals. Address range for which AHB1 bus is connected: 0x4002000 - 0x4007FFFF
All the GPIO ports are connected to the AHB1 Bus of the controller. The clock to the GPIO port is activated by altering the specific bit of the RCC register. 
## GPIO Port Base Addresses (STM32F4 Series)
| GPIO Port | Base Address | Bus   | RCC |  
|-----------|--------------|-------|-----|    ----> Each port has 10 registers : 6 Control registers, 2 data registers , 1 I/O bit handling atomic R/W register 
| GPIOA     | `0x40020000` | AHB1  |     |  
| GPIOB     | `0x40020400` | AHB1  |     |       
| GPIOC     | `0x40020800` | AHB1  |     |       
| GPIOD     | `0x40020C00` | AHB1  |     |           
| GPIOE     | `0x40021000` | AHB1  |     |       
| GPIOF     | `0x40021400` | AHB1  |     |      
| GPIOG     | `0x40021800` | AHB1  |     |       
| GPIOH     | `0x40021C00` | AHB1  |     |        

| Control Register|            
|-----------------|                     
| GPIOx_MODER     |                 
| GPIOx_OTYPER    |       
| GPIOx_OSPEEDR   |   
| GPIOx_PUPDR     |
| GPIOx_AFRL      |
| GPIOx_AFRH      |
| GPIOx_LCKR      |

|  Data Register  | 
|-----------------|
|   GPIOx_IDR     |
|   GPIOx_ODR     |

| I/O Atomic R/W  |
|-----------------|
|   GPIOx_BSRR    |
