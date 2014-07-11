// bbb.h - BeagleBone Black library header file

#ifndef _BBB_H
#define _BBB_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/mman.h>

// cape_path - get fullpath to file include unknown number
// argument - dir: directry, s: known part of filename
// return - fullpath to file
char *cape_path(const char *dir, const char *s);

// cape_number - get slot number of DTBO
// argument - dtbo: DTBO filename
// return - slot number
int cape_number(const char *dtbo);

// cape_remove - remove DTBO from slot
// argument - dtbo: DTBO filename
// return - success flag
int cape_remove(const char *dtbo);

// cape_add - add DTBO to slot
// argument - dtbo: DTBO filename
// return - success flag
int cape_add(const char *dtbo);

// gpio_export - activate a gpio
// argument - n: GPIO number
void gpio_export(int n);

// gpio_unexport - deactivate a gpio
// argument - n: GPIO number
void gpio_unexport(int n);

// gpio_open - open GPIO /sys/class/gpio*/* file
// argument - n: GPIO number, file: file name, flag: one of following
// O_RDONLY, O_WRONLY, O_RDWR
// return - file descriptor
int gpio_open(int n, char *file, int flag);

#define GPIO_PORT(n) (n / 32)
#define GPIO_MASK(n) (1 << (n % 32))

// port_regs - open GPIO /dev/mem file
// argument - n: GPIO number
// return - mapped address
uint32_t *port_mmap(int n);

// port_outp - output to port
// argument - regs: mapped address, value: output value
void port_outp(uint32_t *regs, uint32_t value);

// port_set - set bits of port
// argument - regs: mapped address, mask: bit pattern
void port_set(uint32_t *regs, uint32_t mask);

// port_clear - clear bits of port
// argument - regs: mapped address, mask: bit pattern
void port_clear(uint32_t *regs, uint32_t mask);

// port_inp - input from port
// argument - regs: mapped address
// return - input value
uint32_t inp(uint32_t *regs);

// pwm_setup - setup PWM in nsec
// argument - pname: phisycal pin name, period: period, duty: duty
// return - success flag
int pwm_setup(const char* pname, int period, int duty);

// pwm_open - open PWM /sys/devices/ocp.*/pwm_test_P*_*/* file
// argument - pname: phisycal pin name, fname: file name
// return - file descriptor
int pwm_open(const char* pname, const char *fname);

// w1_setup - 1-Wire setup
// argument - dto: DTO filename
// return - device ID
char *w1_setup(const char* dto);

// w1_read - read DS18B20+
// argument - id: device ID
// return - value
int w1_read(const char *id);

// adc_setup - A/D converter setup
// return - success flag
int adc_setup(void);

// adc_volt - read volt
// argument - ch: channel
// return - volt
int adc_volt(int ch);

// adc_raw - read raw value
// argument - ch: channel
// return - raw value
int adc_raw(int ch);

#endif
