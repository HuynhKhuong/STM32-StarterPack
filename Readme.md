# STM32-StarterPack
> I just started studying embedded coding on STM32 via STM32F407VG, many mistakes may be caused and i want to record all my mistakes as a cheatsheet for later projects. This Repository contains all my mistakes in all peripherals initializaion and usage, my notes for solution and my guide for Setting on STM32CubeMX, and (maybe) knowledges before starting.

## 1. UART Transmit/Receive DMA
### a. Notes before initializing on CubeMX
If you configure UART Transmitt DMA in Normal mode:  **Must enable USART global interrupt**

**Error:**without interrupts, HAL_UART_Transmit_DMA only transmits for the first time and no more transmittion after. 

> In subsequent calls to HAL_UART_Transmit_DMA, the functions immediately returns HAL_BUSY because huart->gState != HAL_UART_STATE_READY.

> This is the case because UART_DMATransmitCplt relies on the USART interrupt to be enabled to reset huart->gState to HAL_UART_STATE_READY. 

*REFERENCE*: [HAL without usart interrupt will work only once](https://community.st.com/s/question/0D50X00009XkePT/hal-without-usart-interrupt-haluarttransmitdma-will-work-only-once).

**Becareful about configuring clock tree:** I've configured clock tree for HSE but entire UART worked incorrectly.
**Example:** **Test_DMA** is my project for read DMA UART Transmit and Receive. This project uses UART4, Transmit using Normal mode and Receive using Circular mode. In the future i will study more about IDLE Mode for more complex data transmit.
  
### b. UART4 Receive IDLE DMA:
> STM32 will automatically detect if the Receive line is free or not after a serie of incoming data. Then it will jump into an interrupt handler, which gives you some informations: data received, data length.

To do that, you have to configure your UART Receive Module into **Normal DMA mode**. After interrupt, you have to **initialize it to continue receive**. 
**ONE MORE THING**, At the beginning, DMA will automatically initialize Half Received Interrupt and Full Received Interrupt,which is quite useful for continuously receiving application and fixed data size application. In my case, I want UART to receive data with unknown size. Then Half Received Interrupt becomes useless. Therefore, you have to disable Half Received Interrupt DMA (example code in the **Final24C04interface** project).

```
HAL_UARTEx_ReceiveToIdle_DMA(&huart4, test_Rx_buff, RECEIVE_DATA_SIZE);
__HAL_DMA_DISABLE_IT(&hdma_uart4_rx, DMA_IT_HT);
```

*REFERENCE*: [How to configure UART Receive Idle](https://l.facebook.com/l.php?u=https%3A%2F%2Ftapit.vn%2Fhuong-dan-su-dung-chuc-nang-uart-idle-dma%2F%3Ffbclid%3DIwAR29hT4baoN5Z-h6i-bBnlOnGHm6Jly8q3RyE_re_T8I7FghtWKu0KTECiU&h=AT2xhWq5Fo5TkGLKItwZie6NwOASbDDHSivUzJnqFcNFnC6eTb9mPbQxna4obTVOcfvT5vlkiSYAoTM8MlrHyLAfKLqVyrauGNiXsW-s3D1vSIHsP1jDIE89u9XyTPAeiSR80w)

### c. UART Error Handler: 
Let's talk about how we deal with UART connections. Most of the tutorials about UART assume that we are working on a stable hardware platform, in which no silly errors could happen. This is not so right in real life application. What if during transmittion, Tx or Rx line is physically disconnected? 
You think the peripheral will continue doing its job? Dead wrong, it will jump into error handler (which has code in it) due to Baudrate mismatch. Then it will stay disabled if you don't do anything to fix. 
> Therefore you have to fix it by DeInit and then Init it again to make it work properly. Remember to Initialize the module to continue receive. (example code in the **Final24C04interface** project)
 ```
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_DeInit(&huart4);  
	HAL_UART_Init(&huart4);
    HAL_UARTEx_ReceiveToIdle_DMA(&huart4, test_Rx_buff, RECEIVE_DATA_SIZE);
	__HAL_DMA_DISABLE_IT(&hdma_uart4_rx, DMA_IT_HT);
}
```

## 2. ADC DMA
### a. Example 
**DMA_ADC** is my project for reading ADC3 Channel 1 using DMA
### b. Some note
- For continous reading: Must enable Continuous DMA Request; For many channel reading: NumOfConversion
- To read ADC: Only 1 instruction needed: HAL_ADC_Start_DMA
- If you use Both ADC and UART DMA, **DO REMEMBER TO DIVIDE YOUR ADC CLK/8**, Interrupt of ADC DMA may occur too often, which caused your system to break down.

## 3. I2C 
- You can configure I2C easily with DMA and use it easily, remember not to run multiple I2C function at the same time, it would force the temprorary data line to release, which can cause crack. => Therefore you should wait for some miliseconds for the dataline to be stable and then start other task.
- **Config DMA (both directions) into Normal mode:** I2C can easily get error if you config DMA into **circular mode**, it would cause STM32 to continuously ask Slave to read, and cause data line conflict. .
- [Some explanations about how HAL library works with I2C commands](https://stackoverflow.com/questions/38230248/how-do-i-use-the-stm32cubef4-hal-library-to-read-out-the-sensor-data-with-i2c?fbclid=IwAR2iq2CyQFAnMB6CFuYCcMLlap_2DaNZ8RYe8ZNhoJq8qPlUsn3GF-69CPg) 

## 4. Custom Library: 
*Example library is in project **24C04interface***

My standard pattern to make a library: [Notion](https://fortunate-smash-efc.notion.site/Embedded-Programing-1ce6e9d260744f34bc9a1285a08cb28a)

**NOTE:** To declare and use global variables: in main.c declare extern => in other files, declare using it as usual.

## 5. Electronic fundamentals 
> Some of my revision to about basic knowledges to understand more about electronic components
### a. How to choose Mosfet?
Mosfet introduction and how it works? [Mosfet introduction, check from 1:59](https://www.digikey.in/en/blog/how-to-select-a-mosfet-for-logic-circuits-or-gate-design)
> Brief: There is a form of *capacitor like* structure in the conduction channel. This is also called *Metal Oxide* term of the mosfet. Applying voltage to the gate allows current to flow throught the capacitor which begins to charge.

-> generate *electric field* : Electron gather at the oxide layer filling in the gap between the source and drain (*conductive region*).

-> When enough electrons have gathered to *create a path*, current flows in the conductive region.

-> Mosfet starts to turn on. The conductive region has a small amount of **resistance** when the MOSFET first turns on.

-> the **resistance** decreases until the Mosfet is fully on.
That's the main feature you have to consider when choosing your mosfet.

> Vgh (threshold) is just the threshold to for the Mosfet to start to turn on. In this case, a big resistance exists, and can cause damage to your application (*for example: Mosfet may burn out if you apply large current to Mosfet*). 

> Therefore, remember to choose the suitable one for your application. Make sure your Vgs is 0.5V - 1V higher than the Vgs threshold (check datasheet in the Electrical Characteristics). Vgs in the **Absolute Maximum Ratings** is the max Vgs voltage you can supply

One more thing: If you're using *N-type Mosfet*, your load must be on the **Drain** side.

*REFERENCE*: 
- [MOSFET HOW IT WORKS](https://www.digikey.in/en/blog/how-to-select-a-mosfet-for-logic-circuits-or-gate-design)
- [PROPER PICKING AND USING MOSFETS](https://www.youtube.com/watch?v=ND8uJWlOgIQ)

### b. Power Supply
Checking every power supply online, you can see that there are always two specifications: Voltage and Current. What should we understand those?
Most of popular power supply nowadays you can check online is *fixed voltage power supply*. That means when you apply this power supply can supply your circuit at **fixed voltage**. 

What about current? It obeys the Ohm rules. 
```
Current I = U/Z
```
That means I can vary at a specific range based on independance of your circuit. So that means you don't have to worry about the **I** specification unless your application exceeds the max I value your power supply can supply.

*REFERENCE*: [Điện trở hạn dòng/cản trở dòng như thế nào?](http://www.dientuvietnam.net/forums/forum/c%C6%A1-s%E1%BB%9F-%C4%90i%E1%BB%87n-%C4%90i%E1%BB%87n-t%E1%BB%AD/%C4%90i%E1%BB%87n-t%E1%BB%AD-d%C3%A0nh-cho-ng%C6%B0%E1%BB%9Di-m%E1%BB%9Bi-b%E1%BA%AFt-%C4%91%E1%BA%A7u/1644759-%C4%90i%E1%BB%87n-tr%E1%BB%9F-h%E1%BA%A1n-d%C3%B2ng-c%E1%BA%A3n-tr%E1%BB%9F-d%C3%B2ng-%C4%91i%E1%BB%87n-nh%C6%B0-th%E1%BA%BF-n%C3%A0o)

### c. How does pull up resistor work?
Check this link: [Pull-up Resistors](https://learn.sparkfun.com/tutorials/pull-up-resistors/all#:~:text=The%20pull%2Dup%20resistor%20R1,to%20read%20a%20high%20state.&text=If%20you%20have%20a%20really,pin%20can%20reliably%20change%20state.)

> **Pull up resistor** is a way to prevent *floating point* in the input of every MCU applications. Besides, **Pull up resistors** prevents shortage cases when you connect directly your *Voltage source* to *Ground* (through button).

That is a very common way of thinking. But let's see, if the input pin is connected to Vcc through **pull up resistor**, the voltage at the input pin V is obviously not equal to Vcc. It will experience a Voltage lost through **pull up resistor** right?

So how does the input pin still recognise high state? 
> It turns out, an input pin on a MCU has an impedance that can vary from 100k - 1M Ohm. That means, Current going from Vcc -> pull up resistor -> input pin -> ground is very minute -> drop out voltage is minute -> input pins still recognizes high voltage. 

So we can just take biggest resistor possible right? *Read the link above for more information*