# STM32-StarterPack


I just started studying embedded coding on STM32 via STM32F407VG, many mistakes may be caused and i want to record all my mistakes as a cheatsheet for later projects. This Repository contains all my mistakes in all peripherals initializaion and usage, my notes for solution and my guide for Setting on STM32CubeMX, and (maybe) knowledges before starting.

- [UART Transmit/Receive DMA](UART.md)

	Comment and notes including 3 types of errors: How to initialize UART on CubeMX, UART Receive IDLE DMA, UART Error Handler 

- [ADC DMA](ADC.md)

	Comment and notes on how to basically use ADC: How continuous read ADC? How to read multiple ADCs? Code to Start ADC?...

- [I2C DMA](I2C.md)

	Comment and notes on how to basically use I2C Interface: How to Initialize I2C interface? How I2C works with other devices?...

- [Electrical components note](Electricacomponents.md)
	
	Some of my revision to about basic knowledges to understand more about electronic components
	
## 4. Custom Library: 
*Example library is in project **24C04interface***

My standard pattern to make a library: [Notion](https://fortunate-smash-efc.notion.site/Embedded-Programing-1ce6e9d260744f34bc9a1285a08cb28a)

**NOTE:** To declare and use global variables: in main.c declare extern => in other files, declare using it as usual.

## 6. C/C++ programming fundamentals
### a. Pointer
> Basically, pointer is a variable which stores memory address as it values

**How to declare a pointer**
```
[data_type] *(name of pointer);
```
**What's its application?**
- *First*, pointers can be parsed in a function as arguments. This helps you to do both things: avoid copies and share data. 

*A normal function which computes sum of 2 integers: this function parses 2 pointers*

```
int add(int *a, int *b)
{return (*a + *b);}
```

This function will take directly values of 2 variables and compute result. *If we build a function which pass 2 integers only*
```
int add(int a, int b)
{return (a+b)}
```
This function will copy values of 2 variales and store to function's local variables *a* and *b*, and compute

- *Second*, pointer to an array:
```
*Declare an array of integers*
int a[10];
```
*a* itself is the pointer to that array. It can also be used as an index to access to its array's members. By increasing and decreasing index as address values.

- *Third*, pointer to a struct:
```
*Declare a pointer to a struct*
[struct_type] *pointer;
```
Using pointer to access to its struct using: **->** operator.

- *Fourth*, function pointer: [Function pointers](function_pointer.md).
- *Fifth*, A very intuitive way to read pointers: [pointer's spiral rules](http://c-faq.com/decl/spiral.anderson.html).

**ESPECIALLY**, *function pointer* and *struct pointer* is the core of [*OOP based C programming method*](C_OOP_Programming.md)
 
