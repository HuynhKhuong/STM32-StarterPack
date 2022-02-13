# STM32-StarterPack


I just started studying embedded coding on STM32 via STM32F407VG, many mistakes may be caused and i want to record all my mistakes as a cheatsheet for later projects. This Repository contains all my mistakes in all peripherals initializaion and usage, my notes for solution and my guide for Setting on STM32CubeMX, and (maybe) knowledges before starting.

- [UART Transmit/Receive DMA](UART.md)

	Comment and notes including 3 types of errors: How to initialize UART on CubeMX, UART Receive IDLE DMA, UART Error Handler 

- [ADC DMA](ADC.md)

	Comment and notes on how to basically use ADC: How continuous read ADC? How to read multiple ADCs? Code to Start ADC?...

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
That means I can vary at a specific range based on inpendance of your circuit. So that means you don't have to worry about the **I** specification unless your application exceeds the max I value your power supply can supply.

*REFERENCE*: [Điện trở hạn dòng/cản trở dòng như thế nào?](http://www.dientuvietnam.net/forums/forum/c%C6%A1-s%E1%BB%9F-%C4%90i%E1%BB%87n-%C4%90i%E1%BB%87n-t%E1%BB%AD/%C4%90i%E1%BB%87n-t%E1%BB%AD-d%C3%A0nh-cho-ng%C6%B0%E1%BB%9Di-m%E1%BB%9Bi-b%E1%BA%AFt-%C4%91%E1%BA%A7u/1644759-%C4%90i%E1%BB%87n-tr%E1%BB%9F-h%E1%BA%A1n-d%C3%B2ng-c%E1%BA%A3n-tr%E1%BB%9F-d%C3%B2ng-%C4%91i%E1%BB%87n-nh%C6%B0-th%E1%BA%BF-n%C3%A0o)

### c. How does pull up resistor work?
Check this link: [Pull-up Resistors](https://learn.sparkfun.com/tutorials/pull-up-resistors/all#:~:text=The%20pull%2Dup%20resistor%20R1,to%20read%20a%20high%20state.&text=If%20you%20have%20a%20really,pin%20can%20reliably%20change%20state.)

> **Pull up resistor** is a way to prevent *floating point* in the input of every MCU applications. Besides, **Pull up resistors** prevents shortage cases when you connect directly your *Voltage source* to *Ground* (through button).

That is a very common way of thinking. But let's see, if the input pin is connected to Vcc through **pull up resistor**, the voltage at the input pin V is obviously not equal to Vcc. It will experience a Voltage lost through **pull up resistor** right?

So how does the input pin still recognise high state? 
> It turns out, an input pin on a MCU has an impedance that can vary from 100k - 1M Ohm. That means, Current going from Vcc -> pull up resistor -> input pin -> ground is very minute -> drop out voltage is minute -> input pins still recognizes high voltage. 

So we can just take biggest resistor possible right? *Read the link above for more information*

### d. How to properly solder and how to keep tin clean? 
> Seems like this is a very simple topic. However, to most students who don't pratice regularly like me, it's time consuming to figure out things like this. 
1. **How to properly solder?** Remember to always keep your tin covered with soldering lead. 

First: *Soldering lead* is a very effective thermal conductive compound. Covering with *soldering lead* helps your tin to paste temperature better, and solder components quickly. Unless, you'll have to stick your tin to the component, wait for it to paste enough heat to melt the soldering lead. That would destroy your electrical components!

Second: *Soldering lead* coverage will help prevent your tin from oxide process. If your tin experiences oxide process, It will be unable to get your *Soldering lead* stick on it anymore. 

2. **How to clean your tin?** Oxide process is a bad affect on tins. However, we can't completely prevent that phenomenon. More or less, it will cause on some part of your tins. That's the reason why we have to clean the tin regularly. And [*How to clean it*?](https://www.youtube.com/watch?v=ijNXNFVNRBY)

Above is the link that I've chosen. On the internet, many tips are given. Some are quite controversial like *using sand paper*. I don't know the reason why but we'd better not touch on that method, do we? So below is the solution that we can use:
> **Tools**: Flux, Soldering lead, **wet** sponge, steel/brass wool.
> **Process**: Heat up your tin. Dip your tin into flux, the flux will help you get rid of most of oxide substances on your tin. Clean your tin with *sponge* or *brass wool*. Repeat that procedure until your tin is completely clean. Apply *soldering lead* on your tin to prevent it from oxide process again.

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
 
