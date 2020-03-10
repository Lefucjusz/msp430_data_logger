/*
 * lcd.c
 *
 *  Created on: 6 mar 2020
 *      Author: Lefucjusz
 *      Heavily based on official TI LCD library
 */
#include "lcd.h"

//LCD Segment Mapping
const uint8_t lcd_char_map[] =
{
	LCD_A+LCD_B+LCD_C+LCD_D+LCD_E+LCD_F,        // '0' or 'O'
	LCD_B+LCD_C,                                // '1' or 'I'
	LCD_A+LCD_B+LCD_D+LCD_E+LCD_G,              // '2' or 'Z'
	LCD_A+LCD_B+LCD_C+LCD_D+LCD_G,              // '3'
	LCD_B+LCD_C+LCD_F+LCD_G,                    // '4' or 'y'
	LCD_A+LCD_C+LCD_D+LCD_F+LCD_G,              // '5' or 'S'
	LCD_A+LCD_C+LCD_D+LCD_E+LCD_F+LCD_G,        // '6'
	LCD_A+LCD_B+LCD_C,                          // '7'
	LCD_A+LCD_B+LCD_C+LCD_D+LCD_E+LCD_F+LCD_G,  // '8' or 'B'
	LCD_A+LCD_B+LCD_C+LCD_D+LCD_F+LCD_G,        // '9' or 'g'
	LCD_A+LCD_B+LCD_C+LCD_E+LCD_F+LCD_G,        // 'A'
	LCD_A+LCD_D+LCD_E+LCD_F,                    // 'C'
	LCD_B+LCD_C+LCD_D+LCD_E+LCD_G,              // 'd'
	LCD_A+LCD_D+LCD_E+LCD_F+LCD_G,              // 'E'
	LCD_A+LCD_E+LCD_F+LCD_G,                    // 'F'
	LCD_B+LCD_C+LCD_E+LCD_F+LCD_G,              // 'H'
	LCD_B+LCD_C+LCD_D+LCD_E,                    // 'J'
	LCD_D+LCD_E+LCD_F,                          // 'L'
	LCD_A+LCD_B+LCD_E+LCD_F+LCD_G,              // 'P'
	LCD_B+LCD_C+LCD_D+LCD_E+LCD_F,              // 'U'
	LCD_A+LCD_B+LCD_F+LCD_G,					// '°'
	LCD_D+LCD_E+LCD_F+LCD_G,					// 't'
	LCD_C+LCD_D+LCD_E+LCD_F+LCD_G       		// 'b'

};

const uint8_t LCD_MAX_CHARS = (sizeof(lcd_char_map)/sizeof(uint8_t));

void lcd_cls(void) //Clear LCD
{
	uint8_t i;
	for(i = LCD_MEM_OFFSET; i < (LCD_MEM_OFFSET+LCD_MEM_LOC);  i++)
	{
		LCDMEM[i] = 0;
	}
}

void lcd_init(void) //Initialize LCD_A
{
  lcd_cls(); //Clear LCD memory

  //Configure COM0-COM3 and R03-R33 pins
  P5SEL |= (BIT4 | BIT3 | BIT2);
  P5DIR |= (BIT4 | BIT3 | BIT2);

  //Configure LCD_A
  LCDACTL = LCDFREQ_128 | LCDMX1 | LCDMX0 | LCDSON | LCDON;
  LCDAPCTL0 = LCDS4 | LCDS8 | LCDS12 | LCDS16 | LCDS20 | LCDS24;
  LCDAPCTL1 = 0;
  LCDAVCTL0 = LCDCPEN;
  LCDAVCTL1 = VLCD_2_84;
}

void lcd_show_all(void) //Display all segments on LCD
{
	uint8_t i;
	for(i = LCD_MEM_OFFSET; i < (LCD_MEM_OFFSET+LCD_MEM_LOC); i++)
	{
		LCDMEM[i] = 0xff;
	}
}

void lcd_putchar(uint8_t pos, uint8_t index) //Display character on LCD
{
	LCDMEM[(LCD_NUM_DIGITS - pos) + LCD_MEM_OFFSET] &= ~lcd_char_map[8];
	if(pos <= LCD_NUM_DIGITS)
	{
		if(index < LCD_MAX_CHARS)
		{
			LCDMEM[(LCD_NUM_DIGITS - pos) + LCD_MEM_OFFSET] |= lcd_char_map[index];
		}
	}
}

void lcd_power_level(uint8_t lvl) //Display power level on LCD
{
	LCDMEM[7+LCD_MEM_OFFSET] &= ~(BIT1+BIT2+BIT3);
	LCDMEM[8+LCD_MEM_OFFSET] &= ~(BIT0+BIT1+BIT2+BIT3);
	switch(lvl)
	{
		case LCD_PWR_LVL_0:
			LCDMEM[7+LCD_MEM_OFFSET] |= (BIT1);
			LCDMEM[8+LCD_MEM_OFFSET] |= (BIT0);
		break;
		case LCD_PWR_LVL_1:
			LCDMEM[7+LCD_MEM_OFFSET] |= (BIT1);
			LCDMEM[8+LCD_MEM_OFFSET] |= (BIT0+BIT1);
		break;
		case LCD_PWR_LVL_2:
			LCDMEM[7+LCD_MEM_OFFSET] |= (BIT1);
			LCDMEM[8+LCD_MEM_OFFSET] |= (BIT0+BIT1+BIT2);
		break;
		case LCD_PWR_LVL_3:
			LCDMEM[7+LCD_MEM_OFFSET] |= (BIT1);
			LCDMEM[8+LCD_MEM_OFFSET] |= (BIT0+BIT1+BIT2+BIT3);
		break;
		case LCD_PWR_LVL_4:
			LCDMEM[7+LCD_MEM_OFFSET] |= (BIT1+BIT3);
			LCDMEM[8+LCD_MEM_OFFSET] |= (BIT0+BIT1+BIT2+BIT3);
		break;
		case LCD_PWR_LVL_5:
			LCDMEM[7+LCD_MEM_OFFSET] |= (BIT1+BIT2+BIT3);
			LCDMEM[8+LCD_MEM_OFFSET] |= (BIT0+BIT1+BIT2+BIT3);
		break;
	}
}

void lcd_function(uint8_t mask) //Display function F1 thru F5 on LCD
{
	uint8_t tmp;
	LCDMEM[7+LCD_MEM_OFFSET] &= ~0xF1;
	if(mask < LCD_FUNC_OFF)
	{
		tmp = mask << 4;

		if(mask & LCD_F5)
		{
			tmp |= BIT0;
		}

		LCDMEM[7+LCD_MEM_OFFSET] |= tmp;
	}
}

void lcd_battery_level(uint8_t lvl) //Display battery level on LCD
{
	LCDMEM[9+LCD_MEM_OFFSET] &= ~(BIT0+BIT1+BIT2+BIT3);

	switch(lvl)
	{
		case LCD_BATT_LOW:
			LCDMEM[9+LCD_MEM_OFFSET] |= (BIT0+BIT3);
		break;
		case LCD_BATT_HALF:
			LCDMEM[9+LCD_MEM_OFFSET] |= (BIT0+BIT2+BIT3);
		break;
		case LCD_BATT_FULL:
			LCDMEM[9+LCD_MEM_OFFSET] |= (BIT0+BIT1+BIT2+BIT3);
		break;
	}
}

void lcd_signal_level(uint8_t lvl) //Display signal level on LCD
{
	LCDMEM[9+LCD_MEM_OFFSET] &= ~(BIT4+BIT5+BIT6+BIT7);
	switch(lvl)
	{
		case LCD_SIG_LVL_0:
			LCDMEM[9+LCD_MEM_OFFSET] |= (BIT4);
		break;
		case LCD_SIG_LVL_1:
			LCDMEM[9+LCD_MEM_OFFSET] |= (BIT4+BIT7);
		break;
		case LCD_SIG_LVL_2:
			LCDMEM[9+LCD_MEM_OFFSET] |= (BIT4+BIT6+BIT7);
		break;
		case LCD_SIG_LVL_3:
			LCDMEM[9+LCD_MEM_OFFSET] |= (BIT4+BIT5+BIT6+BIT7);
		break;
	}
}

void lcd_arrow(uint8_t mask) //Display arrow on LCD
{
	LCDMEM[8+LCD_MEM_OFFSET] &= ~(BIT4+BIT5+BIT6+BIT7);

	if(mask < LCD_ARROW_OFF)
	{
		LCDMEM[8+LCD_MEM_OFFSET] |= (mask << 4);
	}
}

void lcd_symbol(uint8_t mask) //Display symbol on LCD
{
	uint8_t tmp;
	LCDMEM[10+LCD_MEM_OFFSET] &= ~(BIT0+BIT1+BIT2+BIT4+BIT5+BIT6+BIT7);

	if(mask < LCD_SYM_OFF)
	{
		tmp = mask << 4;
		tmp |= mask >> 4;

		LCDMEM[10+LCD_MEM_OFFSET] |= tmp;
	}
}

void lcd_special_char(uint16_t mask) // Display symbol on LCD
{
	uint16_t x;
	uint8_t index = 0;

	if(mask & LCD_8BC)
	{
		LCDMEM[10+LCD_MEM_OFFSET] |= BIT3;
	}
	else
	{
		LCDMEM[10+LCD_MEM_OFFSET] &= ~BIT3;
	}

	for(x = 1; x < LCD_8BC; x <<= 1, index++)
	{
		if(x & mask)
		{
			LCDMEM[index+LCD_MEM_OFFSET] |= BIT7;
		}
		else
		{
			LCDMEM[index+LCD_MEM_OFFSET] &= ~(BIT7);
		}
	}
}
