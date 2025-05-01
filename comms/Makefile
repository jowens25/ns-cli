CC = gcc
CFLAGS = -Wall -g
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET = driver

PLATFORM ?= linux

ifeq ($(PLATFORM),linux)
CFLAGS += -DLINUX_PLATFORM
else
CFLAGS += -DMCU_PLATFORM
endif

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean build_and_clean

clean:
	rm -f $(TARGET) $(OBJECTS)


