# STM32-StarterPack
I just started studying embedded coding on STM32 via STM32F407VG, many mistakes may be caused and i want to record all my mistakes as a cheatsheet for later projects.
This Repository contains all my mistakes in all peripherals initializaion and usage, my notes for solution and my guide for Setting on STM32CubeMX, and (maybe) knowledges before starting.
# Table of Contents
1. [UART Transmit/Receive DMA](#UART-Transmit/Receive-DMA)
   a. [Notes on before initializing on CubeMX] (#Notes-before-initializing-on-CubeMX)


## Headers
# 1.UART Transmit/Receive DMA
#  a.Notes before initializing on CubeMX
In subsequent calls to HAL_UART_Transmit_DMA, the functions immediately returns HAL_BUSY because huart->gState != HAL_UART_STATE_READY. 
This is the case because UART_DMATransmitCplt relies on the USART interrupt to be enabled to reset huart->gState to HAL_UART_STATE_READY.
If you configure UART DMA in Normal mode without interrupts, HAL_UART_Transmit_DMA only transmits for the first time and no more transmittion after.
=> Must enable USART global interrupt. 
