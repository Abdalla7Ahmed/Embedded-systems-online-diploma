/*
 * STM32F103C6_I2C_Driver.c
 *
 *  Created on: ٠٥‏/٠٢‏/٢٠٢٣
 *      Author: Abotaleb
 */


// =======================================
// Includes
#include "STM32F103C6_I2C_Driver.h"
//========================================

// ===================================Generic Variables =========================
I2C_Config Global_I2C_Config[2]  = {0};

// ===========================================================================

// ===================================Generic Macros =========================
#define I2C1_index								0
#define I2C2_index								1

// ===========================================================================




// ===================================Generic functions =========================
void MCAL_I2C_GPIO_set_pins(I2C_TypeDef *I2Cx)
{

	/*
	 *  I2C pinout 					Configuration 				GPIO configuration
	I2Cx_SCL 					I2C clock 					Alternate function open drain
	I2Cx_SDA 					I2C Data I/O 				Alternate function open drain
	 */

	GPIO_PinConfig_t PinCfg ;

	if(I2Cx == I2C1)
	{
		//PB6 I2C1_SCL
		PinCfg.GPIO_PinNumber = GPIO_PIN_6;
		PinCfg.GPIO_MODE = GPIO_MODE_ALTERNATIVE_OUTPUT_OD;
		PinCfg.GPIO_OUTPUT_SPEED = GPIO_OUTPUT_SPEED_10M;
		MCAL_GPIO_INIT(GPIOB, &PinCfg);

		//PB7 I2C1_SDA
		PinCfg.GPIO_PinNumber = GPIO_PIN_7;
		PinCfg.GPIO_MODE = GPIO_MODE_ALTERNATIVE_OUTPUT_OD;
		PinCfg.GPIO_OUTPUT_SPEED = GPIO_OUTPUT_SPEED_10M;
		MCAL_GPIO_INIT(GPIOB, &PinCfg);

	}
	else if(I2Cx == I2C2)
	{
		//PB10 I2C2_SCL
		PinCfg.GPIO_PinNumber = GPIO_PIN_10;
		PinCfg.GPIO_MODE = GPIO_MODE_ALTERNATIVE_OUTPUT_OD;
		PinCfg.GPIO_OUTPUT_SPEED = GPIO_OUTPUT_SPEED_10M;
		MCAL_GPIO_INIT(GPIOB, &PinCfg);

		//PB11 I2C2_SDA
		PinCfg.GPIO_PinNumber = GPIO_PIN_11;
		PinCfg.GPIO_MODE = GPIO_MODE_ALTERNATIVE_OUTPUT_OD;
		PinCfg.GPIO_OUTPUT_SPEED = GPIO_OUTPUT_SPEED_10M;
		MCAL_GPIO_INIT(GPIOB, &PinCfg);
	}

}

void I2C_Generate_Start_bit(I2C_TypeDef *I2Cx,Function_State New_State,Start_Type Start_bit)
{
	// in case of repeated start bit the bus will be busy
	if(Start_bit != Repeated_Start_bit)
	{
		while(I2C_Get_Flag_State(I2Cx,I2C_Flag_Busy));
	}

	if(New_State !=Function_State_Disable)
	{
		// the Start bit
		I2Cx->CR1 |=I2C_CR1_START;
	}
	else
	{
		I2Cx->CR1 &=~(I2C_CR1_START);
	}
}
void I2C_Generate_Stop_bit(I2C_TypeDef *I2Cx,Function_State New_State)
{

	if(New_State !=Function_State_Disable)
	{
		// the stop bit
		I2Cx->CR1 |=I2C_CR1_STOP;
	}
	else
	{
		I2Cx->CR1 &=~(I2C_CR1_STOP);
	}

}
Flage_State I2C_Get_Flag_State(I2C_TypeDef *I2Cx,Stated_Flag Flag)
{
	Flage_State Bit_Stat;

	switch(Flag)
	{
	case I2C_Flag_Busy:
		Bit_Stat = ( (I2Cx->SR2) & I2C_SR2_BUSY)?Flag_SET:Flag_Reset;
		break;
	case I2C_Start_bit_Flag:
		Bit_Stat = ( (I2Cx->SR1) & I2C_SR1_SB)?Flag_SET:Flag_Reset;
		break;
	case I2C_Address_Mathced :
		Bit_Stat = ( (I2Cx->SR1) & I2C_SR1_ADDR)?Flag_SET:Flag_Reset;
		break;
	case I2C_EV_Master_Byte_Transmiting:
		Bit_Stat = (  ((I2Cx->SR1)  | ((I2Cx->SR2)<<16) ) == I2C_EV_Master_Byte_Transmiting) ?Flag_SET:Flag_Reset ;
	case EV8_2 :
		Bit_Stat = (( ( (I2Cx->SR1) & EV8_2) == EV8_2)) ?Flag_SET:Flag_Reset;
		break;
	case EV8_1:
	case I2C_Data_register_Empty:
		Bit_Stat =((I2Cx->SR1) & I2C_SR1_TXE)?Flag_SET:Flag_Reset;
		break;
	case I2C_Stop_bit_Flag:
		Bit_Stat =((I2Cx->SR1) & I2C_SR1_STOPF)?Flag_SET:Flag_Reset;
		break;
	case I2C_Data_register_NOT_Empty:
		Bit_Stat =((I2Cx->SR1) & I2C_SR1_RXNE)?Flag_SET:Flag_Reset;
		break;
	}
	return Bit_Stat;
}

void Send_Slave_Address(I2C_TypeDef *I2Cx,uint8_t device_Address,I2C_Direction Direction)
{
	uint8_t Index;
	device_Address = (device_Address<<1);
	Index =(I2Cx == I2C1)?I2C1_index:I2C2_index;
	if((Global_I2C_Config[Index].I2C_Slave_Device_Address.I2C_Addressing_Slave_Mode) ==I2C_Addressing_Slave_7bit)
	{
		if(Direction !=I2C_Direction_Transmitter)
		{
			device_Address |=(1<<0);
		}
		else
		{
			device_Address &=~(1<<0);
		}
		I2Cx->DR = device_Address;
	}
	else
	{
		// 10 bit addressing mode not supported
	}
}


void I2C_Acknowledge_Config(I2C_TypeDef *I2Cx,Function_State New_State)
{
	if(New_State !=Function_State_Disable)
	{
		I2Cx->CR1 |=I2C_CR1_ACK;
	}
	else
	{
		I2Cx->CR1 &=~(I2C_CR1_ACK);

	}
}


// ===================================APIS =========================

void MCAL_I2C_Init(I2C_TypeDef *I2Cx,I2C_Config *I2C_Config)
{

	uint32_t PCLK1 =0;
	uint32_t Tfrequency =0;
	uint16_t CCR_value =0 ;
	if (I2Cx == I2C1 )
	{
		RCC_I2C1_CLK_EN();
		Global_I2C_Config[I2C1_index] = *I2C_Config;
	}
	else if (I2Cx == I2C2 )
	{
		RCC_I2C2_CLK_EN();
		Global_I2C_Config[I2C2_index] = *I2C_Config;
	}




	if(I2C_Config->I2C_Mode == I2C_Mode_I2C)
	{
		/* ============================= init timing ===================== */
		/*   SM mode      Thigh = Tlow = (Ti2c)/2
		 * Thigh = Tlow = CCR * TPCLK1  	Ti2c = 2*CCR * TPCLK1
		 * CCR = (Ti2c/2*TPCLK1)   = (FPCLK1/2*Fi2c)
		 *
		 */
		PCLK1 = MCAL_RCC_GET_PCLK1Freq ();
		Tfrequency = ( (PCLK1 /1000000)& I2C_CR2_FREQ);

		if( (I2C_Config->I2C_clock_speed == I2C_SM_10K) ||
				(I2C_Config->I2C_clock_speed == I2C_SM_20K) ||
				(I2C_Config->I2C_clock_speed == I2C_SM_30K) ||
				(I2C_Config->I2C_clock_speed == I2C_SM_40K) ||
				(I2C_Config->I2C_clock_speed == I2C_SM_50K) ||
				(I2C_Config->I2C_clock_speed == I2C_SM_60K) ||
				(I2C_Config->I2C_clock_speed == I2C_SM_70K) ||
				(I2C_Config->I2C_clock_speed == I2C_SM_80K) ||
				(I2C_Config->I2C_clock_speed == I2C_SM_90K) ||
				(I2C_Config->I2C_clock_speed == I2C_SM_100K) )
		{
			I2Cx->CCR |=I2C_Config->I2C_Mode;            // by default CCR.0= 0

			CCR_value = (  (PCLK1 / ( (I2C_Config->I2C_clock_speed) << 1) ) &0x0FFF);  // clock speed *2
			I2Cx->CR2 |=Tfrequency;
			I2Cx->CCR |=CCR_value;

			/* ============================= Rise time configuration ===================== */
			//			For instance: in Sm mode, the maximum allowed SCL rise time is 1000 ns.
			//			If, in the I2C_CR2 register, the value of FREQ[5:0] bits is equal to 0x08 and TPCLK1 = 125 ns
			//			therefore the TRISE[5:0] bits must be programmed with 09h.
			//			(1000 ns / 125 ns = 8 + 1)
			I2Cx->TRISE = ( (Tfrequency + 1) & 0x3F);
		}
		else if(I2C_Config->I2C_Mode ==I2C_Mode_SMBus )
		{
			// Fast mode not supported yet

		}
		// the mode of I2C
		I2Cx->CR1 |=I2C_Config->I2C_Mode;
		// the stretching
		I2Cx->CR1 |=I2C_Config->stretching_mode;
		// the Acknowledge
		//large integer implicitly truncated to unsigned type ???
		I2Cx->CR1 |=( (I2C_Config->I2C_ACK_control) == I2C_ACK_Enable) ? (1<<10):(0<<10);
		//I2Cx->CR1 |=I2C_Config->I2C_ACK_control;
		//General call enable
		I2Cx->CR1 |=I2C_Config->I2C_General_call_address_detection;





		// for Slave Address
		if(I2C_Config->I2C_Slave_Device_Address.DUAL_Address_Enable == Dual_Address_Enable)
		{
			I2Cx->OAR2 |=I2C_OAR2_ENDUAL;
			if(I2C_Config->I2C_Slave_Device_Address.I2C_Addressing_Slave_Mode == I2C_Addressing_Slave_7bit)
			{
				I2Cx->OAR1 &=~(I2C_OAR1_ADDMODE);        // by default ADDMODE= 0
				I2Cx->OAR2 |=( (I2C_Config->I2C_Slave_Device_Address.Secondary_Slave_Address) <<(I2C_OAR2_ADD2_Pos) );
				I2Cx->OAR1 |=( (I2C_Config->I2C_Slave_Device_Address.Primary_Slave_Address) <<(I2C_OAR1_ADD1_Pos) );
			}
			else if(I2C_Config->I2C_Slave_Device_Address.I2C_Addressing_Slave_Mode == I2C_Addressing_Slave_10bit)
			{
				// not supported yet
				I2Cx->OAR1 |=I2C_OAR1_ADDMODE;
			}
		}
		else if(I2C_Config->I2C_Slave_Device_Address.DUAL_Address_Enable == Dual_Address_Disable)
		{
			if(I2C_Config->I2C_Slave_Device_Address.I2C_Addressing_Slave_Mode == I2C_Addressing_Slave_7bit)
			{
				I2Cx->OAR1 &=~(I2C_OAR1_ADDMODE);
				I2Cx->OAR1 |=( (I2C_Config->I2C_Slave_Device_Address.Primary_Slave_Address) <<(I2C_OAR1_ADD1_Pos) );
			}
			else if(I2C_Config->I2C_Slave_Device_Address.I2C_Addressing_Slave_Mode == I2C_Addressing_Slave_10bit)
			{
				// not supported yet
				I2Cx->OAR1 |=I2C_OAR1_ADDMODE;
			}
		}




		// Check if I2C is MAster or Slave
		// Master will be polling
		// Slave will be interrupt
		if(I2C_Config->P_Slave_Event_CallBack != NULL ) // Slave
		{
			// Enable IRQ
			I2Cx->CR2 |=I2C_CR2_ITERREN;
			I2Cx->CR2 |=I2C_CR2_ITEVTEN;
			I2Cx->CR2 |=I2C_CR2_ITBUFEN;

			if(I2Cx == I2C1)
			{
				NVIC_IRQ31_I2C1_EV_Enable;
				NVIC_IRQ32_I2C1_ER_Enable;
			}
			else if(I2Cx == I2C2)
			{
				NVIC_IRQ33_I2C1_EV_Enable;
				NVIC_IRQ34_I2C2_ER_Enable;
			}

			I2Cx->SR1 =0;
			I2Cx->SR2 =0;
		}


		// Enable I2C
		I2Cx->CR1 |=I2C_CR1_PE;
	}

	else if(I2C_Config->I2C_Mode == I2C_Mode_SMBus)
	{
		// SMBUS not supported
	}

	// Set GPIO pins
	MCAL_I2C_GPIO_set_pins(I2Cx);
}


void MCAL_I2C_DeInit(I2C_TypeDef *I2Cx)
{
	if(I2Cx == I2C1)
	{
		// Disable interrupt
		NVIC_IRQ31_I2C1_EV_Disable;
		NVIC_IRQ32_I2C1_ER_Disable;
		// Reset the Peripheral
		RCC_I2C1_Reset();
	}
	else if(I2Cx == I2C2)
	{
		// Disable interrupt
		NVIC_IRQ33_I2C1_EV_Disable;
		NVIC_IRQ34_I2C2_ER_Disable;
		// Reset the Peripheral
		RCC_I2C2_Reset();
	}

}




void MCAL_I2C_Master_TX(I2C_TypeDef *I2Cx,uint32_t device_Address,uint8_t *Data_Out,uint32_t DataLen,Stop_Condition Stop_bit,Start_Type Start_bit )
{

	int counter =0;
	/* todo
	support time out if the clock raised above time out will be interrupt and go out of this polling
	 */
	// generate Start bit
	I2C_Generate_Start_bit(I2Cx,Function_State_Enable,Start_bit);
	// Wait until start bit flag is rise
	// Wait for event EV5
	while(!(I2C_Get_Flag_State(I2Cx,I2C_Start_bit_Flag)));

	// send the Address of Slave followed by the Read Or Write command
	Send_Slave_Address(I2Cx,device_Address,I2C_Direction_Transmitter);
	// Wait EV6 The address of the slave is matched
	while(!(I2C_Get_Flag_State(I2Cx,I2C_Address_Mathced)));
	// Wait until (TXE ,TRA , MSL, Busy flag) is set
	while(!(I2C_Get_Flag_State(I2Cx,I2C_EV_Master_Byte_Transmiting)));
	// send the data
	for (counter = 0; counter < DataLen;counter++)
	{
		// send the buffer
		I2Cx->DR = Data_Out[counter];
		// wait  EV8: TxE=1, shift register not empty, d . ata register empty, cleared by writing DR register
		while(!(I2C_Get_Flag_State(I2Cx,I2C_Data_register_Empty)));
		// Generate Stop bit

	}
	// wait until transmit the last bit
	while(!(I2C_Get_Flag_State(I2Cx,EV8_2)));
	// stop bit
	if(Stop_bit == With_Stop_bit)
	{
		I2C_Generate_Stop_bit(I2Cx,Function_State_Enable);
	}
}
void MCAL_I2C_Master_RX(I2C_TypeDef *I2Cx,uint32_t device_Address,uint8_t *Data_Out,uint32_t DataLen,Stop_Condition Stop_bit,Start_Type Start_bit )
{
	int counter =0;
	uint8_t Index;
	Index =(I2Cx == I2C1)?I2C1_index:I2C2_index;

	// generate Start bit
	I2C_Generate_Start_bit(I2Cx,Function_State_Enable,Start_bit);
	// Wait until start bit flag is rise
	// Wait for event EV5
	while(!(I2C_Get_Flag_State(I2Cx,I2C_Start_bit_Flag)));

	// send the Address of Slave followed by the Read Or Write command
	Send_Slave_Address(I2Cx,device_Address,I2C_Direction_Receiver);
	// Wait EV6 The address of the slave is matched
	while(!(I2C_Get_Flag_State(I2Cx,I2C_Address_Mathced)));


	// Enable the Acknowledge
	I2C_Acknowledge_Config(I2Cx,Function_State_Enable);
	if(DataLen)
	{
		for (counter = DataLen; counter >1;counter--)
		{
			// wait until data register is not empty
			while(!(I2C_Get_Flag_State(I2Cx,I2C_Data_register_NOT_Empty)));
			// read the buffer
			*Data_Out =I2Cx->DR ;
			// increment the pointer
			Data_Out++;
		}

		// Disable the Acknowledge
		I2C_Acknowledge_Config(I2Cx,Function_State_Disable);
	}


	if(Stop_bit == With_Stop_bit)
	{
		I2C_Generate_Stop_bit(I2Cx,Function_State_Enable);
	}


	if(Global_I2C_Config[Index].I2C_ACK_control == I2C_ACK_Enable)
	{
		I2C_Acknowledge_Config(I2Cx,Function_State_Enable);
	}
}


void MCAL_I2C_Slave_TX(I2C_TypeDef *I2Cx,uint8_t buffer)
{
	I2Cx->DR = buffer;
}
uint8_t MCAL_I2C_Slave_RX(I2C_TypeDef *I2Cx)
{
	uint8_t buffer;
	buffer=I2Cx->DR;
	return buffer;

}




void I2C1_EV_IRQHandler()
{

}
void I2C1_ER_IRQHandler()
{

}
void I2C2_EV_IRQHandler()
{

}
void I2C2_ER_IRQHandler()
{

}


