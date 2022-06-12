# STM32-StarterPack


I just started studying embedded coding on STM32 via STM32F407VG, many mistakes may be caused and i want to record all my mistakes as a cheatsheet for later projects. This Repository contains all my mistakes in all peripherals initializaion and usage, my notes for solution and my guide for Setting on STM32CubeMX, and (maybe) knowledges before starting.

### 1.[UART Transmit/Receive DMA](UART.md)

	Comments and notes including 3 types of errors: How to initialize UART on CubeMX, UART Receive IDLE DMA, UART Error Handler 

### 2.[ADC DMA](ADC.md)

	Comments and notes on how to basically use ADC: How continuous read ADC? How to read multiple ADCs? Code to Start ADC?...

### 3.[I2C DMA](I2C.md)

	Comments and notes on how to basically use I2C Interface: How to Initialize I2C interface? How I2C works with other devices?...

### 4. Custom Library: 
*Example library is in project **24C04interface***

My standard pattern to make a library: [Notion](https://fortunate-smash-efc.notion.site/Embedded-Programing-1ce6e9d260744f34bc9a1285a08cb28a)
Refer to this [HeadfirstC](http://karadev.net/uroci/filespdf/files/head-first-c-o-reilly-david-grifffiths-dawn-griffiths.pdf) documentation - Chapter 4 for better understanding (how C works with splitted files and how the compiler works)

**b. Dynamic Memory Allocation 

To understand Dynamic Memory Alloction, you should take a quick look to [Memory Architecture in C](https://www.facebook.com/truonggiang.deviot.vn/videos/1305159089970607)

Dynamic Memory Allocation focuses in Heap region in Memory. It differs from cases when you declare a variable in c file, these variables are automatically allocated in Stack field.

- [Electrical components note](Electricalcomponents.md)
	
	Some of my revision to about basic knowledges to understand more about electronic components
