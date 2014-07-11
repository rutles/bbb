// remocon.h - libremocon header file

#ifndef _REMOCON_H
#define _REMOCON_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <fcntl.h>
#include <poll.h>
#include <time.h>
#include <getopt.h>

// libremocon constants
#define RC_DETECT 2000 // minimum time in micro second regarded as leader
#define RC_TIMEOUT 10 // timeout in miri second
#define RC_CONTINUE 100 // the time of wait for next signal in miri second
#define RC_FORMAT 6500 // leader on time in micro second regarded as AEHA
#define RC_SIGLEN 512 // maximum signal length in bits
#define RC_SIGCNT (RC_SIGLEN / 8) //maximum signal count in bytes

#define RC_PIN 30 //default PL-IRM2161-XD1 connected pin GPIO number
#define RC_POUT "P9_14" //default IrLED connected pin physical name

// time between o->n in usec
#define RC_LAP(n, o) \
  ((n.tv_sec - o.tv_sec) * 1000000 + \
  (n.tv_nsec - o.tv_nsec) / 1000)

#define RC_NSLEEP(o, u) \
  o.tv_nsec += (u * 1000); \
  if(o.tv_nsec >= 1000000000){ \
    o.tv_nsec -= 1000000000; \
    o.tv_sec += 1; \
  } \
  clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &o, NULL);

typedef struct {
  int lon, loff;// 1st leader on / off
  int t;// mark
  int h;// space high
  int l;// space low
  int repeat;// 2nd leader off
  int length;// data length of 1st shot in bit
  int *son, *soff;// array time of bit stream
  int multi;// multi shot flag
  int gap;// from top of 1st to top of 2nd time
  int interval;// from tail of 1st to top of 2nd time
  int count1, count2;// signal count in bytes
  uint8_t *code1, *code2;// array byte code
} irform_t;

typedef struct {
  int r;// PL-IRM2161-XD1 connected pin GPIO number
  char *t;// IrLED connected pin physical name
  char *d;// *.conf search path
} option_t;

// rc_analyze - analyze IR format
// argument - fd: file descriptor, ir: see below
// set pointers in ir.son, ir.soff, ir.code1, and ir.code2
// if ir.son == NULL || ir.soff == NULL, decode only
// return - 0: success, -1: error, ir: filled IR data except t, h, l
int rc_analyze(int fd, irform_t *ir);

// rc_center - called by rc_statist.
int rc_center(int *array, int count);

// rc_statist - set ir.t, ir.h, ir.l by statistical processing
// argument - ir: set parameters by rc_analyzer beforehand
// return - ir: filled t, h, l
void rc_statist(irform_t *ir);

// rc_rassign - setup GPIO as input
// argument - gpio: GPIO number
// return - 0: success, -1: error
int rc_rassign(int gpio);

// rc_ropen - open GPIO for read
// argument - gpio: GPIO number
// return - file descriptor
int rc_ropen(int gpio);

// rc_rfree - unexport GPIO
// argument - gpio: GPIO number
void rc_rfree(int gpio);

// rc_tassign - setup PWM
// argument - pname: phisycal pin name
// return - 0: success, -1: error
int rc_tassign(char* pname);

// rc_transmit - IR transmit
// argument - fd: file descriptor, ir: IR data
void rc_transmit(int fd, irform_t *ir);

// rc_topen - open run for PWM run/stop
// argument - pname: phisycal pin name
// return - file descriptor
int rc_topen(char* pname);

// rc_report - print analyzed IR format
// argument - ir: see below
// call rc_analyzer and rc_statist beforehand
void rc_report(irform_t *ir);

// rc_memo - save a button data into temporary file
// argument - serial: serial number of data, ir: see below
// call rc_analyzer beforehand
// return - 0: success, -1: error
int rc_memo(int serial, irform_t *ir);

// rc_save - save buttons data into analyzer.conf
// argument - serial: buttons count, dir: directory, ir: see below
// call rc_analyzer, rc_statist, and rc_memo beforehand
// return - 0: success, -1: error
int rc_save(int serial, const char *dir, irform_t *ir);

// rc_getopt - get command line option
// argument - argc, argv: arguments, ir: for return
// return - ir: filled options or default value
void rc_getopt(int argc, char *argv[], option_t *op);

// rc_gettoken - get token from file
// argument - file: file
// return - token, seek fp to next token 
char *rc_gettoken(FILE *file);

// rc_getbuttons - get button names in file and it's count
// argument - file: fp of analyzer.conf, pbutton: button names array
// return - count of buttons
int rc_getbuttons(FILE *file, char ***pbutton);

// rc_getdata - get Ir signal data
// argument - file: fp of analyzer.conf, button: button name, ir: Ir data
// return - 0: success, -1: error, ir filled
int rc_getdata(FILE *file, const char *button, irform_t *ir);

// rc_speccmp - compare Ir signal data
// argument - ir1: Ir data1, ir2: Ir data2
// return - 0: equal, -1: not equal
int rc_speccmp(irform_t *ir1, irform_t *ir2);

// rc_getcommand - get command from file by button name
// argument - file: fp of received.conf, button: button name
// return - 0: equal, -1: not equal
char *rc_getcommand(FILE *file, const char *button);

#endif
