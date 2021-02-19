//Victor Daniel Stella Paiva
//GRR20200234

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "PPMImage.h"

//Function to read blocks of tiles, allocate blocks at once and fill them with information readed
PPMImgInfo **readTiles( char *tilesDir ) {

  DIR *dir = opendir( tilesDir );
  if( !dir ) {
    perror( "Failed to open directory" );
    exit( 1 );
  }
  struct dirent *dirEnt;

  //Array of tiles structs to be returned
  PPMImgInfo **tilesList = NULL;

  //Size of block of tiles to allocate each time (200 tiles) and number of blocks readed
  int blockSize = 300;
  int numOfBlocks = 0;

  //Current file path to call readImg on each tile 
  char filePath[ 1024 ];

  //Scrolls through files in directory, filling each block and restarting with 0
  int fileCount = blockSize;

  /*While the directory entry is valid and the array is smaller than 2000 positions,
  it keeps reallocating space and reading all the tiles*/
  dirEnt = readdir( dir );
  while ( dirEnt && blockSize * numOfBlocks + fileCount < 1800 ) {
    
    //Check the need to realloc (current block is full)
    if( fileCount == blockSize ) {
      fileCount = 0;
      numOfBlocks++;
      tilesList = realloc( tilesList, numOfBlocks * blockSize * sizeof( PPMImgInfo * ) );
    }
      
    //Check if the entry is a regular file and if contains '.ppm' in its name 
    if( dirEnt->d_type == DT_REG && strstr( dirEnt->d_name, ".ppm" ) != NULL ) {
      
      //Create the file path
      sprintf( filePath, "%s/%s", tilesDir, dirEnt->d_name );

      //Store the readed file in the array
      tilesList[ ( numOfBlocks - 1 ) * blockSize + fileCount ] = readImg( filePath );
      fileCount++;
    }

    //Go to next entry
    dirEnt = readdir( dir );
  }

  //Set the last position of the array as NULL, close the directory and returns the array
  tilesList[ ( numOfBlocks - 1 ) * blockSize + 1 ] = NULL;
  closedir( dir );

  return tilesList;
}