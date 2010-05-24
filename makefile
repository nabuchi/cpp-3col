TARGET = col1
OBJS = mt.o col1.o

all : $(OBJS)
	g++ -o $(TARGET) $(OBJS)

clean :
	rm *.o

.c.o :
	g++ -c $<

.cpp.o :
	g++ -c $<
