/*
 * lcd.h
 *
 *  Created on: 6 mar 2020
 *      Author: Lefucjusz
 */

#ifndef LCD_H_
#define LCD_H_

#include <msp430xG46x.h>
#include <stdint.h>

// LCD Constants
#define LCD_NUM_DIGITS  7                   // Number of digits on LCD
#define LCD_MEM_OFFSET  2                   // Offset from LCDMEM[0]
#define LCD_MEM_LOC 11                  	// Num of LCDMEM[] locations used

// LCD Segments
#define LCD_A BIT0
#define LCD_B BIT1
#define LCD_C BIT2
#define LCD_D BIT3
#define LCD_E BIT6
#define LCD_F BIT4
#define LCD_G BIT5
#define LCD_H BIT7

extern const uint8_t LCD_MAX_CHARS;

void lcd_cls(void); //Clear LCD
void lcd_init(void); //Initialize LCD_A
void lcd_show_all(void); //Display all segments on LCD

void lcd_putchar(uint8_t pos, uint8_t index); //Display character on LCD

#define LCD_PWR_LVL_0   0x01
#define LCD_PWR_LVL_1   0x02
#define LCD_PWR_LVL_2   0x04
#define LCD_PWR_LVL_3   0x08
#define LCD_PWR_LVL_4   0x10
#define LCD_PWR_LVL_5   0x20
#define LCD_PWR_OFF     0x40

void lcd_power_level(uint8_t lvl); //Display power level on LCD

#define LCD_F1          0x01
#define LCD_F2          0x02
#define LCD_F3          0x04
#define LCD_F4          0x08
#define LCD_F5          0x10
#define LCD_FUNC_OFF    0x20

void lcd_function(uint8_t mask); //Display function F1 thru F5 on LCD

// Display Battery Level
#define LCD_BATT_LOW    0x01
#define LCD_BATT_HALF   0x02
#define LCD_BATT_FULL   0x04
#define LCD_BATT_OFF    0x08

void lcd_battery_level(uint8_t lvl); //Display battery level on LCD

#define LCD_SIG_LVL_0   0x01
#define LCD_SIG_LVL_1   0x02
#define LCD_SIG_LVL_2   0x04
#define LCD_SIG_LVL_3   0x08
#define LCD_SIG_OFF     0x10

void lcd_signal_level(uint8_t lvl); //Display signal level on LCD

#define LCD_ARROW_UP     0x01
#define LCD_ARROW_RIGHT  0x02
#define LCD_ARROW_DOWN   0x04
#define LCD_ARROW_LEFT   0x08
#define LCD_ARROW_OFF    0x10

void lcd_arrow(uint8_t mask); //Display arrow on LCD

#define LCD_SYM_DOL      0x01
#define LCD_SYM_ERR      0x02
#define LCD_SYM_MINUS    0x04
#define LCD_SYM_MEM      0x08
#define LCD_SYM_ENV      0x10
#define LCD_SYM_TX       0x20
#define LCD_SYM_RX       0x40
#define LCD_SYM_OFF      0x80

void lcd_symbol(uint8_t mask); //Display symbol on LCD

#define LCD_DP7          0x0001
#define LCD_DP6          0x0002
#define LCD_COL5         0x0004
#define LCD_DP4          0x0008
#define LCD_COL3         0x0010
#define LCD_DP2          0x0020
#define LCD_DP1          0x0040
#define LCD_8BC          0x0080
#define LCD_SPC_CHAR_OFF 0x0100

void lcd_special_char(uint16_t mask); //Display special character on LCD

#endif /* LCD_H_ */
