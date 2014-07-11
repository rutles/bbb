PREFIX = /usr
TARGET = libbbb.a
HEADER = bbb.h
CC := cc

$(TARGET):cape.o gpio.o port.o pwm.o adc.o w1.o

cape.o:cape.c $(HEADER)
gpio.o:gpio.c $(HEADER)
port.o:port.c $(HEADER)
pwm.o:pwm.c $(HEADER)
adc.o:adc.c $(HEADER)
w1.o:w1.c $(HEADER)

.c.o:
	$(CC) -c -I. $<
	ar r $(TARGET) $@

install:$(TARGET)
	install -o root -g root -m 0644 $(HEADER) $(PREFIX)/include
	install -o root -g root -m 0644 $(TARGET) $(PREFIX)/lib

uninstall:
	rm -f $(PREFIX)/include/$(HEADER)
	rm -f $(PREFIX)/lib/$(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)
