STM32-StarterPack
I just started studying embedded coding on STM32 via STM32F407VG, many mistakes may be caused and i want to record all my mistakes as a cheatsheet for later projects. This Repository contains all my mistakes in all peripherals initializaion and usage, my notes for solution and my guide for Setting on STM32CubeMX, and (maybe) knowledges before starting.

1. UART Transmit/Receive DMA
  a. Notes before initializing on CubeMX
     In subsequent calls to HAL_UART_Transmit_DMA, the functions immediately returns
     HAL_BUSY because huart->gState != HAL_UART_STATE_READY
     This is the case because UART_DMATransmitCplt relies on the USART interrupt to be enabled to reset huart->gState to HAL_UART_STATE_READY. If you configure UART DMA in Normal mode without interrupts, HAL_UART_Transmit_DMA only transmits for the first time and no more transmittion after. => Must enable USART global interrupt.
     Reference: https://community.st.com/s/question/0D50X00009XkePT/hal-without-usart-interrupt-haluarttransmitdma-will-work-only-once.
  b. Becareful about configuring clock tree
     I've configured clock tree for HSE but entire UART worked incorrectly.
  c. Example
     Above is my project for read DMA UART Transmit and Receive. This project uses UART4, Transmit using Normal mode and Receive using Circular mode. In the future i will study more about IDLE Mode for more complex data transmit
2. ADC DMA
  a. Example 
     Above is my project for reading ADC3 Channel 1 using DMA
  b. Some note
     For continous reading: Must enable Continuous DMA Request; For many channel reading: NumOfConversion
     To read ADC: Only 1 instruction needed: HAL_ADC_Start_DMA
