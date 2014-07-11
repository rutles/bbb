// i2clcd.h - lcd library header file

#ifndef _I2CLCD_H
#define _I2CLCD_H

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define I2C_BUS "/dev/i2c-1"

// lcd_open - open /dev/i2c-1
// return - file discriptor, error: -1
int lcd_open(void);

// lcd_setup - setup i2c lcd
// argument - c: contrust 0-63
// return - success: 0, error: -1
int lcd_setup(uint8_t c);

// lcd_command - send command
// argument - c: command
void lcd_command(uint8_t c);

// lcd_setCursor - set cursor position
// argument - x: column, y: row
void lcd_setCursor(uint8_t x, uint8_t y);

// lcd_createChar - create charactor
// argument - ptn: charactor data
// format - ptn[0] = code, ptn[1..8] = font
void lcd_createChar(uint8_t *ptn);

// lcd_putch - send charactor
// argument - c: charactor
void lcd_putch(uint8_t c);

// lcd_puts - send string
// argument - same as printf
void lcd_printf(const char *format, ...);

#endif
