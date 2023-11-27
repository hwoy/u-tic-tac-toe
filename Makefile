CC = gcc
AR = ar
CFLAGS = -O2 -std=c99 -pedantic -Wall
bin = u3t
bin_pvp = $(bin)_pvp
bin_eve = $(bin)_eve

.PHONY: all clean

all: $(bin) $(bin_pvp) $(bin_eve) libttt.a

$(bin): main.o ttt_engine.o ttt_io.o function.o opt.o glibcrng.o
	$(CC) -o $(bin) main.o ttt_engine.o ttt_io.o function.o opt.o glibcrng.o
	
$(bin_pvp): main_pvp.o ttt_engine.o ttt_io.o function.o opt.o glibcrng.o
	$(CC) -o $(bin_pvp) main_pvp.o ttt_engine.o ttt_io.o function.o opt.o glibcrng.o
	
$(bin_eve): main_eve.o ttt_engine.o ttt_io.o function.o opt.o glibcrng.o
	$(CC) -o $(bin_eve) main_eve.o ttt_engine.o ttt_io.o function.o opt.o glibcrng.o

main.o: main.c
	$(CC) -c -DBUILT_IN_VARS -o main.o $(CFLAGS) main.c
	
main_pvp.o: main.c
	$(CC) -c -DBUILT_IN_VARS -D_PVP_ -o main_pvp.o $(CFLAGS) main.c
	
main_eve.o: main_eve.c
	$(CC) -c -DBUILT_IN_VARS -o main_eve.o $(CFLAGS) main_eve.c

ttt_engine.o: ttt_engine.c


ttt_io.o: ttt_io.c


function.o: function.c


opt.o: opt.c


glibcrng.o: glibcrng.c


libttt.a: ttt_engine.o glibcrng.o
	$(AR) crv libttt.a ttt_engine.o glibcrng.o


clean:
	rm -rf *.o $(bin) $(bin_pvp) $(bin_eve) *~ *.log *.exe *.a

