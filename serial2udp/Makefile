CC = gcc
CFLAGS = -Wall -O2 -std=gnu9x
LDFLAGS = 

SRC = serial2udp.c uart.c
OBJECTS = $(SRC:.c=.o)

REPLAY_SRC = replay_log.c
REPLAY_OBJECTS = $(REPLAY_SRC:.c=.o)

REPLAY_SPEED_SRC = replay_log_speed_synth.c
REPLAY_SPEED_OBJECTS = $(REPLAY_SPEED_SRC:.c=.o)


LOG2KML_SRC = log2kml.c
LOG2KML_OBJECTS = $(LOG2KML_SRC:.c=.o)



all: serial2udp replay_log

serial2udp: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o serial2udp

replay_log: $(REPLAY_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(REPLAY_OBJECTS) -o replay_log

replay_log_speed_synth: $(REPLAY_SPEED_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(REPLAY_SPEED_OBJECTS) -o replay_log_speed_synth -lm


log2kml: $(LOG2KML_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LOG2KML_OBJECTS) -o log2kml


clean:
	rm serial2udp replay_log

