BeagleBone Black handling examples

bbb/README.md: this file<br>
bbb/ain<br>
bbb/ain/ain.c: read out AIN0..AIN7<br>
bbb/ain/joy.c: analog joystick reader<br>
bbb/ain/uv.c: UV sensor cape test program<br>
bbb/ain/uvmeterd.c: UV meter daemon for UV sensor cape<br>
bbb/cape<br>
bbb/cape/dtbo_handler.c: cape manager read out or add dtob file<br>
bbb/cape/paths.c: display uncertain paths<br>
bbb/gpio<br>
bbb/gpio/buzzer.c: PIEZO buzzer alarm sounds rings<br>
bbb/gpio/led.c: LED blink<br>
bbb/gpio/pird.c: PYRO alarm daemon<br>
bbb/gpio/switch.c: tactile switch read example<br>
bbb/i2c<br>
bbb/i2c/aqm0802a.c: AQM0802A display demo. i2clcd library required<br>
bbb/i2c/i2c7segtest.c: Arduino on BBB cape & 7seg.board handling example<br>
bbb/i2c/i2c_7seg: Arduino sketch folder for 7seg.board<br>
bbb/i2c/i2c_7seg/i2c_7seg.ino<br>
bbb/i2c/i2clx.c: LPC1114 on bbb & S9705 board illuminometer<br> 
bbb/i2c/libi2clcd: i2clcd library<br>
bbb/i2c/libi2clcd/Makefile<br>
bbb/i2c/libi2clcd/i2clcd.h<br>
bbb/i2c/libi2clcd/lcd_command.c<br>
bbb/i2c/libi2clcd/lcd_createChar.c<br>
bbb/i2c/libi2clcd/lcd_open.c<br>
bbb/i2c/libi2clcd/lcd_printf.c<br>
bbb/i2c/libi2clcd/lcd_putch.c<br>
bbb/i2c/libi2clcd/lcd_setCursor.c<br>
bbb/i2c/libi2clcd/lcd_setup.c<br>
bbb/i2c/rtc.sh: RTC cape setup shell script<br>
bbb/libbbb: bbb library<br>
bbb/libbbb/Makefile<br>
bbb/libbbb/adc.c<br>
bbb/libbbb/bbb.h<br>
bbb/libbbb/cape.c<br>
bbb/libbbb/gpio.c<br>
bbb/libbbb/port.c<br>
bbb/libbbb/pwm.c<br>
bbb/libbbb/w1.c<br>
bbb/pwm<br>
bbb/pwm/colors.c: Color LED cape display demo<br>
bbb/pwm/debian<br>
bbb/pwm/debian/fl_servo.cpp: servo motor trimmer. FLTK1.3 required<br>
bbb/pwm/measure.c: Ultrasonic measure cape measure<br>
bbb/pwm/measure_lcdd.c: Ultrasonic measure cape & AQM0802A measure daemon<br>
bbb/pwm/measure_proto.c: ultrasonic measure prototype<br>
bbb/pwm/servo.c: spin servo motor -30..30 degrees<br>
bbb/remocon<br>
bbb/remocon/analyzer.c: IR remote controller signal analyser<br>
bbb/remocon/analyzer.conf: analized signal file(sample)<br>
bbb/remocon/button00.sh: USER LED3 blink shell script<br>
bbb/remocon/button01.sh: USER LED3 blink stop shell script<br>
bbb/remocon/debian<br>
bbb/remocon/debian/g_analyzer.c: IR remote controller signal analyser. GTK3 required<br>
bbb/remocon/libremocon: IR remote controller library<br>
bbb/remocon/libremocon/Makefile<br>
bbb/remocon/libremocon/rc_analyze.c<br>
bbb/remocon/libremocon/rc_getbuttons.c<br>
bbb/remocon/libremocon/rc_getcommand.c<br>
bbb/remocon/libremocon/rc_getdata.c<br>
bbb/remocon/libremocon/rc_getopt.c<br>
bbb/remocon/libremocon/rc_gettoken.c<br>
bbb/remocon/libremocon/rc_memo.c<br>
bbb/remocon/libremocon/rc_rassign.c<br>
bbb/remocon/libremocon/rc_receive.c<br>
bbb/remocon/libremocon/rc_record.c<br>
bbb/remocon/libremocon/rc_report.c<br>
bbb/remocon/libremocon/rc_save.c<br>
bbb/remocon/libremocon/rc_speccmp.c<br>
bbb/remocon/libremocon/rc_statist.c<br>
bbb/remocon/libremocon/rc_tassign.c<br>
bbb/remocon/libremocon/rc_transmit.c<br>
bbb/remocon/libremocon/remocon.h<br>
bbb/remocon/received.c: BeagleBone Black remote control daemon<br>
bbb/remocon/received.conf: BeagleBone Black remote control command file<br>
bbb/remocon/shot.c: IR signal transmit program<br>
bbb/spi<br>
bbb/spi/aqm1248a.c: AQM1248A fill draw gray test program<br>
bbb/spi/beat.c: Heart beat cape beat counter<br>
bbb/spi/gl_beat<br>
bbb/spi/gl_beat/bigfont.h: Numeric big font<br>
bbb/spi/gl_beat/font.h: font<br>
bbb/spi/gl_beat/fonts.c: AQM1248A display fonts demo<br>
bbb/spi/gl_beat/gl_beat.c: Heart beat cape & AQM1248A health meter<br>
bbb/spi/gl_beat/glcd.c: AQM1248A control functions object<br>
bbb/spi/gl_beat/glcd.h: AQM1248A control functions prototype<br>
bbb/spi/gl_beat/sin.c: draw sin wave to AQM1248A<br>
bbb/uart<br>
bbb/uart/fc1114.bin: LPC1114 on BBB frequncy counter firmware<br>
bbb/w1<br>
bbb/w1/BB-W1-00A0.dtbo: 1-Wire setup dtob file<br>
bbb/w1/BB-W1-00A0.dts: 1-Wire setup dtob file source<br>
bbb/w1/ds18b20.c: DS18B20+ thermometer
