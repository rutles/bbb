PREFIX = /usr
TARGET = libi2clcd.a
HEADER = i2clcd.h
CC := cc

$(TARGET):lcd_open.o lcd_setup.o lcd_command.o lcd_putch.o lcd_printf.o \
lcd_createChar.o lcd_setCursor.o

lcd_open.o:lcd_open.c $(HEADER)
lcd_setup.o:lcd_setup.c $(HEADER)
lcd_command.o:lcd_command.c $(HEADER)
lcd_putch.o:lcd_putch.c $(HEADER)
lcd_printf.o:lcd_printf.c $(HEADER)
lcd_createChar.o:lcd_createChar.c $(HEADER)
lcd_setCursor.o:lcd_setCursor.c $(HEADER)

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
