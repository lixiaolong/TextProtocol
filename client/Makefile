# may need change
TARGET = text_protocol_client
CC = gcc
# CFLAGS += -g -D__HAVE_SOME_DEFINE
CFLAGS += -g
LDFLAGS += -I./ -L./

# for c
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c = .o)

###################
$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) $(OBJS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o
.PHONY:clean
