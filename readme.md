# STM32 Device Drivers – Bare-Metal HAL Layer  

This project implements device drivers and builds a custom HAL Layer for STM32F446RE microcontrollers, as part of learning **bare-metal programming**. 
As part of the learning process, currently this repo holds Device Driver implementation for GPIO and USART.

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

### Clone the Repository  

```bash
git clone https://github.com/Niralidave19/DEVICE_DRIVERS_STM32.git
cd DEVICE_DRIVERS_STM32
