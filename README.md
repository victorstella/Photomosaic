# Photomosaic
*by Victor Daniel Stella Paiva*


This is a plain C program that make a copy of an PPM image, but using several smaller images to compose the original by color proximity.
For the program to work correctly, you will need a directory with a large number of small images (eg. 20x20 pixels) and a single big image in the PPM format.

### Installing

With the cloned project, type the command:

##### $ make

You only need to do this the first time!

### Using

From now on, every time you want to make a photomosaic of a PPM image, all you have to do is run the command:

##### $ ./mosaico (arguments)

Where it is possible to pass 3 arguments. They are not mandatory, but I am sure you will need them all, since the program has default values for them that are not very useful most of the time.

##### -i NameOfTheImage.ppm

##### -o NameOfThePhotomosaic.ppm

##### -p NameOfTheTilesDirectory
