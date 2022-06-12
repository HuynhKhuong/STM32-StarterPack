/*
 * led.h
 *
 *  Created on: May 23, 2022
 *      Author: DELL
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "main.h"
#include "stdlib.h"
#include "stdio.h"
#include "stm32f1xx_hal_gpio.h"

#define PORT 
/**
* @brief: This class allows you to control 4 LEDs (4 GPIOs) on your developement board
					Based on the hardware configuration you have, this class is compatible with the MCU 
					with both complex GPIO (Port and Pins Declarations) and simple ones. 
	@Virtualfunction: _setLEDstate
	@Virtualattributes: Led_pins
**/

//These are attributes
typedef enum{
	GREEN = 0,
	ORANGE,
	RED,
	BLUE,
	NUM_OF_LED = 5,
}Led_Color_Typedef;

typedef enum{
	OFF,
	ON,
}Led_State_Typedef;


struct LED{
	uint16_t Led_pins[(int)NUM_OF_LED];
	Led_Color_Typedef cur_pin;
	Led_State_Typedef state;
	#ifdef PORT
	GPIO_TypeDef* Port;
	#endif
	struct vtbl* vtpr;
};

struct vtbl{
	void (*_setLEDstate)(struct LED*);
	Led_State_Typedef (*_getLEDstate)(struct LED*);
};

//constructor and destructor
struct LED* Led_ctor(void);
void Led_dtor(struct LED* me);

//virtual fucntion
static void _setLEDstate(struct LED* me);
static Led_State_Typedef _getLEDstate(struct LED* me);

//public methods
void Led_setState(struct LED* const me, Led_State_Typedef state);
Led_State_Typedef Led_getState(struct LED* const me, Led_Color_Typedef LED_pins);

#endif /* INC_LED_H_ */
