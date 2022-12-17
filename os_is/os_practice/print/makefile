CC = gcc
TARGET = os_print_write
OBJS = os_print_write.o

all: $(TARGET)
$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS)
clean:
	rm -f $(OBJS) $(TARGET)
