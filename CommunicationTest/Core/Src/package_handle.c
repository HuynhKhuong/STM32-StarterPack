#include "package_handle.h"

//Constructor and Destructor
struct Package_Typedef* Package_Ctor(void)
{
	struct Package_Typedef* pointer = malloc(sizeof(struct Package_Typedef));
	if(pointer != NULL) {
		memset(pointer->buffer, 0, 50);
		memset(pointer->receive_package, 0 , 50);
		memset(pointer->response_package, 0, 100);
		pointer->mode = 0;
		pointer->status = OK;
		//Virtual Assign
		static struct package_vtbl new_virtualtable = {_IDget, _IDset, _LEDget, _LEDset, _Package_seal};
		pointer-> vptr = &new_virtualtable;
		
		//Assign array of virtual functions:
		pointer->func_pointer[0] = pointer->vptr->_IDget;
		pointer->func_pointer[1] = pointer->vptr->_IDset;
		pointer->func_pointer[2] = pointer->vptr->_LEDget;
		pointer->func_pointer[3] = pointer->vptr->_LEDset;

		return pointer;
	}
	return NULL;
}

void Package_Del(struct Package_Typedef *me){
	memset(me, 0, sizeof(*me));
	if (me != NULL) free(me);
}

//Processing
void Package_unfold(struct Package_Typedef *me){
	/**
		* @brief: Every kinds of package has the following process:
			1. Check the checksum (in this case is the Ending_syntax)
			2. Check mode including:
				+ Check mode get or set
				+ Check object (in this case is the LED and ID), the number of objects can vary
			3. Take values 
	*/

	/*1. Check the checksum
		The protocol is described as a string ending with syntax "\r\n"
		Check if get the ending syntax correctly?
	*/
	char* pointer1 = strstr(me->receive_package, Ending_Syntax);

	if(pointer1 == NULL) {
		me->status = PACKAGE_ERROR;
		return;
	}

	//Separate Ending_Syntax
	//char* pointer = NULL;
	char* pointer = strtok(me->receive_package, Ending_Syntax);
	pointer = strtok(pointer, Seperator);

	/*Create pointers to enumerate: 2*_enum_1 + _enum_2
		_enum_1: object
		_enum_2: get or set
	*/
	int _enum_1 = 2;			//Set to check errors
	int _enum_2 = 2;
	
	//Check Get, Set Command
	int i = strcmp(pointer, Get_Command);
	if(i == 0) _enum_2 = 0;
	i = strcmp(pointer, Set_Command);
	if(i == 0)	_enum_2 = 1;
	
	if(_enum_2 ==2) {
		me->status  = PACKAGE_ERROR;
		return;
	}

	//Check Object_Command
	pointer = strtok (NULL, Seperator);
	i = strcmp(pointer, LED_Command);
	if (i == 0) _enum_1 = 1;
	
	i = strcmp(pointer, ID_Command);
	if (i == 0) _enum_1 = 0;
	
	if (_enum_1 == 2) {
		me->status = PACKAGE_ERROR;
		return;
	}
	
	me->func_pointer[2*_enum_1 + _enum_2](me);
	
}

static packageState_Typedef _IDget(struct Package_Typedef *me){
		me->mode = 0;
		me->status= OK;
		return OK;
}

static packageState_Typedef _LEDget(struct Package_Typedef *me){
		me->mode = 2;
		me->status= OK;
		char* pointer;
		pointer = strtok(NULL, ":");
		pointer = strtok(NULL, ":");
		me->buffer[0] = *pointer - '0';
		return OK;
}

static packageState_Typedef _LEDset(struct Package_Typedef *me){
		me->mode = 3;
		me->status= OK;
		char* pointer;
		pointer = strtok(NULL, ":");
		pointer = strtok(NULL, ",");
			
		int ref = 0;
		if ((*pointer - '0') == 1) me->buffer[ref++] = 0;
		if ((*pointer - '0') == 0) me->buffer[ref++] = 1;

		pointer = strtok(NULL, ",");
		me->buffer[ref] = (*pointer - '0');
		return OK;
}

static packageState_Typedef _IDset(struct Package_Typedef *me){
		me->mode = 1;
		me->status= OK;

		//Get ID
		char* pointer;
		pointer = strtok(NULL, ":");
		pointer = strtok(NULL, ":");
		pointer = strtok(pointer, Data_Headers);
		strcpy(me->buffer, pointer);
		return OK;
}

static int _Package_seal(struct Package_Typedef* me){
		static void	(*func_enumerate_response[4])(struct Package_Typedef*) = {_IDget_response, _IDset_response, 
																																			_LEDget_response, _LEDset_response};
		func_enumerate_response[me->mode](me);
		return 3;
}

static void _IDget_response(struct Package_Typedef* me){
		int i = strncmp(me->buffer,Undefined_ID, 6);
		if (i == 0){
			strcpy(me->response_package,ID_response_Failed);
			return;
		}
		strcpy(me->response_package,"+ID:");
		strcat(me->response_package, me->buffer);
		strcat(me->response_package, State_response_Success);
		return;
}

static void _IDset_response(struct Package_Typedef* me){
	if(me->status == OK) {
		sprintf(me->response_package, State_response_Success);
		return;
	}
	sprintf(me->response_package, State_response_Failed);
	return;
}

static void _LEDget_response(struct Package_Typedef* me){
	sprintf(me->response_package, me->buffer, State_response_Success);
	return;
}

static void _LEDset_response(struct Package_Typedef* me){
	if(me->status == OK) {
		sprintf(me->response_package, State_response_Success);
		return;
	}
	sprintf(me->response_package, State_response_Failed);
	return;
}

int Package_seal(struct Package_Typedef* me){
	return me->vptr->_Packageseal(me);
}
