CC = gcc
CFLAGS = -O2 -std=c89 -pedantic -Wall # -D_DEVRAND_  #for better randomize
bin = u3t
bin_pvp = $(bin)_pvp
bin_eve = $(bin)_eve

.PHONY: all clean

all: $(bin) $(bin_pvp) $(bin_eve)

$(bin): main.o ttt_engine.o ttt_io.o function.o opt.o
	$(CC) -o $(bin) main.o ttt_engine.o ttt_io.o function.o opt.o
	
$(bin_pvp): main_pvp.o ttt_engine.o ttt_io.o function.o opt.o
	$(CC) -o $(bin_pvp) main_pvp.o ttt_engine.o ttt_io.o function.o opt.o
	
$(bin_eve): main_eve.o ttt_engine.o ttt_io.o function.o opt.o
	$(CC) -o $(bin_eve) main_eve.o ttt_engine.o ttt_io.o function.o opt.o

main.o: main.c
	$(CC) -c -DBUILT_IN_VARS -o main.o $(CFLAGS) main.c
	
main_pvp.o: main.c
	$(CC) -c -DBUILT_IN_VARS -D_PVP_ -o main_pvp.o $(CFLAGS) main.c
	
main_eve.o: main_eve.c
	$(CC) -c -DBUILT_IN_VARS -o main_eve.o $(CFLAGS) main_eve.c

ttt_engine.o: ttt_engine.c
	$(CC) -c -o ttt_engine.o $(CFLAGS) ttt_engine.c

ttt_io.o: ttt_io.c
	$(CC) -c -o ttt_io.o $(CFLAGS) ttt_io.c

function.o: function.c
	$(CC) -c -o function.o $(CFLAGS) function.c

opt.o: opt.c
	$(CC) -c -o opt.o $(CFLAGS) opt.c

clean:
	rm -rf *.o $(bin) $(bin_pvp) $(bin_eve) *~ *.log *.exe

