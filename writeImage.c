//Victor Daniel Stella Paiva
//GRR20200234

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include "PPMImage.h"
#include "writeImage.h"

//Function to calculate de distance between the color averages of two images through the Red Mean formula
int colorDistance( double *imgAvg1, double *imgAvg2 ) {
  double distRed = imgAvg1[ 0 ] - imgAvg2[ 0 ];
  double distGreen = imgAvg1[ 1 ] - imgAvg2[ 1 ];
  double distBlue = imgAvg1[ 2 ] - imgAvg2[ 2 ];

  double redAvg = ( imgAvg1[ 0 ] + imgAvg2[ 0 ] ) / 2;

  double powRed = distRed * distRed;
  double powGreen = distGreen * distGreen;
  double powBlue = distBlue * distBlue;

  double redMean = sqrt( ( 2 + redAvg / 256 ) * powRed + ( 4 * powGreen ) + ( 2 + (255 - redAvg ) / 256 ) * powBlue );

  return redMean;
}

//Function to calculate the color average of an image
double *imgSquareAvg( PPMImgInfo *img, int startY, int endY, int startX, int endX ) {

  double *avg = calloc( 3, sizeof( double ) );
  
  int count = 0;
  for( int i = startY; i < endY; i++ ) {
    for( int j = startX; j < endX; j++ ) {
      avg[ 0 ] += pow( img->raster[ i ][ j ].red, 2 );
      avg[ 1 ] += pow( img->raster[ i ][ j ].green, 2 );
      avg[ 2 ] += pow( img->raster[ i ][ j ].blue, 2 );
      count++;
    }
  }

  for( int i = 0; i < 3; i++ ) {
    avg[ i ] = sqrt( avg[ i ] / count );
  }
  return avg;
}

//Swicth each pixel value of a image region for the corresponding value of the tile
int switchContent( PPMImgInfo *image, int startY, int endY, int startX, int endX, PPMImgInfo *tile ) {
  for( int i = startY; i < endY; i++ ) {
    for( int j = startX; j < endX; j++ ) {
      image->raster[ i ][ j ] = tile->raster[ i - startY ][ j - startX ];
    }
  }
  
  return 0;
}

//Function to write the output image
int writeImg( char *outImgName, PPMImgInfo *origImg ) {

  FILE *outImgFile;

  if( strcmp( outImgName, "stdout" ) != 0 ) outImgFile = fopen( outImgName, "w" );
  else outImgFile = stdout;

  //Write outImg image header
  fprintf(
    outImgFile,
    "%s\n%d %d\n%d\n",
    origImg->imgType,
    origImg->x,
    origImg->y,
    origImg->maxColorValue
  );

  //Write the raster into the P3 output image
  if( strcmp( origImg->imgType, "P3" ) == 0 ) {
    for( int i = 0; i < origImg->y; i++ ) {
      for( int j = 0; j < origImg->x; j++ ) {
        fprintf(
          outImgFile,
          "%d %d %d ",
          origImg->raster[ i ][ j ].red,
          origImg->raster[ i ][ j ].green,
          origImg->raster[ i ][ j ].blue
        );
      }
      fprintf( outImgFile, "\n" );
    }
  }
  
  //Write the raster into the P6 output image
  else if( strcmp( origImg->imgType, "P6" ) == 0 ) {
    for( int i = 0; i < origImg->y; i++ ) {
      for( int j = 0; j < origImg->x; j++ ) {
        fprintf(
          outImgFile,
          "%c%c%c",
          origImg->raster[ i ][ j ].red,
          origImg->raster[ i ][ j ].green,
          origImg->raster[ i ][ j ].blue
        );
      }
    }
  }
    
  //Close output image
  if( outImgFile != stdout ) fclose( outImgFile );

  return 0;
}