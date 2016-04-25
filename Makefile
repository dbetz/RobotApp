# This won't work until simple libraries are distributed with PropGCC!

CFLAGS=-mcmm -Os -ffunction-sections
LDFLAGS=-mcmm -Wl,-gc-sections

OBJS=\
RobotApp.o \
ELClient.o \
ELClientHTTP.o \
ELClientResponse.o \
FP.o \
FdSerial.o \
FdSerial_driver.o \
robot.o \
stream.o

RobotApp.elf:	$(OBJS)
	propeller-elf-gcc $(LDFLAGS) -o $@ $(OBJS)

%.o:	%.cpp
	propeller-elf-g++ -c $(CFLAGS) -o $@ $<

%.o:	%.c
	propeller-elf-gcc -c $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o *.elf
