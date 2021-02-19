//Victor Daniel Stella Paiva
//GRR20200234

#ifndef PPMIMAGE
#define PPMIMAGE
 
//Struct of image pixels
typedef struct {
  unsigned int red, green, blue;
} ImgPixel;

//Struct of all needed information about a image
typedef struct {
  int x, y;
  char imgType[ 3 ];
  unsigned int maxColorValue;
  ImgPixel **raster;
  FILE *imgSource;
} PPMImgInfo;

//Function to read the file header
int readHeader( PPMImgInfo *image );

//Function to read a image and return its information
PPMImgInfo *readImg( char *inputImgName );

//Function to read tiles directory
PPMImgInfo **readTiles( char *tilesDir );

//Function to free a image
int freeImg( PPMImgInfo *image );
 
#endif