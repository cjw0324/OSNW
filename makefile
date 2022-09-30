TARGET=hw3
OBJECTS=main.o id.o sum10.o
$(TARGET) : $(OBJECTS)
	gcc -o $(TARGET) $(OBJECTS)
main.o : main.c
	gcc -c main.c
id.o : id.c
	gcc -c id.c
sum10.o : sum10.c
	gcc -c sum10.c
