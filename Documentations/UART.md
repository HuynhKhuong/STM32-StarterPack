# UART Notes

This Note includes cases i've met when make use of UART DMA. Their errors and how to solve it. Comprising Example Codes.

## Initialize UART peripheral on CubeMX:

### Issue: 

When you configure UART Transmitt DMA in Normal mode without interrupts, HAL_UART_Transmit_DMA only transmits for the first time and no more transmittion after. 

In subsequent calls to HAL_UART_Transmit_DMA, the functions immediately returns HAL_BUSY because 
```
huart->gState != HAL_UART_STATE_READY.
```
This is because UART_DMATransmitCplt relies on the USART interrupt to be enabled to reset *huart->gState* to *HAL_UART_STATE_READY.* 

### Solution:

If you configure UART Transmitt DMA in Normal mode:  **Must enable USART global interrupt**. 

*REFERENCE*: [HAL without usart interrupt will work only once](https://community.st.com/s/question/0D50X00009XkePT/hal-without-usart-interrupt-haluarttransmitdma-will-work-only-once).

**Becareful about configuring clock tree:** I've configured clock tree for HSE but entire UART worked incorrectly.

### Example Code:

[TestDMA](/TestDMA) is my project using DMA UART Transmit and Receive. This project uses **UART4**, Transmit using **Normal mode** and **Receive using Circular mode**. In the future i will study more about IDLE Mode for more complex data transmit.
  
## UART Receive IDLE DMA:

### Issue: 

**My Expectation:** STM32 will automatically detect if the Receive line is free or not after a serie of incoming data. Then it will jump into an interrupt handler, which gives you some informations: data received, data length.

### Solution:
To do that, you have to configure your UART Receive Module into **Normal DMA mode**. After interrupt, you have to **initialize it to continue receive**. At the beginning, DMA will automatically initialize Half Received Interrupt and Full Received Interrupt,which is quite useful for continuously receiving application and fixed data size application. In my case, I want UART to receive data with unknown size. Then Half Received Interrupt becomes useless. Therefore, you have to disable Half Received Interrupt DMA.

```
HAL_UARTEx_ReceiveToIdle_DMA(&huart4, test_Rx_buff, RECEIVE_DATA_SIZE);
__HAL_DMA_DISABLE_IT(&hdma_uart4_rx, DMA_IT_HT);
```
*REFERENCE*: [How to configure UART Receive Idle](https://l.facebook.com/l.php?u=https%3A%2F%2Ftapit.vn%2Fhuong-dan-su-dung-chuc-nang-uart-idle-dma%2F%3Ffbclid%3DIwAR29hT4baoN5Z-h6i-bBnlOnGHm6Jly8q3RyE_re_T8I7FghtWKu0KTECiU&h=AT2xhWq5Fo5TkGLKItwZie6NwOASbDDHSivUzJnqFcNFnC6eTb9mPbQxna4obTVOcfvT5vlkiSYAoTM8MlrHyLAfKLqVyrauGNiXsW-s3D1vSIHsP1jDIE89u9XyTPAeiSR80w)

### Example code: 
[**Final24C04interface**](./Final24C04interface) is a mini project built to interface with **24C04 ROM**. In this project, i make use of UART Receive IDLE DMA technique to connect from STM32 to PC and it works quite well.


## UART Error Handler: 

### Issue:

Let's talk about how we deal with UART connections. Most of the tutorials about UART assume that we are working on a stable hardware platform, in which no silly errors could happen. This is not so right in real life application. What if during transmittion, Tx or Rx line is physically disconnected? 

You think the peripheral will continue doing its job? Dead wrong, it will jump into error handler (which has code in it) due to Baudrate mismatch. Then it will stay disabled if you don't do anything to fix. 

### Solution:

You have to fix it by DeInit and then Init it again to make it work properly. Remember to Initialize the module to continue receive:

 ```
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_DeInit(&huart4);  
	HAL_UART_Init(&huart4);
    HAL_UARTEx_ReceiveToIdle_DMA(&huart4, test_Rx_buff, RECEIVE_DATA_SIZE);
	__HAL_DMA_DISABLE_IT(&hdma_uart4_rx, DMA_IT_HT);
}
```
### Example code:

[**Final24C04interface**](./Final24C04interface) is a mini project built to interface with **24C04 ROM**. In this project, i make use of UART Error Handler to catch errors and fix them and it works quite well.