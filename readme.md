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
#### Syncronization between Client and Server 
- Pre-defined baud rate is used by both the client and the server.
- The receiver continuously monitors and detects the fallind edge (from high to low)  - which implies a start of a new frame. NOTE": In UART, the line is idle high (logic 1) when no data is being transmitted. Hence, when there is a transition from a high to low, that is the start of the frame. This falling edge synchronizes the receivers internal clock to begin sampling.
- After the start bit, the receiver samples each subsequent bit at exactly one bit time intervals. It samples in the middle of each bit because this would reduce chance of reading incorrect value due to noise or signal distortion.
For example, let's say the baud rate = 9600 bits/sec  
Each bit is sampled at 104 microseconds.  
The sampling points would be at the middle of each bit:
Start Bit : 0.5 × 104µs = 52µs  
Bit 0     : 1.5 × 104µs = 156µs  
Bit 1     : 2.5 × 104µs = 260µs  
Bit 2     : 3.5 × 104µs = 364µs  
Bit 3     : 4.5 × 104µs = 468µs  
Bit 4     : 5.5 × 104µs = 572µs  
Bit 5     : 6.5 × 104µs = 676µs  
Bit 6     : 7.5 × 104µs = 780µs  
Bit 7     : 8.5 × 104µs = 884µs  
Stop Bit  : 10.5 × 104µs = 1092µs
#### Why have a separate baud rate? Why can't UART comminicate syncing with the clock source?
1. UART peripheral receives a clock from the system clock source. This is much faster than the baud rate.
2. In the software, we configure the BRR value which would divide the UART clock down to match the desired baud rate.
3. The UART hardware uses this BRR value to generate internal timing pulses.
#### Initialization of the UART hardware (Asynchronous) 
1. Consider two GPIO Pins in Alternate functionality mode, such that it behaves as a UART Rx and UART Tx Pin. Initialise the GPIO pins as described in section 1.
2. Enable the clock for UART hardware - The UART clock is the system or peripheral clock that powers the UART hardware block inside the microcontroller. A clock is used to generate the baud rate, transmit/receive shift registers etc.
   - The UART peripheral is connected to the APB1 bus. Hence enable the clock to the UART2 peripheral in the RCC registers.
3. Set the baud rate for UART communication - Set it in register USART_BRR.
   - Baud rate = UART Clock / USART_BRR.
   - Consider the Baud rate to be 9600 bits/sec. Assuming the UART clock is 16MHz, the valu eof the USART_BRR must be set to 1667.
4. In USART CR1 register, set the USART Enable , Transmitter enable pin and Receiver Enable Pins to 1.
#### How is the transmission acheived? 
During a USART transmission, data shifts out least significant bit first on the TX pin. In this mode, the USART_DR register consists of a buffer (TDR) between the internal bus amd the transmit shift register.
- When we transmit data, the data register (USART_DR) is empty — i.e., the previous byte has moved to the shift register and the USART is ready for new data - Then TXE bit in the status register is set to 1 and an interrupt is triggered to transmit this data if TXEIE = 1.
  
## INTERRUPTS AND EVENTS 
- Interrupts are signals, that temporarily halts the main execution of a program to allow the CPU to respond to an external or internal event. At the reset state, all interrupts are disabled. The processor begins executing the code instructions with a base execution priority lower than the lowest programmable priority level, so any enabled interrupt can pre-empt the processor.
- Interrupts can occur in two ways: Internal Interrupts and External Interrupts. At the end, everything is handled by NVIC.
- In case of Internal Interrupts, they originate from inside the MCU, from on-chip peripherals which are directly connected to NVIC.
- In case of External Interrupts, the external interrupt controller (EXTI) monitors external signals and generates interrupts or events. There are 23 edge detectors connected to EXTI - These circuits detects change in signal levels which is useful when an external event/interupt occurs.When an edge transition is detected on an external pin, the controller can generate an Interupt / Event. Each line is configured to trigger an event: Rising edge, Falling edge, Both edges. The lines/interrupts can be masked - meaning you can disable or ignore specific interrupt lines temporarily.
<img width="968" height="648" alt="image" src="https://github.com/user-attachments/assets/fa35bf1f-6be5-4af3-b37d-a076ed70ed29" />

### NVIC - Nested Vectored Interrupt Controller 
- Manages all interrupts in the system. 
- This controller handles, enabling and disabling the interrupts, Setting priorities for the interrupts.(Priority and sub-priority)
- Interrupt stacking, entry, and exit are done automatically in NVIC. Which offloads this work overhead from the CPU.
Without NVIC, managing multiple interrupts would be a chaotic task. NVIC ensures that higher-priority tasks can preempt lower-priority ones, maintaining system responsiveness.

### Where are the NVIC peripherals? Where do they sit?
- In the below image , the highlighted RED coloured box indicates the Base address of the Cortex-M4 internal memory mapped registers.(0xE000000 - 0xE00FFFFF) A part of these represent the NVIC peripherals as well. 
- This address range is inside a private peripheral bus, unlike the GPIOs USART etc. 
<img width="1392" height="858" alt="image" src="https://github.com/user-attachments/assets/85c3b384-2ddc-41ed-94df-5e019e496333" />

#### NVIC Register layout is such that:
There are around 82 interrupts in cortex M4, and each interrupt is enabled/disabled/given a priority through NVIC. Each NVIC Register is 32 bits wide - where each bit would correspond to an interrupt. Looking at this structure, each register will handle 32 interrupts.
The User/APP, decides the priority grouping and priority of an interrupt. This is configured by the NVIC, and written into registers.
- ISER (Interrupt Set Enable Register): Writing one to the bit, would enable the specific IRQ. 
      1. ISER0 - Handles interrupts from IRQ0 - IRQ31 
      2. ISER1 - Handles interrupts from IRQQ32 - IRQ63
      3. ISER3 - Handles interrupts from IRQ64 - IRQ81
- ICER (Interrupt Clear Enable Interrupt): Writing one to the bit, would disable the specific IRQ.
- ISPR (Interrupt Set Pending Register): Writing one to the bit, sets the specific IRQ as pending.
- ICPR (Interrupt Clear Pending Register): Writing one to the bit, clears the pending interrupt for the specific IRQ.
- IABR (Interrupt Active Bit Register): This is used to check if a given interrupt is currently being executed. If the specific interrupt bit in IABR is set to 1 implies the ISR is executed for that interrupt.
- IPR  (Interrupt Priority Register): Note that this register is byte addressable - Each IRQ has an IPR register, which indicates the priority of the interrupt.

### What is a Priority? How to we set a priority group? 
NVIC allows setting both preemption priority and subpriority. Here’s how they differ:
- Preemption Priority: Determines whether one interrupt can interrupt another.
- Subpriority: Used to resolve conflicts when two interrupts of the same preemption level occur simultaneously.
- Priority of an interrupt is written into the corresponding IPR Register.The priority grouping, is decided by the APP and written into the AICR register.
#### What does this priority number indicate? 
- The 4 bits of priority value, has two aspects to it:
  1. Preemption Priority: Decides which interrupt can preempt another one
  2. Subpriority : Decides which interrupt should run first if two are at the same pre-emption level and are pending.
- The concept of **Priority Grouping** controls how many bits of the 4-bit priority are used for preemption and how many for subpriority.
- There are different priority groups:
  1. 0b000 - All 4 bits for preemption priority
  2. 0b001 - 1 bits preemption and 3 bit subpriority
  3. 0b010 - 2 bits preemption and 2 bits subpriority
  4. 0b011 - 3 bits preemption and 1 bit subpriority
  5. 0b100 - 4 bits preemption and 0 bit subpriority
#### AIRCR (Application Control and Reset Control Register)
- This register sets the priority grouping used by the application, inside the AIRCR Register. This determines how the NVIC interprets the priority byte you write for each IRQ.
- It is a 32 bit register, where
  1. Bits 31:16 represent the VECT_KEY
  2. Bits 10:8  Priority Group
- To set the priority grouping, a write_key needs to be written into the VECT_KEY bits - to unlock writes. The VECT_KEY that needs to be written for unlocking is 0x5FA.
#### How to set the priority grouping?
- First clear the bits in the AIRCR register - VECT_KEY and Priority Group
- Write 0X5FA into bits 31:16 (Bits corresponding to VECT_KEY) - This will unlock the writing into the register
- Set the priority as per the user needs.

On STM32, Each interrupt priority value is 4 bits long - Priority Value can vary from 0 - 15 (16 Priority values). When the priority is written into the IPR register of NVIC, only the upper four bits is written
So how to write in this priority for interrupts?
- NVIC IPR register base address is 0xE000E400
- Each interrupt gets one NVIC IPR register (1 Byte long), where only the top 4 bits are used to indicate the priority.
- For USART2_IRQn = n, Priority register address = 0xE000E400 + n
  *(volatile uint8_t *)(0xE000E400 + 38) = (5 << 4);   // set priority level 5 for USART2




















### EXTI - External Interrupt/Event Controller - USED ONLY FOR EXTERNAL INTERRUPTS NOT USED FOR INTERNAL INTERRUPTS
- Monitors external pins (like GPIOs) for signal changes.
- When a edge is detected, EXTI sets a pending interrupt flag.
- EXTI doesn't directly interrupt the CPU - It signals that an interrupt is pending.
### External Interrupt/Event Controller Block Diagram
<img width="566" height="422" alt="image" src="https://github.com/user-attachments/assets/7801f8bb-1aa4-420b-81d2-5735de2e9dd1" />

#### 23 Bit wide registers
1. Pending Request Register: Stores which interrupt lines have been triggered and waiting to be serviced.
2. Interrupt Mask Register: Controls which interrupt lines are enabled or disabled.
3. Software Interrupt Event Register: Allows software to manually trigger an interrupt.
4. Rising Trigger Selection Register: Configures which lines should respond to rising edges.
5. Falling Trigger Selection Register: Configures which lines should respond to falling edges.
#### Hardware circuits as part of the EXTI controller
1. **Edge Detect Circuit** : Monitors input signal for rising, falling or both edges.
2. **Pulse Generator & Event mask register** : Pulse is generated when an even occurs and the Event Mask Register decides if the events are allowed to proporgate.
3. **Logic gates & NVIC** : Combines signals from the edge detector and pulse generator. Sends valid requests to NVIC - which handles by calling appropriate ISRs.
   
### System Configuration controller 
- We use the register system configuration external interrupt configuration register for glowing an LED in an interrupt mode.
- It allows you to map external interrupts (EXTI0 - EXTI3) to specific GPIO ports.
- EXTI0 can come from PA0 or PB0 or PC0 etc.. In one application , we can tie it to only one pin.
- Each EXTIx is tied to which port - Is defined in the SYSCFG_EXTICR1 register as part of the SYSCFG block.
Say we want to configure EXTI0 for pin PA0: 
                               [PA0 pin] → [SYSCFG EXTICR: map PA0 → EXTI0] → [EXTI controller: enable line, set edge detection] → [NVIC EXTI0_IRQn] → CPU ISR
                               

The EXTI registers are part of this 
<img width="704" height="55" alt="image" src="https://github.com/user-attachments/assets/51c35c42-3e05-4909-a3ae-f75a7aa73c01" />

