CC          = gcc
CFLAGS      = -Wall -Wextra -O2 -pthread
TSANFLAGS   = -fsanitize=thread -g

SRCS        = $(wildcard src/*.c)
OBJS        = $(SRCS:.c=.o)
TARGET      = sync_car
TSAN_TARGET = sync_car_tsan

.PHONY: all tsan clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c src/sync_car.h
	$(CC) $(CFLAGS) -c -o $@ $<

tsan: $(TSAN_TARGET)

$(TSAN_TARGET): $(SRCS) src/sync_car.h
	$(CC) $(CFLAGS) $(TSANFLAGS) -o $@ $(SRCS)

clean:
	rm -f $(OBJS) $(TARGET) $(TSAN_TARGET)