#Main settings
CC = g++
CFLAGS = -Wall -Wextra -std=c++11
SOURCES = main.cpp

#Settings for different OS
ifeq ($(OS), Windows_NT)
	RM = del
	TARGET = main.exe
else
	RM = rm
	TARGET = main
endif

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	echo $(OS)
	$(RM) $(TARGET)

# Run the programm
run: all
	./$(TARGET)