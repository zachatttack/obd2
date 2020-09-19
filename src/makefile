CC=gcc
CFLAG= -Wall -g 

TARGET= main

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c -lpthread

clean:
	$(RM) $(TARGET)
