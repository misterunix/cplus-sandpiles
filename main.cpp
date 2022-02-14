#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <algorithm> // std::copy
#include <string.h>

// globals
int bMinX;
int bMinY;
int bMaxX;
int bMaxY;

int grid_X;
int grid_Y;
int grid_size;

uint8_t *grid1;
uint8_t *grid2;

void PrintPPM()
{

    bMinX -= 10;
    bMinY -= 10;
    bMaxX += 10;
    bMaxY += 10;

    int width = bMaxX - bMinX;
    int height = bMaxY - bMinY;

    std::cerr << "  - " << bMinX << ":" << bMaxX << " " << bMinY << ":" << bMaxY << std::endl;
    std::cerr << "  - " << width << " " << height << std::endl;
    std::cout << "P6" << std::endl;
    std::cout << width << " " << height << " " << "255" << std::endl;

    for (int y = bMinY; y < bMaxY; y++)
    {
        for (int x = bMinX; x < bMaxX; x++)
        {
            int index = y * grid_X + x;
            uint8_t num = grid1[index];
            switch (num)
            {
            case 0:
                std::cout << uint8_t(0x4) << uint8_t(0x3a) << uint8_t(0x6f);
                break;
            case 1:
                std::cout << uint8_t(0xf) << uint8_t(0x72) << uint8_t(0x84);
                break;
            case 2:
                std::cout << uint8_t(0xaa) << uint8_t(0x88) << uint8_t(0x39);
                break;
            case 3:
                std::cout << uint8_t(0x9c) << uint8_t(0x00) << uint8_t(0x3c);
                break;
            default:
                std::cout << uint8_t(255) << uint8_t(255) << uint8_t(255);
            }
        }
        // std::cout << (int)grid1[index] << " ";
    }
    // std::cout << std::endl;
}
// std::cout << std::endl;


void PrintGrid()
{
    for (int y = bMinY; y <= bMaxY; y++)
    {
        for (int x = bMinX; x <= bMaxX; x++)
        {
            int index = y * grid_X + x;
            std::cout << (int)grid1[index] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool topple(void)
{
    bool bail = true;
    // anything less than 4 just get copied over
    for (int y = bMinY; y <= bMaxY; y++)
    {
        for (int x = bMinX; x <= bMaxX; x++)
        {
            int index = y * grid_X + x;
            uint8_t num = grid1[index];
            if (num < 4)
            {
                grid2[index] = num;
            }
            else
            {
                grid2[index] = 0;
            }
        }
    }

    int wMinX = bMinX;
    int wMaxX = bMaxX;
    int wMinY = bMinY;
    int wMaxY = bMaxY;

    for (int y = wMinY; y <= wMaxY; y++)
    {
        for (int x = wMinX; x <= wMaxX; x++)
        {
            int index = y * grid_X + x;
            uint8_t num = grid1[index];
            if (num < 4)
            {
                continue;
            }

            grid2[index] += (num - 4);
            bail = false;

            // north
            int ty = y - 1;
            if (ty < 0)
            {
                bMinY = 0;
            }
            else
            {
                if (ty < bMinY)
                {
                    bMinY = ty;
                }
                grid2[ty * grid_X + x] += 1;
            }

            // south
            ty = y + 1;
            if (ty >= grid_Y)
            {
                bMinY = grid_Y - 1;
            }
            else
            {
                if (ty > bMaxY)
                {
                    bMaxY = ty;
                }
                grid2[ty * grid_X + x] += 1;
            }

            // west
            int tx = x - 1;
            if (tx < 0)
            {
                bMinX = 0;
            }
            else
            {
                if (tx < bMinX)
                {
                    bMinX = tx;
                }
                grid2[y * grid_X + tx] += 1;
            }

            // east
            tx = x + 1;
            if (tx >= grid_X)
            {
                bMaxX = grid_X - 1;
            }
            else
            {
                if (tx > bMaxX)
                {
                    bMaxX = tx;
                }
                grid2[y * grid_X + tx] += 1;
            }
        }
    }
    for (int i = 0; i < grid_size; i++)
    {
        grid1[i] = grid2[i];
    }
    // memcpy(grid1,grid2,sizeof(uint8_t));
    return bail;
}

int main(int argc, char **argv)
{

    uint64_t grains;
    time_t start;
    time_t end;

    int shift = atoi( argv[1] );
    grains = 1 << shift;

    grid_X = 1000;
    grid_Y = 1000;
    grid_size = grid_X * grid_Y;

    grid1 = new uint8_t[grid_size];
    grid2 = new uint8_t[grid_size];

    bMinX = grid_X;
    bMaxX = 0;
    bMinY = grid_Y;
    bMaxY = 0;

    int ip1x = grid_X / 2;
    int ip1y = grid_Y / 2;
    int pos1 = ip1y * grid_X + ip1x;

    grid1[pos1] = 128;

    start = time(NULL);

    for (int y = 0; y < grid_Y; y++)
    {
        for (int x = 0; x < grid_X; x++)
        {
            int index = y * grid_X + x;
            
            if (grid1[index] != 0)
            {
                if (x < bMinX)
                {
                    bMinX = x;
                }
                if (x > bMaxX)
                {
                    bMaxX = x;
                }
                if (y < bMinY)
                {
                    bMinY = y;
                }
                if (y > bMaxY)
                {
                    bMaxY = y;
                }
            }
            
        }
    }

    bMaxX++;
    bMaxY++;
    bMinX--;
    bMinY--;

    //std::cout << bMinX << ":" << bMaxX << " " << bMinY << ":" << bMaxY << std::endl;


    
    //for (uint64_t outerloop = 128; outerloop < grains; outerloop += 128)
    while(1)
    {

        while (1)
        {
            bool t;
            t = topple();
            if (t == true)
            {
                break;
            }
            // std::cout << bMinX << ":" << bMaxX << " " << bMinY << ":" << bMaxY << std::endl;
        }
        grains -= 128;
        if( grains <= 0 ) { break; }
        grid1[pos1] += 128;
/*      
        if (outerloop < grains)
        {
            grid1[pos1] += 128;
        }
*/
        // PrintGrid();
    }

    int st = 1 << shift;
    std::cerr << "- 2^" << shift << std::endl << "  - " << st << " grains placed" << std::endl;
    end = time(NULL);
    int v[3];
    v[0] = (end-start) % 60;
    float u = ((end-start) / 3600.0) * 60;
    v[1] =  (int)u % 60;
    v[2] = (end-start) / 3600;
    std::cerr << "  - Time " << v[2] << "h " << v[1] << "m " << v[0] << "s" << std::endl; 
    //PrintGrid();
    PrintPPM();

    if (grid2 != NULL)
    {
        delete[] grid2;
    }
    if (grid1 != NULL)
    {
        delete[] grid1;
    }

    return (0);
}