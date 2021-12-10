#include "btlnhom11.h"
#include "stm32f4xx_hal_i2c.h"

extern uint8_t lock;
uint8_t address;
uint8_t EEPROM_ADDRESS = 0xA0; // dia chi ngoai vi
/**
  * @brief  Decode Mode in ASCII form to Mode
  * @note   Protocol includes: [W/R][1/2/3]
  * @param  Mode Protocol arrays
	* @param  decoded Mode
  * @retval Operation_ModeTypeDef;
  */
	
Operation_ModeTypeDef modedecode(uint8_t received_data[])
{
		//Decode for Operation mode
	switch (received_data[0])
	{
		case 'W':
			if (received_data[1] == '0') return WRITE_BYTE;
			else if (received_data[1] == '1') return WRITE_BLOCK;
			break;
		case 'R':
			if (received_data[1] == '0')return READ_BYTE;
			else if (received_data[1] == '1') return READ_BLOCK;
			break;
		default: return WRITE_BYTE;
	}
}


/**
  * @brief  Decode Address bytes in ASCII form to single Address byte
  * @note   Protocol includes: [ADD1][ADD2]
  * @param  address Protocol arrays
	* @param  decoded address arrays
  * @retval address byte
  */
uint8_t	addressdecode(uint8_t address_data[])
{
	uint8_t result = 0;
	for (int i = 0; i < 2; i++)
	{
		if (address_data[i] > 0x39)
		{
			address_data[i] = address_data[i] - 0x41 +10;
		}
		else address_data[i] = address_data[i] - '0';
	}	
	result = address_data[1] + address_data[0]*16;
	return result;
}

/**
  * @brief  Decode the Protocol arrays between PC and STM32F407.
  * @note   Protocol includes: [W/R/E][1/2/3][A][BLOCK][ADD1][ADD2][D][BYTESARRAY][*]
  * @param  received_data Protocol arrays
	* @param  decoded data arrays
  * @retval pointer to data arrays
  */
 
uint8_t * protocoldecode(uint8_t received_data[], uint16_t size, Eeprom_Typedef *Eeprom)
{
	static uint8_t feedback[2];
	uint8_t mode_data[2];
	uint8_t address_data[2];
	uint8_t flag = 0;
	uint8_t count = 0;
	for (uint8_t i = 0; i<2; i++) feedback[i] = '0';
	for (int i = 0; i < size; i++)
	{
		if ((feedback[0] == 'F')&&(feedback[1] == '5')) break;
		switch (flag)
		{	
			case 0: 
			{
				if(i == 2) 
				{
					if(received_data[i] == 'A')
					{
						Eeprom -> Mode = modedecode(mode_data);
						if (Eeprom ->Mode == WRITE_BLOCK)
						{
							if (size < 264)
							{
								feedback[0] = 'F';
								feedback[1] = '5';
							}
							Eeprom->Datasize = BLOCK_SIZE;
						}
						else if (Eeprom ->Mode == READ_BLOCK)
						{
							if (size != 8)
							{
								feedback[0] = 'F';
								feedback[1] = '5';
							}
							Eeprom->Datasize = BLOCK_SIZE;
						}
						else 
						{
							if (size != 9)
							{
								feedback[0] = 'F';
								feedback[1] = '5';
							}
							Eeprom->Datasize = BYTE_SIZE;
						}
						flag = 1;
					}
					else
					{
						feedback[0] = 'F';
						feedback[1] = '5';
					}		
				}
				else mode_data[i] = received_data[i];
				break;
			}	
			case 1:
			{
				Eeprom->Block = received_data[i] - '0';
				flag = 2;
				break;
			}
			case 2:
			{
				if(i == 6) 
				{
					if(received_data[i] == 'D')
					{	if ((Eeprom ->Mode == WRITE_BLOCK) || (Eeprom ->Mode == READ_BLOCK))
						{
							address_data[0] ='0';
							address_data[1] ='0';
						}
						Eeprom->Address = addressdecode(address_data);
						count = 0;
						flag = 3;
					}
					else
					{
						feedback[0] = 'F';
						feedback[1] = '5';
					}
				}
				else 
				{
					address_data[count] = received_data[i];
					count++;
				}
				break;
			}
			case 3:
			{
				if (i < size -1) 
				{
					Eeprom->Data[count] = received_data[i];
					count++;
				}
				else if (received_data[i] == '*') //End of string
				{
					feedback[0] = 'F';
					feedback[1] = '6';
				}
			}
		}
	}
	if ((feedback[0] == 'F')||(feedback[1] == '6')) Eeprom->Trigger = 1;
	return feedback;
}

/**
  * @brief  Encode result to feedback to PC 
  * @note   Protocol includes: [W/R/E][Result]
  * @param  received_data Protocol arrays
	* @param  decoded data arrays
  * @retval Eeprom_Typedef
  */
void protocolencode(Eeprom_Typedef *Eeprom, Status_TypeDef Status, uint8_t *data)
{
	switch (Status)
	{
		case WRITE_ERROR:
		{
			Eeprom -> Data[0] = 'F';
			Eeprom -> Data[1] = '3';
			break;
		}
		case READ_ERROR:
		{
			Eeprom -> Data[0] = 'F';
			Eeprom -> Data[1] = '2';
			break;
		}
		case WRITE_SUCCESS :
		{
			Eeprom -> Data[0] = 'F';
			Eeprom -> Data[1] = '4';
			break;
		}
		case READ_SUCCESS :
		{
			if (Eeprom -> Mode == READ_BYTE)
			{
				Eeprom -> Data[0] = 'F';
				Eeprom -> Data[1] = '1';
				Eeprom -> Data[2] = 'D';
				Eeprom -> Data[3] = data[0];
				Eeprom -> Data[4] = '*';
			}
			else if (Eeprom -> Mode == READ_BLOCK)
			{
				for(int i = 0; i < BLOCK_SIZE ; i++)
				{
					Eeprom -> Data[i] = data[i];
				}
			}
			break;
		}
	}
}



Status_TypeDef read_eeprom(Eeprom_Typedef *Eeprom, I2C_HandleTypeDef *i2c, uint8_t *data_read)
{
	int timercounter = 0;
	if (HAL_I2C_IsDeviceReady(i2c, EEPROM_ADDRESS, 2, 100) != HAL_OK)  return READ_ERROR;
  else
	{
		switch(Eeprom -> Mode)
		{
			case READ_BYTE:
			{
				Eeprom->Trigger = 0;
				address = EEPROM_ADDRESS;
				address |= 	Eeprom -> Block<< 1;
				//Clear the data arrays
				for(int i = 0; i < BLOCK_SIZE; i++) Eeprom->Data[i] = ' ';
				if (HAL_I2C_Mem_Read_DMA(i2c, address, Eeprom -> Address, 1, data_read, Eeprom->Datasize) == HAL_OK)
				{
					HAL_Delay(5);
					return READ_SUCCESS;
				}			
				else return READ_ERROR;
			}
			case READ_BLOCK:
			{
				Eeprom->Trigger = 0;
				address = EEPROM_ADDRESS;
				address |= 	Eeprom -> Block<< 1;
				HAL_I2C_Mem_Read_DMA(i2c, address, Eeprom -> Address, 1, data_read, Eeprom -> Datasize);
				lock  = 1;
				HAL_Delay(100);
				while (timercounter <= 100)
				{
					if(lock == 0x00) 
					{
						return READ_SUCCESS;
					}
					else 
					{
						timercounter += 1;
						if (timercounter == 100)
						{
							lock = 0;
							timercounter = 0;
							return READ_ERROR;
						}
					}
				}
			}
			default:
			{
				Eeprom->Trigger = 0;
				return READ_ERROR;
			}
		}
	}
}
Status_TypeDef write_eeprom(Eeprom_Typedef *Eeprom, I2C_HandleTypeDef *i2c)
{
  if (HAL_I2C_IsDeviceReady(i2c, EEPROM_ADDRESS, 2, 100) != HAL_OK)  return WRITE_ERROR;
  else
	{
		switch(Eeprom -> Mode)
		{
			case WRITE_BYTE:
			{
				Eeprom->Trigger = 0;
				address = EEPROM_ADDRESS;
				address |= 	Eeprom -> Block<< 1;
				if (HAL_I2C_Mem_Write_DMA(i2c, address, Eeprom -> Address, 1, Eeprom->Data, Eeprom->Datasize) == HAL_OK)
					return WRITE_SUCCESS;
				else return WRITE_ERROR;
			}
			case WRITE_BLOCK:
			{
				Eeprom->Trigger = 0;
				return eeprom_blockwrite(Eeprom, i2c);
			}
			default:
			{
				Eeprom->Trigger = 0;
				return WRITE_ERROR;
			}
		}
	}
}
Status_TypeDef eeprom_blockwrite(Eeprom_Typedef *Eeprom, I2C_HandleTypeDef *i2c)
{
	int count;
	int timercounter = 0;
	address = EEPROM_ADDRESS;
	address |= 	Eeprom -> Block<< 1;
	// Switch to 8 bytes write
	HAL_GPIO_WritePin(MODE_GPIO_Port, MODE_Pin, GPIO_PIN_RESET); 
	if (HAL_I2C_IsDeviceReady(i2c, EEPROM_ADDRESS, 2, 100) != HAL_OK) return WRITE_ERROR;
	else 
		{
			for(count = 0; count < 32; count++)
			{
				//Parse 8bytes from data to data_buffer
				for (int i = 0; i<8; i++) Eeprom->DataBuffer[i] = Eeprom->Data[i+count*8];
				HAL_I2C_Mem_Write_DMA(i2c, address, Eeprom -> Address, 1, Eeprom -> DataBuffer, 8);
				lock  = 1;
				HAL_Delay(5);
				while (timercounter <= 100)
				{
					if(lock == 0x00) 
					{
						break;
					}
					else 
					{
						timercounter += 1;
						if (timercounter == 100)
						{
							lock = 0;
							timercounter = 0;
							return WRITE_ERROR;
						}
					}
				}
	
				Eeprom -> Address += 8;
			}
			return WRITE_SUCCESS;
		}
		
	
}
