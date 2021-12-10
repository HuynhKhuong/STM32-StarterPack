STM32-StarterPack
I just started studying embedded coding on STM32 via STM32F407VG, many mistakes may be caused and i want to record all my mistakes as a cheatsheet for later projects. This Repository contains all my mistakes in all peripherals initializaion and usage, my notes for solution and my guide for Setting on STM32CubeMX, and (maybe) knowledges before starting.

1. UART Transmit/Receive DMA
  a. Notes before initializing on CubeMX
     In subsequent calls to HAL_UART_Transmit_DMA, the functions immediately returns
     HAL_BUSY because huart->gState != HAL_UART_STATE_READY
     This is the case because UART_DMATransmitCplt relies on the USART interrupt to be enabled to reset huart->gState to HAL_UART_STATE_READY. If you configure UART DMA in Normal mode without interrupts, HAL_UART_Transmit_DMA only transmits for the first time and no more transmittion after. => Must enable USART global interrupt.
     REFERENCE: https://community.st.com/s/question/0D50X00009XkePT/hal-without-usart-interrupt-haluarttransmitdma-will-work-only-once.
  b. Becareful about configuring clock tree
     I've configured clock tree for HSE but entire UART worked incorrectly.
  c. Example
     Above is my project for read DMA UART Transmit and Receive. This project uses UART4, Transmit using Normal mode and Receive using Circular mode. In the future i will study more about IDLE Mode for more complex data transmit.
  
  d. UART4 Receive IDLE DMA, STM32 will automatically detect if the Receive line is free or not after a serie of incoming data. Then it will jump into an interrupt handler, which gives you some informations: data received, data length
     To do that, you have to configure your UART Module into Normal DMA mode. After interrupt, you have to initialize it to continue receive. 
     ONE MORE THING, At the beginning, DMA will automatically initialize Half Received Interrupt and Full Received Interrupt, which is quite useful for continuously receiving application and fixed data size application. In my case, 
     I want UART to receive data with unknown size. Then Half Received Interrupt becomes useless. Therefore, you have to disable Half Received Interrupt DMA (example code in the Final24C04interface project)
     REFERENCE: https://l.facebook.com/l.php?u=https%3A%2F%2Ftapit.vn%2Fhuong-dan-su-dung-chuc-nang-uart-idle-dma%2F%3Ffbclid%3DIwAR29hT4baoN5Z-h6i-bBnlOnGHm6Jly8q3RyE_re_T8I7FghtWKu0KTECiU&h=AT2xhWq5Fo5TkGLKItwZie6NwOASbDDHSivUzJnqFcNFnC6eTb9mPbQxna4obTVOcfvT5vlkiSYAoTM8MlrHyLAfKLqVyrauGNiXsW-s3D1vSIHsP1jDIE89u9XyTPAeiSR80w

  e. UART Error Handler: Let's talk about how we deal with UART connections. Most of the tutorials about UART assume that we are working on a stable hardware platform, in which no silly errors can be caught. This is not so right in 
     Real life application. What if during transmittion, Tx or Rx line is physically disconnected? You think the peripheral will continue doing its job? Dead wrong, it will jump into error handler (which has code in it) due to Clock mismatch. Then it will 
     stay disabled if you don't do anything to fix. Therefore you have to fix it by DeInit and then Init it again to make it work properly. Remember to Initialize the module to continue receive. (example code inthe Final 24C04interface project)


2. ADC DMA
  a. Example 
     Above is my project for reading ADC3 Channel 1 using DMA
  b. Some note
     For continous reading: Must enable Continuous DMA Request; For many channel reading: NumOfConversion
     To read ADC: Only 1 instruction needed: HAL_ADC_Start_DMA
  c. If you use Both ADC and UART DMA, DO REMEMBER TO DIVIDE YOUR ADC CLK/8, Interrupt of ADC DMA May occur too often, which caused your system to break down.

3. I2C 
  You can configure I2C easily with DMA and use it easily, remember not to run multiple I2C function at the same time, it would force
  the temprorary data line to release, which can cause crack. => Therefore you should wait for some miliseconds for the dataline to be stable and then start other task.

4. Custom Library: Example library is in project 24C04interface
   My standard pattern to make a library: https://fortunate-smash-efc.notion.site/Embedded-Programing-1ce6e9d260744f34bc9a1285a08cb28a

   NOTE: To declare and use global variables: in main.c declare extern => in other files, declare using it as usual.