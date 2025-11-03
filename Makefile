FLAGS = -O2 -Wall -Wextra
CC = gcc

TARGET_NAME = example

SRC  = example.c ./ncui/ncui.c

OBJS = $(SRC:.c=.o)

all: $(TARGET_NAME)


%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@ 

$(TARGET_NAME): $(OBJS)
	$(CC) $(OBJS) -o $@ $(FLAGS) -lncurses

clean:
	rm $(OBJS) $(TARGET_NAME)

.PHONY: all clean

