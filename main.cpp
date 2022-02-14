#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <algorithm> // std::copy

// structs
struct Rec
{
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
void PrintGrid(void);
bool topple(void);

int main(int argc, char **argv)
{

    // initialize globals
    grid1 = NULL;
    grid_width = 2000;
    grid_height = 2000;
    grid_center_x = grid_width / 2;
    grid_center_y = grid_height / 2;
    grid_size = grid_width * grid_height;

    uint64_t grains = (1 << 10) - 1;

    grid1 = new uint8_t[grid_size];
    if (grid1 == NULL)
    {
        std::cerr << "could not allocate memory for grid1." << std::endl;
        exit(1);
    }

    wrec.MinX = grid_width;
    wrec.MaxX = 0;
    wrec.MinY = grid_height;
    wrec.MaxY = 0;

    uint32_t cindex = grid_center_y * grid_width + grid_center_x;

    grid1[cindex] = 1;

    PrintStats();

    // scan and get bounding box
    for (uint16_t y = 0; y < grid_height; y++)
    {
        for (uint16_t x = 0; x < grid_height; x++)
        {
            uint32_t index = y * grid_width + x;
            if (grid1[index] != 0 && x < wrec.MinX)
            {
                wrec.MinX = x;
            }
            if (grid1[index] != 0 && y < wrec.MinY)
            {
                wrec.MinY = y;
            }
            if (grid1[index] != 0 && x > wrec.MaxX)
            {
                wrec.MaxX = x;
            }
            if (grid1[index] != 0 && y > wrec.MaxY)
            {
                wrec.MaxY = y;
            }
        }
    }

    // Give a little wiggle room for the working bounding box.
    wrec.MinX--;
    wrec.MinY--;
    wrec.MaxX++;
    wrec.MaxY++;

    PrintStats();

    uint32_t frame = 0; // frame : the number of times, topple() has been run. Why frame... because...
    uint8_t ty = 0;     // ty : used for the spinner.
    
    grid1[cindex]-=1;
    
    for (uint64_t outerloop = 0; outerloop < grains; outerloop += 128)
    {
        grid1[cindex] += 128;
        while (1)
        {
            bool t = topple();
            if (t)
            {
                break;
            }

            frame++;
            if (frame % 100 == 0)
            {
                switch (ty)
                {
                case 0:
                {
                    std::cout << "|";
                }
                case 1:
                {
                    std::cout << "/";
                }
                case 2:
                {
                    std::cout << "-";
                }
                case 3:
                {
                    std::cout << "\\";
                }
                case 4:
                {
                    std::cout << "|";
                }
                case 5:
                {
                    std::cout << "/";
                }
                case 6:
                {
                    std::cout << "-";
                }
                case 7:
                {
                    std::cout << "\\";
                }
                }
                std::cout << "\r";
                ty++;
                if (ty == 8)
                {
                    ty = 0;
                }
            }
        }
        
    }

    // make bounding box a little larger so there is padding for the image.
    wrec.MinX -= 10;
    wrec.MinY -= 10;
    wrec.MaxX += 10;
    wrec.MaxY += 10;

    if (wrec.MinX < 0)
    {
        wrec.MinX = 0;
    }
    if (wrec.MinY < 0)
    {
        wrec.MinY = 0;
    }
    if (wrec.MaxX > grid_width)
    {
        wrec.MaxX = grid_width;
    }
    if (wrec.MaxY > grid_height)
    {
        wrec.MaxY = grid_height;
    }

    PrintStats();

    // calculate the image width and height
    uint16_t iwidth = wrec.MaxX - wrec.MinX;  // iwidth : The image's width
    uint16_t iheight = wrec.MaxY - wrec.MinY; // iheight : The image's height

    PrintGrid();

    // clean up memory
    if (grid1 != NULL)
    {
        delete[] grid1;
    }
}

void PrintGrid(void)
{
    for (uint16_t y = wrec.MinY; y <= wrec.MaxY; y++)
    {
        for (uint16_t x = wrec.MinX; x <= wrec.MaxX; x++)
        {
            uint32_t tmpi = y * grid_width + x;
            std::cout << (int)grid1[tmpi] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void PrintStats(void)
{

    std::cout << "grid width:height " << grid_width << ":" << grid_height << std::endl;
    std::cout << "grid center width:height " << grid_center_x << ":" << grid_center_y << std::endl;
    std::cout << "grid size " << grid_size << std::endl;
    std::cout << "wrec " << wrec.MinX << ":" << wrec.MaxX << " " << wrec.MinY << ":" << wrec.MaxY << std::endl;
    std::cout << std::endl;
}

bool topple(void)
{

    bool bail;
    bail = true;

    uint8_t *grid2 = new uint8_t[grid_size];
    if (grid2 == NULL)
    {
        std::cerr << "Memory allocation error with grid2." << std::endl;
    }

    // copy cells that are less than 4
    for (uint16_t y = wrec.MinY; y <= wrec.MaxY; y++)
    {
        for (uint16_t x = wrec.MinX; x <= wrec.MaxX; x++)
        {
            uint32_t index = y * grid_height + x;
            uint8_t num;
            num = grid1[index];
            if (num < 4)
            {
                grid2[index] = num;
            }
        }
    }

    Rec w;
    w.MaxX = wrec.MaxX;
    w.MinX = wrec.MinX;
    w.MaxY = wrec.MaxY;
    w.MinY = wrec.MinY;

    if (wrec.MinX < 0)
    {
        wrec.MinX = 0;
    }
    if (wrec.MinY < 0)
    {
        wrec.MinY = 0;
    }
    if (wrec.MaxX > grid_width)
    {
        wrec.MaxX = grid_width;
    }
    if (wrec.MaxY > grid_height)
    {
        wrec.MaxY = grid_height;
    }

    for (uint16_t y = w.MinY; y <= w.MaxY; y++)
    {
        for (uint16_t x = w.MinX; x <= w.MaxX; x++)
        {
            uint32_t index = y * grid_height + x;
            uint8_t tmp;
            tmp = grid1[index];

            if (tmp >= 4)
            {
                grid2[index] += (tmp - 4); // Nasty gotha here. Add then subtract!
                if (grid2[index] >= 4)
                {
                    bail = false;
                }

                // west
                if (x - 1 >= 0)
                {
                    uint16_t tx = x - 1;
                    uint32_t tmpi = y * grid_width + tx;
                    grid2[tmpi]++;
                    if (tx < wrec.MinX)
                    {
                        wrec.MinX = tx; // modifiy the working bounding box because of a spill over.
                    }
                    if (grid2[tmpi] >= 4)
                    {
                        bail = false;
                    }
                }

                // north
                if (y - 1 >= 0)
                {
                    uint16_t ty = y - 1;
                    uint32_t tmpi = ty * grid_width + x;
                    grid2[tmpi]++;
                    if (ty < wrec.MinY)
                    {
                        wrec.MinY = ty; // modifiy the working bounding box because of a spill over.
                    }
                    if (grid2[tmpi] >= 4)
                    {
                        bail = false;
                    }
                }

                // east
                if (x + 1 <= grid_width - 1)
                {
                    uint16_t tx = x + 1;
                    uint32_t tmpi = y * grid_width + tx;
                    grid2[tmpi]++;
                    if (tx > wrec.MaxX)
                    {
                        wrec.MaxX = tx; // modifiy the working bounding box because of a spill over.
                    }
                    if (grid2[tmpi] >= 4)
                    {
                        bail = false;
                    }
                }

                // south
                if (y + 1 <= grid_height - 1)
                {
                    uint16_t ty = y + 1;
                    uint32_t tmpi = ty * grid_width + x;
                    grid2[tmpi]++;
                    if (ty > wrec.MaxY)
                    {
                        wrec.MaxY = ty; // modifiy the working bounding box because of a spill over.
                    }
                    if (grid2[tmpi] >= 4)
                    {
                        bail = false;
                    }
                }
            }
        }
    }
    std::copy(grid2, grid2 + grid_size, grid1);
    return bail;
}