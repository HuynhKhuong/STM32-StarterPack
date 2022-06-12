# ADC DMA

This Note focuses on ADC peripherals. How to initialize it and some notes for error countering

### Some notes
- For continous reading: Must enable Continuous DMA Request; For many channel reading: NumOfConversion

- To read ADC: Only 1 instruction needed: HAL_ADC_Start_DMA

- If you use Both ADC and UART DMA, **DO REMEMBER TO DIVIDE YOUR ADC CLK/8**, Interrupt of ADC DMA may occur too often, which caused your system to break down.

### Example code 
[DMA_ADC](./ADC_DMA/) is my project for reading ADC3 Channel 1 using DMA
