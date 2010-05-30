TARGET = col1
OBJS = mt.o col1.o

all : $(OBJS)
	g++ -o $(TARGET) $(OBJS)

clean :
	rm *.o

.c.o :
	g++ -O3 -c $<

.cpp.o :
	g++ -O3 -c $<
