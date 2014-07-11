PREFIX = /usr
TARGET = libremocon.a
HEADER = remocon.h
CC := cc

$(TARGET):rc_analyze.o rc_statist.o rc_rassign.o rc_tassign.o \
rc_report.o rc_memo.o rc_save.o rc_getopt.o rc_transmit.o \
rc_gettoken.o rc_getbuttons.o rc_getdata.o rc_speccmp.o rc_getcommand.o

rc_analyze.o:rc_analyze.c $(HEADER)
rc_statist.o:rc_statist.c $(HEADER)
rc_rassign.o:rc_rassign.c $(HEADER)
rc_tassign.o:rc_tassign.c $(HEADER)
rc_transmit.o:rc_transmit.c $(HEADER)
rc_report.o:rc_report.c $(HEADER)
rc_memo.o:rc_memo.c $(HEADER)
rc_save.o:rc_save.c $(HEADER)
rc_getopt.o:rc_getopt.c $(HEADER)
rc_gettoken.o:rc_gettoken.c $(HEADER)
rc_getbuttons.o:rc_getbuttons.c $(HEADER)
rc_getdata.o:rc_getdata.c $(HEADER)
rc_speccmp.o:rc_speccmp.c $(HEADER)
rc_getcommand.o:rc_getcommand.c $(HEADER)

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
	rm -f *.a
