// glcd.h AQM1248A handler header file

#ifndef _GLCD_H
#define _GLCD_H

#define CAPE_DTBO "BB-SPIDEV0"
#define SPI_CH "/dev/spidev1.0"
#define GPIO_RS 48
#define CONTRUST 0x1c
#define GL_WIDTH 60


// gl_setup - setup AQM1248A
// argument - contrast: contrust
// return - success flag
int gl_setup(uint8_t contrust);

// gl_locate - set text drawing position
// argument - p: page, c: column
void gl_locate(uint8_t p, uint8_t c);

// gl_printf - print string
// argument - same as printf
void gl_printf(const char *format, ...);

// gl_bigprintf - print string by BIGFONT
// argument - p: page, c: column, format, ...: same as printf
void gl_bigprintf(uint8_t p, uint8_t c, const char *format, ...);

// gl_set - set data to graphic buffer
// argument - data: 0..47
void gl_set(int data);

// gl_refresh - refresh graphic area by graphic buffer
void gl_refresh();

#endif
