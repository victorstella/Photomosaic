//Victor Daniel Stella Paiva
//GRR20200234

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "PPMImage.h"

//Function to read the tiles directory and allocate all its tiles
PPMImgInfo **readTiles( char *tilesDir ) {

  DIR *dir = opendir( tilesDir );
  if( !dir ) {
    perror( "Failed to open directory" );
    exit( 1 );
  }
  struct dirent *dirEnt;
  
  /*Get the number of interesting files inside the directory (regular files that whose name ends with ".ppm"),
  and returns the buffer to the start of the folder*/
  int fileCount = 0;
  while( ( dirEnt = readdir( dir ) ) != NULL ) {
    if ( dirEnt->d_type == DT_REG && strstr( dirEnt->d_name, ".ppm" ) != NULL ) {
      fileCount++;
    }
  }
  rewinddir( dir );

  /*Allocate memory for the list of tiles (based on fileCount) that will receive information
  of each one of them and allocate their pixel data*/
  PPMImgInfo **tilesList = ( PPMImgInfo ** ) malloc( ( fileCount + 1 ) * sizeof( PPMImgInfo * ) );

  //Initialize the char pointer that will be reallocated to fit the size of every file path to be opened and readed
  char *filePath = NULL;

  /*Walks through the directory looking for files of interest, then calls the readImg function. When see a
  file that it is not interesting, the fileCount remains unchanged at the end of the current loop*/
  for( int i = 0; i < fileCount; i++ ) {
    dirEnt = readdir( dir );
    if ( dirEnt->d_type == DT_REG && strstr( dirEnt->d_name, ".ppm" ) != NULL ) {
    
      filePath = realloc( filePath, ( strlen( tilesDir ) + strlen( dirEnt->d_name ) + 2 ) * sizeof( char ) );
      sprintf( filePath, "%s/%s", tilesDir, dirEnt->d_name );
      tilesList[ i ] = readImg( filePath );
    
    } else i--;
  }

  //Set the last position of the tilesList to NULL
  tilesList[ fileCount ] = NULL;

  //Deallocate the filePath char pointer
  free( filePath );
  //Closes the tiles directory and returns the tilesList
  closedir( dir );
  return tilesList;
}