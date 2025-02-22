﻿/*
 * LCD.c
 *
 * Created: 14/09/2022 01:27:34 AM
 *  Author: Abotaleb
 */ 

#include "lcd.h"

GPIO_PinConfig_t PinConfigLCD;

void LCD_INIT()
{
	Wait_m(20);
	// 	set EN_SWITCH , RW_SWITCH and RS_SWITCH as an output push pull
	PinConfigLCD.GPIO_PinNumber =RS_SWITCH;
	PinConfigLCD.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinConfigLCD.GPIO_OUTPUT_SPEED=GPIO_OUTPUT_SPEED_10M;
	MCAL_GPIO_INIT(LCD_CTRL,&PinConfigLCD);

	PinConfigLCD.GPIO_PinNumber =RW_SWITCH;
	PinConfigLCD.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinConfigLCD.GPIO_OUTPUT_SPEED=GPIO_OUTPUT_SPEED_10M;
	MCAL_GPIO_INIT(LCD_CTRL,&PinConfigLCD);

	PinConfigLCD.GPIO_PinNumber =EN_SWITCH;
	PinConfigLCD.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinConfigLCD.GPIO_OUTPUT_SPEED=GPIO_OUTPUT_SPEED_10M;
	MCAL_GPIO_INIT(LCD_CTRL,&PinConfigLCD);

	// Set LCD_Port as an output push pull with speed 10 MHZ
	PinConfigLCD.GPIO_PinNumber=GPIO_PIN_0;
	PinConfigLCD.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinConfigLCD.GPIO_OUTPUT_SPEED=GPIO_OUTPUT_SPEED_10M;
	MCAL_GPIO_INIT(LCD_PORT, &PinConfigLCD);

	PinConfigLCD.GPIO_PinNumber=GPIO_PIN_1;
	PinConfigLCD.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinConfigLCD.GPIO_OUTPUT_SPEED=GPIO_OUTPUT_SPEED_10M;
	MCAL_GPIO_INIT(LCD_PORT, &PinConfigLCD);

	PinConfigLCD.GPIO_PinNumber=GPIO_PIN_2;
	PinConfigLCD.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinConfigLCD.GPIO_OUTPUT_SPEED=GPIO_OUTPUT_SPEED_10M;
	MCAL_GPIO_INIT(LCD_PORT, &PinConfigLCD);

	PinConfigLCD.GPIO_PinNumber=GPIO_PIN_3;
	PinConfigLCD.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinConfigLCD.GPIO_OUTPUT_SPEED=GPIO_OUTPUT_SPEED_10M;
	MCAL_GPIO_INIT(LCD_PORT, &PinConfigLCD);

	PinConfigLCD.GPIO_PinNumber=GPIO_PIN_4;
	PinConfigLCD.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinConfigLCD.GPIO_OUTPUT_SPEED=GPIO_OUTPUT_SPEED_10M;
	MCAL_GPIO_INIT(LCD_PORT, &PinConfigLCD);

	PinConfigLCD.GPIO_PinNumber=GPIO_PIN_5;
	PinConfigLCD.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinConfigLCD.GPIO_OUTPUT_SPEED=GPIO_OUTPUT_SPEED_10M;
	MCAL_GPIO_INIT(LCD_PORT, &PinConfigLCD);

	PinConfigLCD.GPIO_PinNumber=GPIO_PIN_6;
	PinConfigLCD.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinConfigLCD.GPIO_OUTPUT_SPEED=GPIO_OUTPUT_SPEED_10M;
	MCAL_GPIO_INIT(LCD_PORT, &PinConfigLCD);

	PinConfigLCD.GPIO_PinNumber=GPIO_PIN_7;
	PinConfigLCD.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinConfigLCD.GPIO_OUTPUT_SPEED=GPIO_OUTPUT_SPEED_10M;
	MCAL_GPIO_INIT(LCD_PORT, &PinConfigLCD);

	MCAL_GPIO_WRITE_PIN(LCD_CTRL, EN_SWITCH, GPIO_PIN_RESET);
	MCAL_GPIO_WRITE_PIN(LCD_CTRL, RS_SWITCH, GPIO_PIN_RESET);
	MCAL_GPIO_WRITE_PIN(LCD_CTRL, RW_SWITCH, GPIO_PIN_RESET);
	Wait_m(15);

	LCD_clear_screen();
//#ifdef EIGHT_BIT_MODE
	LCD_WRITE_COMMAND(LCD_FUNCTION_8BIT_2LINES);
//#endif
//#ifdef FOUR_BIT_MODE
//	LCD_WRITE_COMMAND(0x02);
//	LCD_WRITE_COMMAND(LCD_FUNCTION_4BIT_2LINES);
//#endif

	LCD_WRITE_COMMAND(LCD_ENTRY_MODE);
	LCD_WRITE_COMMAND(LCD_BEGIN_AT_FIRST_ROW);
	LCD_WRITE_COMMAND(LCD_DISP_ON_CURSOR_BLINK);

}
void LCD_WRITE_COMMAND(unsigned char command)
{
//LCD_check_lcd_isbusy();
//#ifdef EIGHT_BIT_MODE
	//LCD_PORT = command;
	MCAL_GPIO_WRITE_PORT(LCD_PORT,(uint16_t)command);
	//CLEARBIT(LCD_CTRL,RW_SWITCH);
	MCAL_GPIO_WRITE_PIN(LCD_CTRL, RW_SWITCH, GPIO_PIN_RESET);
	//CLEARBIT(LCD_CTRL,RS_SWITCH);
	MCAL_GPIO_WRITE_PIN(LCD_CTRL, RS_SWITCH, GPIO_PIN_RESET);
	Wait_m(1);
	LCD_lcd_kick();
//#endif
//#ifdef FOUR_BIT_MODE
//	LCD_PORT = (LCD_PORT & 0x0F) | (command & 0xF0);
//	CLEARBIT(LCD_CTRL,RW_SWITCH);
//	CLEARBIT(LCD_CTRL,RS_SWITCH);
//	LCD_lcd_kick();
//	LCD_PORT = (LCD_PORT & 0x0F) | (command << DATA_shift);
//	CLEARBIT(LCD_CTRL,RW_SWITCH);
//	CLEARBIT(LCD_CTRL,RS_SWITCH);
//	LCD_lcd_kick();
//#endif

}
void LCD_WRITE_CHAR(unsigned char character)
{
//#ifdef EIGHT_BIT_MODE
	//LCD_check_lcd_isbusy();
	//LCD_PORT = character;
	MCAL_GPIO_WRITE_PORT(LCD_CTRL,(uint16_t)character);
	//CLEARBIT(LCD_CTRL,RW_SWITCH);
	MCAL_GPIO_WRITE_PIN(LCD_CTRL, RW_SWITCH, GPIO_PIN_RESET);
	//SETBIT(LCD_CTRL,RS_SWITCH);
	MCAL_GPIO_WRITE_PIN(LCD_CTRL, RS_SWITCH, GPIO_PIN_SET);
	Wait_m(1);
	LCD_lcd_kick();
//#endif
//#ifdef FOUR_BIT_MODE
//	LCD_PORT = (LCD_PORT & 0x0F) | (character & 0xF0);
//	SETBIT(LCD_CTRL,RS_SWITCH);
//	CLEARBIT(LCD_CTRL,RW_SWITCH);
//	LCD_lcd_kick();
//	LCD_PORT = (LCD_PORT & 0x0F) | (character <<DATA_shift);
//	SETBIT(LCD_CTRL,RS_SWITCH);
//	CLEARBIT(LCD_CTRL,RW_SWITCH);
//	LCD_lcd_kick();
//#endif

}

void LCD_WRITE_STRING(char* string)
{
	uint32_t counter=0;
	while (*string >0)
	{
		LCD_WRITE_CHAR(*string++);
		counter++;
		if (counter==16)
		{
			LCD_GOTO_XY(1,0);
		}
		else if (counter==32)
		{
			LCD_clear_screen();
			LCD_GOTO_XY(0,0);
			counter=0;
		}
	}

}

void LCD_check_lcd_isbusy(void)
{
	//DataDir_LCD_PORT &= ~(0xFF<<DATA_shift);
	// Set LCD_Port as an input to check if it busy ?
	PinConfigLCD.GPIO_PinNumber=GPIO_PIN_0;
	PinConfigLCD.GPIO_MODE=GPIO_MODE_FLOATINg_INPUT;
	MCAL_GPIO_INIT(LCD_PORT, &PinConfigLCD);

	PinConfigLCD.GPIO_PinNumber=GPIO_PIN_1;
	PinConfigLCD.GPIO_MODE=GPIO_MODE_FLOATINg_INPUT;
	MCAL_GPIO_INIT(LCD_PORT, &PinConfigLCD);

	PinConfigLCD.GPIO_PinNumber=GPIO_PIN_2;
	PinConfigLCD.GPIO_MODE=GPIO_MODE_FLOATINg_INPUT;
	MCAL_GPIO_INIT(LCD_PORT, &PinConfigLCD);

	PinConfigLCD.GPIO_PinNumber=GPIO_PIN_3;
	PinConfigLCD.GPIO_MODE=GPIO_MODE_FLOATINg_INPUT;
	MCAL_GPIO_INIT(LCD_PORT, &PinConfigLCD);

	PinConfigLCD.GPIO_PinNumber=GPIO_PIN_4;
	PinConfigLCD.GPIO_MODE=GPIO_MODE_FLOATINg_INPUT;
	MCAL_GPIO_INIT(LCD_PORT, &PinConfigLCD);

	PinConfigLCD.GPIO_PinNumber=GPIO_PIN_5;
	PinConfigLCD.GPIO_MODE=GPIO_MODE_FLOATINg_INPUT;
	MCAL_GPIO_INIT(LCD_PORT, &PinConfigLCD);

	PinConfigLCD.GPIO_PinNumber=GPIO_PIN_6;
	PinConfigLCD.GPIO_MODE=GPIO_MODE_FLOATINg_INPUT;
	MCAL_GPIO_INIT(LCD_PORT, &PinConfigLCD);

	PinConfigLCD.GPIO_PinNumber=GPIO_PIN_7;
	PinConfigLCD.GPIO_MODE=GPIO_MODE_FLOATINg_INPUT;
	MCAL_GPIO_INIT(LCD_PORT, &PinConfigLCD);

	//	SETBIT(LCD_CTRL,RW_SWITCH); // read mood
	MCAL_GPIO_WRITE_PIN(LCD_CTRL,RW_SWITCH,GPIO_PIN_SET);
	//CLEARBIT(LCD_CTRL,RS_SWITCH); // Write mood
	MCAL_GPIO_WRITE_PIN(LCD_CTRL,RS_SWITCH,GPIO_PIN_RESET);

	LCD_lcd_kick();
	//DataDir_LCD_PORT =0xFF;

	//CLEARBIT(LCD_CTRL,RW_SWITCH);
	MCAL_GPIO_WRITE_PIN(LCD_CTRL,RW_SWITCH,GPIO_PIN_RESET);

}
void LCD_lcd_kick(void)
{
	MCAL_GPIO_WRITE_PIN(LCD_CTRL,EN_SWITCH,GPIO_PIN_SET);
	Wait_m(50);
	MCAL_GPIO_WRITE_PIN(LCD_CTRL,EN_SWITCH,GPIO_PIN_RESET);
}
void LCD_clear_screen()
{
	LCD_WRITE_COMMAND(LCD_CLEAR_SCREEN);
}
void LCD_GOTO_XY(unsigned char line, unsigned char position){
	if (line == 0)
	{
		if (position < 16 && position >= 0)
		{
			LCD_WRITE_COMMAND(LCD_BEGIN_AT_FIRST_ROW+position);
		}
	}
	if (line == 1)
	{
		if (position < 16 && position >= 0)
		{
			LCD_WRITE_COMMAND(LCD_BEGIN_AT_SECOND_ROW+position);
		}
	}
}
//void LCD_DISPLAY_NUMBER(uint32_t number)
//{
//	char str[7];
//	sprintf(str,"%d",number);
//	LCD_WRITE_STRING(str);
//
//}
//void LCD_DISPLAY_REAL_NUMBER(double number)
//{
//	char str[16];
//	char *tmpsign=(number<0)? "-" : "";
//	float tmpval=(number<0) ? -number : number;
//	int tmpint1=tmpval;
//	float tmpfrac=tmpval-tmpint1;
//	int tmpint2=tmpfrac*10000;
//	sprintf(str,"%s%d.%04d",tmpsign,tmpint1,tmpint2);
//	LCD_WRITE_STRING(str);
//}


void Wait_m(uint32_t time)
{
	uint32_t i,j;
	for(i=0;i<time;i++)
	{
		for (j = 0; j < 255;j++) ;
	}
}
