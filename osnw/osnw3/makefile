CC = gcc
TARGET=hw3
OBJECTS=main.o id.o sum10.o
all:$(TARGET)
$(TARGET) : $(OBJECTS)
	gcc -o $(@) $(OBJECTS)
clean:
	rm -f $(OBJECTS) $(TARGET)
