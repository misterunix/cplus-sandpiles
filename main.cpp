#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// structs
struct Rec {
    uint16_t MinX;
    uint16_t MaxX;
    uint16_t MinY;
    uint16_t MaxY;
} wrec;

// globals
uint16_t grid_width;
uint16_t grid_height;
uint32_t grid_size;
uint16_t grid_center_x;
uint16_t grid_center_y;

uint8_t *grid1;


void PrintStats(void);


int main( int argc, char **argv ) {

// initialize globals
grid1 = NULL;
grid_width = 2000;
grid_height = 2000;
grid_center_x = grid_width / 2;
grid_center_y = grid_height / 2;
grid_size = grid_width * grid_height;



grid1 = new uint8_t[grid_size];
if( grid1 == NULL) {
    std::cerr << "could not allocate memory for grid1." << std::endl;
    exit(1);
}

wrec.MinX = 0;
wrec.MaxX = grid_width;
wrec.MinY = 0;
wrec.MaxY = grid_height;


PrintStats();


// clean up memory
if( grid1 != NULL) {
    delete []grid1;
}


}


void PrintStats(void) {

std::cout << "grid width:height " << grid_width << ":" << grid_height << std::endl;
std::cout << "grid center width:height " << grid_center_x << ":" << grid_center_y << std::endl;
std::cout << "grid size " << grid_size << std::endl;
std::cout << "wrec " << wrec.MinX << ":" << wrec.MaxX << " " << wrec.MinY << ":" << wrec.MaxY << std::endl;

}


bool topple(void) {

  bool bail;
  bail = true;

  uint8_t *grid2 = new uint8_t[grid_size];
  if( grid2 == NULL ) {
      std::cerr << "Memory allocation error with grid2." << std::endl;
  }

    // copy cells that are less than 4
    for( uint16_t y=wrec.MinY;y<=wrec.MaxY;y++ ) {
        for( uint16_t x=wrec.MinX;x<=wrec.MaxX;x++ ) {
            uint32_t index = y * grid_height + x;
            uint8_t num;
            num = grid1[index];
            if( num < 4 ) {
                grid2[index] = num;
            }
        }
    }

    Rec w;
    w.MaxX = wrec.MaxX;
    w.MinX = wrec.MinX;
    w.MaxY = wrec.MaxY;
    w.MinY = wrec.MinY;

    if( wrec.MinX < 0 ) { wrec.MinX = 0; }
    if( wrec.MinY < 0 ) { wrec.MinY = 0; }
    if( wrec.MaxX > grid_width ) { wrec.MaxX = grid_width; }
    if( wrec.MaxY > grid_height ) { wrec.MaxY = grid_height; }

    for( uint16_t y=w.MinY;y<=w.MaxY;y++ ) {
        for( uint16_t x=w.MinX;x<=w.MaxX;x++ ) {
            uint32_t index = y * grid_height + x;
            uint8_t tmp;
            tmp = grid1[index];

            if( tmp >= 4 ) {
               grid2[index] += (tmp - 4); // Nasty gotha here. Add then subtract!
                if( grid2[index] >= 4) { bail = false; }



            }

        }
    }
    return bail;
}