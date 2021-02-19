//Victor Daniel Stella Paiva
//GRR20200234

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "PPMImage.h"
#include "writeImage.h"

int main( int argc, char **argv ) {

  //Char pointers for the inputs
  char *origImage, *finalMosaic, *tilesDir;
  //Inputs control variables
  int iArgExist = 0, oArgExist = 0, pArgExist = 0;

  if( argc >= 8 || argc % 2 == 0 ) {
    perror( "Incorrect inputs" );
    exit( 1 );
  }

  //Vefify inputs
  for( int i = 0; i < argc; i++ ) {
    if( strcmp( argv[ i ], "-i" ) == 0 ) {
      origImage = malloc( ( strlen( argv[ i + 1 ] ) + 1 ) * sizeof( char ) );
      strcpy( origImage, argv[ i + 1 ] );
      iArgExist = 1;
    } else if( strcmp( argv[ i ], "-o" ) == 0 ) {
      finalMosaic = malloc( ( strlen( argv[ i + 1 ] ) + 1 ) * sizeof( char ) );
      strcpy( finalMosaic, argv[ i + 1 ] );
      oArgExist = 1;
    } else if( strcmp( argv[ i ], "-p" ) == 0 ) {
      tilesDir = malloc( ( strlen( argv[ i + 1 ] ) + 1 ) * sizeof( char ) );
      strcpy( tilesDir, argv[ i + 1 ] );
      pArgExist = 1;
    }
  }

  //Default values for inputs
  if( iArgExist == 0 ) {
    origImage = malloc( 6 * sizeof( char ) );
    strcpy( origImage, "stdin" );
  }
  if( oArgExist == 0 ) {
    finalMosaic = malloc( 7 * sizeof( char ) );
    strcpy( finalMosaic, "stdout" );
  }
  if( pArgExist == 0 ) {
    tilesDir = malloc( 6 * sizeof( char ) );
    strcpy( tilesDir, "tiles" );
  }
  
  //Read tiles directory and image
  PPMImgInfo *imageData;
  imageData = readImg( origImage );
  PPMImgInfo **tilesList;
  tilesList = readTiles( tilesDir );
  
  //Calculate square average of colors of each tile
  double **tileColorAvg = NULL;
  for( int i = 0; tilesList[ i ] != NULL; i++ ) {
    tileColorAvg = realloc( tileColorAvg, ( i + 1 ) * sizeof( double * ) );
    tileColorAvg[ i ] = imgSquareAvg( tilesList[ i ], 0, tilesList[ i ]->y, 0, tilesList[ i ]->x );
  }

  //Set standard tile
  PPMImgInfo *tilesSize = tilesList[ 0 ];
  
  //Set image dimensions to float
  double imageY = ( double ) imageData->y;
  double imageX = ( double ) imageData->x;

  //Number of tiles inside the image 
  double imgBlockY = ceil( imageY / tilesSize->y );
  double imgBlockX = ceil( imageX / tilesSize->x );

  //Difference between tile and region and its index    
  double leastMean;
  int leastMeanIndex;
  
  //Aux variables
  double *AvgOfImage;
  double currMean;

  //Change all regions of image for a similar tile 
  for( int i = 0; i < imgBlockY * tilesSize->y; i += tilesSize->y ) {
    for( int j = 0; j < imgBlockX * tilesSize->x; j += tilesSize->x ) {

      leastMean = imageData->maxColorValue;
      leastMeanIndex = 0;

      AvgOfImage = imgSquareAvg( imageData, i, i + tilesSize->y, j, j + tilesSize->x );

      //Find tile to current region
      for( int k = 0; tilesList[ k ] != NULL; k++ ) {
        currMean = colorDistance( AvgOfImage, tileColorAvg[ k ] );
        if( leastMean > currMean ) {
          leastMeanIndex = k;
          leastMean = currMean;
        }
      }

      //Change region for tile
      switchContent( imageData, i, i + tilesSize->y, j, j + tilesSize->x, tilesList[ leastMeanIndex ] );

      //Free allocated space
      free( AvgOfImage );
    }
  }

  //Free tile average array
  for( int i = 0; tilesList[ i ] != NULL ; i++ ) {
    free( tileColorAvg[ i ] );
  }
  free( tileColorAvg );
  
  //Write output image
  writeImg( finalMosaic, imageData );

  //Close input image
  freeImg( imageData );

  //Close tiles
  for( int i = 0; tilesList[ i ] != NULL; i++ ) {
    freeImg( tilesList[ i ] );
  }
  free( tilesList );

  free( origImage );
  free( finalMosaic );
  free( tilesDir );
    
  return 0;
}

//Function to free a image
int freeImg( PPMImgInfo *image ) {
  //Verify image source and close it
  if( image->imgSource != NULL && image->imgSource != stdin && image->imgSource != stdout ) {
    fclose( image->imgSource );
  }
  image->imgSource = NULL;

  //Free allocated raster
  free( image->raster[ 0 ] );
  free( image->raster );

  //Free image
  free( image );

  return 0;
}
