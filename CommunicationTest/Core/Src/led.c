/*
 * led.c
 *
 *  Created on: May 23, 2022
 *      Author: DELL
 */
#include "led.h"


struct LED* Led_ctor(void){
	
	struct LED* pointer = malloc(sizeof(struct LED));
	if (pointer != NULL){
		pointer->cur_pin = RED;
		pointer->state = OFF;
		pointer->Led_pins[RED] = (uint16_t)0x0001;
	  pointer->Led_pins[GREEN] = (uint16_t)0x0002;
	  pointer->Led_pins[BLUE] = (uint16_t)0x0004;
	  pointer->Led_pins[ORANGE] =(uint16_t)0x0008;
		#ifdef PORT
		pointer->Port = GPIOA;
		#endif
		static struct vtbl new_virtualtable = {_setLEDstate, _getLEDstate};
		pointer->vtpr = &new_virtualtable;
	}
	return pointer;
}

void Led_setState(struct LED* const me, Led_State_Typedef state){
	me->state = state;
	me->vtpr->_setLEDstate(me);
}

Led_State_Typedef Led_getState(struct LED* me, Led_Color_Typedef LED_pins){
	me->cur_pin = LED_pins;
	return _getLEDstate(me);
}

static void _setLEDstate(struct LED* me){
	HAL_GPIO_WritePin(me->Port, me->Led_pins[me->cur_pin], (GPIO_PinState)me->state);
}

static Led_State_Typedef _getLEDstate(struct LED* me){
	return (Led_State_Typedef) HAL_GPIO_ReadPin(me->Port, me->Led_pins[me->cur_pin]);
}

void Led_dtor(struct LED* me){
	if (me!= NULL) free(me);
}
