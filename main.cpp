#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

// globals
int bMinX;
int bMinY;
int bMaxX;
int bMaxY;

int grid_X;
int grid_Y;
int grid_size;

int shift;

uint8_t *grid1;

void PrintPPM()
{

    bMinX -= 10;
    bMinY -= 10;
    bMaxX += 10;
    bMaxY += 10;

    int width = bMaxX - bMinX;
    int height = bMaxY - bMinY;
    ;
    std::cerr << "- " << bMinX << ":" << bMaxX << " " << bMinY << ":" << bMaxY << std::endl;
    std::cerr << "- " << width << " " << height << std::endl;

    std::ofstream outfile;
    char filename[256];

    sprintf(filename, "images/%i-center.ppm", shift);
    outfile.open(filename);
    outfile << "P6" << std::endl;
    outfile << width << " " << height << " "
            << "255" << std::endl;

    for (int y = bMinY; y < bMaxY; y++)
    {
        for (int x = bMinX; x < bMaxX; x++)
        {
            int index = y * grid_X + x;
            uint8_t num = grid1[index];
            switch (num)
            {
            case 0:
                outfile << uint8_t(0x4) << uint8_t(0x3a) << uint8_t(0x6f);
                break;
            case 1:
                outfile << uint8_t(0xf) << uint8_t(0x72) << uint8_t(0x84);
                break;
            case 2:
                outfile << uint8_t(0xaa) << uint8_t(0x88) << uint8_t(0x39);
                break;
            case 3:
                outfile << uint8_t(0x9c) << uint8_t(0x00) << uint8_t(0x3c);
                break;
            default:
                outfile << uint8_t(255) << uint8_t(255) << uint8_t(255);
            }
        }
    }
    outfile.close();
}




void topple(void)
{

    bool bail;
    bail = false;
    while (!bail)
    {
        int wMinX = bMinX;
        int wMaxX = bMaxX;
        int wMinY = bMinY;
        int wMaxY = bMaxY;

        bail = true;

        for (int y = wMinY; y <= wMaxY; y++)
        {
            for (int x = wMinX; x <= wMaxX; x++)
            {
                int index = y * grid_X + x;
                if (grid1[index] >= 4)
                {
                    bail = false;

                    grid1[index] -= 4;

                    int tyn = y - 1;
                    if (tyn >= 0)
                    {
                        int t_index = tyn * grid_X + x;
                        grid1[t_index]++;
                        if (tyn < bMinY)
                        {
                            bMinY = tyn;
                        }
                    }
                    int tys = y + 1;
                    if (tys <= grid_Y - 1)
                    {
                        int t_index = tys * grid_X + x;
                        grid1[t_index]++;
                        if (tys > bMaxY)
                        {
                            bMaxY = tys;
                        }
                    }

                    int txw = x - 1;
                    if (txw >= 0)
                    {
                        int t_index = y * grid_X + txw;
                        grid1[t_index]++;
                        if (txw < bMinX)
                        {
                            bMinX = txw;
                        }
                    }
                    int txe = x + 1;
                    if (txe <= grid_X - 1)
                    {
                        int t_index = y * grid_X + txe;
                        grid1[t_index]++;
                        if (txe > bMaxX)
                        {
                            bMaxX = txe;
                        }
                    }
                }
            }
        }
    }
}



int main(int argc, char **argv)
{

    uint64_t grains;
    time_t start;
    time_t end;

    shift = atoi(argv[1]);
    grains = 1 << shift;

    grid_X = 4000;
    grid_Y = 4000;
    grid_size = grid_X * grid_Y;

    grid1 = new uint8_t[grid_size];


    for (int i = 0; i < grid_size; i++)
    {
        grid1[i] = 0;
    }

    bMinX = grid_X;
    bMaxX = 0;
    bMinY = grid_Y;
    bMaxY = 0;

    int ip1x = grid_X / 2;
    int ip1y = grid_Y / 2;
    int pos1 = ip1y * grid_X + ip1x;

    grid1[pos1] = 128;

    // find area of grid with starting grains
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

    start = time(NULL);

    int grains_put = 128;

    while (1)
    {
        grains -= grains_put;
        topple();

        if (grains > 0 && grains >= 128)
        {
            grains_put = 128;
            grid1[pos1] += grains_put;
        }
        else if (grains > 0 && grains < 128)
        {
            grains_put = grains;
            grid1[pos1] += grains_put;
        }
        else
        {
            break;
        }
    }

    int st = 1 << shift;
    std::cerr << "2^" << shift << std::endl
              << "- " << st << " grains placed" << std::endl;
    end = time(NULL);

   std::cerr << "- Time " << (end - start) << std::endl;
    
    PrintPPM();

    if (grid1 != NULL)
    {
        delete[] grid1;
    }

    return (0);
}