#ifndef PACKAGE_HANDLE_H_
#define PACKAGE_HANDLE_H_

/**
	*@Creator: Huynh Khuong HCMUT
	*This class is created for handling the protocol created on this website:
	*"https://tapit.vn/huong-dan-xu-ly-chuoi-trong-lap-trinh-vi-dieu-khien/?amp=1"
	*This class' duty is to unfold and save the mode and data transmitted, the result 
	*of the transmittion (Successful or not)
**/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//Define params

#define State_response_Success "+OK\r\n"
#define State_response_Failed "+ERROR\r\n"
#define ID_response_Failed "ERROR:undefined\r\n"
#define Ending_Syntax "\r\n"
#define Set_Command "+SET"
#define Get_Command "+GET"
#define ID_Command "DEV"
#define LED_Command "LED"
#define Data_Headers "< >"
#define Data_Starter ":"
#define Data_Seperator ","
#define Seperator "_"
#define Undefined_ID "000000"
//The total number of objects is x
//2x commands required:
#define OBJECTS 2

typedef enum{
	OK = 0,
	PACKAGE_ERROR,
}packageState_Typedef;


struct Package_Typedef{
	struct package_vtbl *vptr; //virtual pointer
	char buffer[50];
	char receive_package[50];
	char response_package[100];
	char mode;
	packageState_Typedef (*func_pointer[OBJECTS*2])(struct Package_Typedef*);
	packageState_Typedef status;
};





//Virtual table
struct package_vtbl{
	packageState_Typedef (*_IDget)(struct Package_Typedef*);
	packageState_Typedef (*_IDset)(struct Package_Typedef*);
	packageState_Typedef (*_LEDget)(struct Package_Typedef*);
	packageState_Typedef (*_LEDset)(struct Package_Typedef*);
	int (*_Packageseal)(struct Package_Typedef*);
};

//Virtual methods
static packageState_Typedef _IDget(struct Package_Typedef* me);
static packageState_Typedef _IDset(struct Package_Typedef* me);
static packageState_Typedef _LEDset(struct Package_Typedef* me);
static packageState_Typedef _LEDget(struct Package_Typedef* me);
static int _Package_seal(struct Package_Typedef* me);

//Constructor and Destructor
struct Package_Typedef* Package_Ctor(void);
void Package_Del(struct Package_Typedef* me);

//Methods for interfacing outside class
int Package_seal(struct Package_Typedef* me);
void Package_unfold(struct Package_Typedef* me);

//Private function
static void _IDget_response(struct Package_Typedef* me);
static void _IDset_response(struct Package_Typedef* me);
static void _LEDget_response(struct Package_Typedef* me);
static void _LEDset_response(struct Package_Typedef* me);

#endif /* PACKAGE_HANDLE_H */
