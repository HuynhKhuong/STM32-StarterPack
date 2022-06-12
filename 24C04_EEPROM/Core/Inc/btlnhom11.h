#ifndef _BTLNHOM11_H_
#define _BTLNHOM11_H_

#ifdef __cplusplus
extern "C" {
#endif 
#include "main.h"


#define	BYTE_SIZE  								1
#define BLOCK_SIZE 								256
#define RECEIVE_DATA_SIZE 				264


typedef enum 
{
	WRITE_BYTE 	=   0x00,
	WRITE_BLOCK = 	0x01,
	READ_BYTE		= 	0x02,
	READ_BLOCK	=		0x03,
} Operation_ModeTypeDef;

typedef enum 
{
	WRITE_ERROR = 	0x00,
	READ_ERROR	= 	0x01,
	WRITE_SUCCESS = 0x02,
	READ_SUCCESS = 	0x03,
} Status_TypeDef;

typedef struct 
{
	int Datasize;
	uint8_t Address;
	uint8_t Block;
	uint8_t Data[BLOCK_SIZE];
	uint8_t DataBuffer[8];
	uint8_t Trigger;
	Operation_ModeTypeDef Mode;	
}Eeprom_Typedef;


//extern Eeprom_Typedef My_Eeprom;
	
uint8_t * protocoldecode(uint8_t received_data[], uint16_t size, Eeprom_Typedef *Eeprom);
void protocolencode(Eeprom_Typedef *Eeprom, Status_TypeDef Status, uint8_t *data);
Status_TypeDef eeprom_blockwrite(Eeprom_Typedef *Eeprom, I2C_HandleTypeDef *i2c);
Status_TypeDef write_eeprom(Eeprom_Typedef *Eeprom, I2C_HandleTypeDef *i2c);
Status_TypeDef read_eeprom(Eeprom_Typedef *Eeprom, I2C_HandleTypeDef *i2c, uint8_t *data_read);

#ifdef __cplusplus
}
#endif
#endif
