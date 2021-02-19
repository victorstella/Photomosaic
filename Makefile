CFLAGS = -Wall
LDLIBS = -lm

objects = main.o readImage.o readTiles.o writeImage.o

all: mosaico

mosaico : $(objects)
	cc -o mosaico $(objects) $(LDLIBS)

main.o : main.c PPMImage.h writeImage.h
	cc -c main.c $(CFLAGS)

readImage.o : readImage.c PPMImage.h
	cc -c readImage.c $(CFLAGS)

readTiles.o : readTiles.c PPMImage.h
	cc -c readTiles.c $(CFLAGS)

writeImage.o : writeImage.c writeImage.h PPMImage.h
	cc -c writeImage.c $(CFLAGS)

insert.o : insert.c defs.h buffer.h
	cc -c insert.c $(CFLAGS)

clean :
	-rm -f $(objects)

purge : clean
	-rm -f mosaico