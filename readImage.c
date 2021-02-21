//Victor Daniel Stella Paiva
//GRR20200234

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PPMImage.h"

int readHeader( PPMImgInfo *img ) {
  
  char auxHeader[ 7 ]; 
  int i = 0;
  char aux;
  
  //Read all 4 lines from the header that aren't comments
  while( i < 4 ) {
    fscanf( img->imgSource, "%s", auxHeader );
    auxHeader[ strcspn ( auxHeader, "\n" ) ] = '\0';

    if( auxHeader[ 0 ] == '#' ) {
      aux = fgetc( img->imgSource );
      while ( aux != '\n' && aux != '\r' ) {
        aux = fgetc( img->imgSource );
      }
    } else {
      //Saves the file type (P3 or P6)
      if( i == 0 ) strncpy( img->imgType, auxHeader, 3 );
      //Saves the image width
      else if( i == 1 ) img->x = atoi( auxHeader );
      //Saves the image height
      else if( i == 2 ) img->y = atoi( auxHeader );
      //Saves the pixel maximum value
      else if( i == 3 ) img->maxColorValue = atoi( auxHeader );
      else continue;

      i++;
    }
  }

  return 0;
}

//Function to read a image and return its information
PPMImgInfo *readImg( const char *inputImgName ) {

  //Allocate a struct to receive the information of the image and return it
  PPMImgInfo *img;
  img = malloc( sizeof( PPMImgInfo ) );
  
  if( !img ) {
    perror( "Error allocating image" );
    exit( 1 );
  }

  //Define a pointer to the image file and opens it
  if( strcmp( inputImgName, "stdin" ) != 0 ) img->imgSource = fopen( inputImgName, "r" );
  else img->imgSource = stdin;
  
  if( !img->imgSource ) {
    perror( "Error opening the image file" );
    exit( 1 );
  }
  
  //Call a function to read the image header
  readHeader( img );

  //Allocate memory for the struct containing the RGB data of the every pixel of the image
  img->raster = calloc( img->y, sizeof( ImgPixel * ) );
  if( !img->raster ) {
    perror( "Error allocating memory for the raster image" );
    exit( 1 );
  }

  //Allocate raster's first line and set line pointers
  img->raster[ 0 ] = calloc( img->x * img->y, sizeof( ImgPixel ) );
  if( !img->raster[ 0 ] ) {
    perror( "Error allocating line 0 of the raster image" );
    exit( 1 );
  }

  for( int i = 1; i < img->y; i++ ) {
    img->raster[ i ] = img->raster[ 0 ] + i * img->x;
  }

  //Read the information about every pixel of a P3 ppm image and fill the raster
  if( strcmp( img->imgType, "P3" ) == 0 ) {
    for( int i = 0; i < img->y; i++ ) {
      for( int j = 0; j < img->x; j++ ) {
        fscanf(
          img->imgSource,
          "%d %d %d",
          &( img->raster[ i ][ j ].red ),
          &( img->raster[ i ][ j ].green ),
          &( img->raster[ i ][ j ].blue )
        );
      }
    }
  }
  
  //Read the information about every pixel of a P6 ppm image and fill the raster
  else if( strcmp( img->imgType, "P6" ) == 0 ) {
    fgetc( img->imgSource );
    for( int i = 0; i < img->y; i++ ) {
      for (int j = 0; j < img->x; j++) {
        img->raster[ i ][ j ].red = fgetc( img->imgSource );
        img->raster[ i ][ j ].green = fgetc( img->imgSource );
        img->raster[ i ][ j ].blue = fgetc( img->imgSource );
      }
    }
  }

  //Close the image file, set its source to NULL and returns the allocated information
  if( img->imgSource != stdin ) fclose( img->imgSource );
  img->imgSource = NULL;

  return img;
}