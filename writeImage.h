//Victor Daniel Stella Paiva
//GRR20200234

#ifndef WRITEIMAGE
#define WRITEIMAGE

//Function to calculate de distance between the color averages of two images through the Red Mean formula
int colorDistance( double *imgAvg1, double *imgAvg2 );

//Function to calculate the color average of an image
double *imgSquareAvg( PPMImgInfo *image, int startY, int endY, int startX, int endX );

//Swicth each pixel value of a image region for the corresponding value of the tile
int switchContent( PPMImgInfo *image, int startY, int endY, int startX, int endX, PPMImgInfo *tile );

//Function to write the output image
int writeImg( char *outImgName, PPMImgInfo *origImg );

#endif