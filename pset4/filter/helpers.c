#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{

    // printf("%p\n", image[0]); -> Prints address
    // printf("%i\n", image[1][0].rgbtBlue); -> Prints blue value; ARRAYS = POINTERS!


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float average = ((float) image[i][j].rgbtBlue + (float) image[i][j].rgbtRed + (float) image[i][j].rgbtGreen) / 3;

            image[i][j].rgbtBlue = image[i][j].rgbtRed = image[i][j].rgbtGreen = (int) round(average);
        }

    }

    /**
    for (int i = 0; i < height; i++)

    // Turn the first ( i'th ) row of pixels into grayscale: ; starting at 1 for no each problems
        for (int j = 0; i < width; i++)
        {
            // For loop to sum up all three x three rows:
            int h = -1;
            float sum = 0;

            for (int g = -1; g < 2; g++)
            {
                sum = (float) image[&(i+h)][j+g];

                if (g == 1 && h != 1)
                {
                    g = -1;
                    h++;
                }
            }

            image[i][j] = (int) round(sum / 9);


        }
      **/
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp;

        }

    }
    return;
}


// RGBTRIPLE test[2][2] = {  {10, 20, 30}, {40, 50, 60}, {70, 80, 90}, {110, 130, 140}, {120, 140, 150}, {130, 150, 160}, {200, 210, 220}, {220, 230, 240}, {240, 250, 255} };




// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{


    // Copy image to not reuse changed values and get a flawed image
    
    RGBTRIPLE copy[height][width]; 
    
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            copy[h][w] = image[h][w];
        }
    }
    
    // Iterate through all


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            int x = j - 1;
            int y = i - 1;
            float blue_avg = 0;
            float red_avg = 0;
            float green_avg = 0;
            int x_ticker = 0;
            int y_ticker = 0;
            int x_limit = 3;
            int y_limit = 3;
            float dividend = 0;
        


            // Now we are in the upper left corner except if out of bonds:

            // Exception cases:



            // Upper left corner:
            if (x < 0 && y < 0)
            {
                x_ticker++;
                y_ticker++;
            }
            // Upper right corner:
            else if (x + 2 >= width && y < 0)
            {
                y_ticker++;
                x_limit--;
            }
            // Lower left corner:
            else if (x < 0 && y + 2 >= height)
            {
                x_ticker++;
                y_limit--;
            }
            // Lower right corner:
            else if (x + 2 >= width && y + 2 >= height)
            {
                y_limit--;
                x_limit--;
            }
            // Upper Frame
            else if (y < 0)
            {
                y_ticker++;
            }
            // Left Frame
            else if (x < 0)
            {
                x_ticker++;
            }
            // Lower Frame
            else if (y + 2 >= height)
            {
                y_limit--;
            }
            // Right Frame
            else if (x + 2 >= width)
            {
                x_limit--;
            }

            // For normal case:

            for (int h = y_ticker; h < y_limit; h++)
            {
                for (int g = x_ticker; g < x_limit; g++)
                {
                    blue_avg = blue_avg + (float) copy[y + h][x + g].rgbtBlue;
                    red_avg = red_avg + (float) copy[y + h][x + g].rgbtRed;
                    green_avg = green_avg + (float) copy[y + h][x + g].rgbtGreen;
                    dividend++;
                }
            }

            image[i][j].rgbtBlue = (int) round(blue_avg / dividend);
            image[i][j].rgbtRed = (int) round(red_avg / dividend);
            image[i][j].rgbtGreen = (int) round(green_avg / dividend);
            
            // EVERYTHING IS RIGHT ??

        }
    }
    

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{

    // Copy image to not reuse changed values and get a flawed image
    
    RGBTRIPLE copy[height][width]; 
    
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            copy[h][w] = image[h][w];
        }
    }
    
    // Pixels beyond border as rgbtBlue etc = 00
    // Calculate both Gx & Gy for each channel for each pixel
    // Then combine Gx & Gy via Gx^2 + Gy^2 -> Cap at 255

    int Gx[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    int Gy[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float gx_red_sum = 0;
            float gx_blue_sum = 0;
            float gx_green_sum = 0;

            float gy_red_sum = 0;
            float gy_blue_sum = 0;
            float gy_green_sum = 0;

            float rgbtRed = 0;
            float rgbtBlue = 0;
            float rgbtGreen = 0;

            for (int y = 0; y < 3; y++)
            {
                for (int x = 0; x < 3; x++)
                {
                    // Exceptions (Frame):
                    if ((i + y - 1) < 0 || (j + x - 1) < 0 || (i + y - 1) > height - 1 || (j + x - 1) > width - 1)
                    {
                        rgbtRed = 0;
                        rgbtBlue = 0;
                        rgbtGreen = 0;
                    }

                    else
                    {
                        // Normal Case:
                        rgbtRed = copy[i + y - 1][j + x - 1].rgbtRed;
                        rgbtBlue = copy[i + y - 1][j + x - 1].rgbtBlue;
                        rgbtGreen = copy[i + y - 1][j + x - 1].rgbtGreen;
                    }

                    gx_red_sum = gx_red_sum + rgbtRed * Gx[y][x];
                    gx_blue_sum = gx_blue_sum + rgbtBlue * Gx[y][x];
                    gx_green_sum = gx_green_sum + rgbtGreen * Gx[y][x];
                    
                    gy_red_sum = gy_red_sum + rgbtRed * Gy[y][x];
                    gy_blue_sum = gy_blue_sum + rgbtBlue * Gy[y][x];
                    gy_green_sum = gy_green_sum + rgbtGreen * Gy[y][x];

                }
            }

            // Change pixel value according to Sobel Formula

            image[i][j].rgbtRed = round(fminf(sqrt(gx_red_sum * gx_red_sum + gy_red_sum * gy_red_sum), 255));
            image[i][j].rgbtBlue = round(fminf(sqrt(gx_blue_sum * gx_blue_sum + gy_blue_sum * gy_blue_sum), 255));
            image[i][j].rgbtGreen = round(fminf(sqrt(gx_green_sum * gx_green_sum + gy_green_sum * gy_green_sum), 255));




        }
    }




    return;
}
