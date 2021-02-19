#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Height
    for (int h = 0; h < height; h++)
    {
        // Width
        for (int w = 0; w < width; w++)
        {
            // Getting average value of BGR of original image
            float r = image[h][w].rgbtRed;
            float g = image[h][w].rgbtGreen;
            float b = image[h][w].rgbtBlue;

            float gzb = (r + g + b)/3;

            int avg = round(gzb);

            // Turning the pixel into gay escale
            image[h][w].rgbtRed = avg;
            image[h][w].rgbtGreen = avg;
            image[h][w].rgbtBlue = avg;

        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    float middle = ((width - 1) / 2);

    // Height
    for (int h = 0; h < height; h++)
    {
        // Width
        for (int w = 0; w <= middle; w++)
        {
            // Swap pixel with it's opposite
            RGBTRIPLE temp = image[h][w];
            image[h][w] = image[h][width - w - 1];
            image[h][width - w - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    // Height
    for (int h = 0; h < height; h++)
    {
        // Width
        for (int w = 0; w < width; w++)
        {
            // Initialize values of each color
            float r = 0; float g = 0; float b = 0;

            // Int to know what number to divide with
            int d = 0;

            // check a 3x3 grid surrounding image[h][w]
            for (int ht = h - 1; ht <= h + 1; ht++) // Height
            {
                for (int wt = w - 1; wt <= w + 1; wt++) // Width
                {
                    // Make sure it only uses values from the image
                    if (ht >= 0 && ht < height && wt >= 0 && wt < width)
                    {
                        r += image[ht][wt].rgbtRed;
                        g += image[ht][wt].rgbtGreen;
                        b += image[ht][wt].rgbtBlue;
                        d++;
                    }
                }
            }

            // Average values of gzb
            float ar = r/d; temp[h][w].rgbtRed = round(ar);
            float ag = g/d; temp[h][w].rgbtGreen = round(ag);
            float ab = b/d; temp[h][w].rgbtBlue = round(ab);

        }
    }

    // Asign avg values to image[h][w]
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w].rgbtRed = temp[h][w].rgbtRed;
            image[h][w].rgbtGreen = temp[h][w].rgbtGreen;
            image[h][w].rgbtBlue = temp[h][w].rgbtBlue;
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    RGBTRIPLE temp[height][width];

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            float Gx_blue = 0;
            float Gx_green = 0;
            float Gx_red = 0;
            float Gy_blue = 0;
            float Gy_green = 0;
            float Gy_red = 0;
            for(int k = 1; k <= 3; k++)
            {
                for(int l = 1; l <= 3; l++)
                {
                    if(k >= 0 && k <= height - 1 && l >= 0 && l <= height - 1)
                    {
                    // Gx
                    Gx_blue += image[i + k][j + l].rgbtBlue * Gx[i + k][j + i];
                    Gx_green += image[i + k][j + l].rgbtGreen * Gx[i + k][j + l];
                    Gx_red += image[i + k][j + l].rgbtRed * Gx[i + k][j + l];

                    // Gy
                    Gy_blue += image[i + k][j + l].rgbtBlue * Gy[i + k][j + l];
                    Gy_green += image[i + k][j + l].rgbtGreen * Gy[i + k][j + l];
                    Gy_red += image[i +  k][l].rgbtRed * Gy[i + k][j + l];
                    }
                }
            }
            if (round(sqrt(pow(Gx_blue, 2) + pow(Gx_blue, 2)) > 255)) {temp[i][j].rgbtBlue = 255;}
            else temp[i][j].rgbtBlue = round(sqrt(pow(Gx_blue, 2) + pow(Gx_blue, 2)));

            if (round(sqrt(pow(Gx_green, 2) + pow(Gx_green, 2)) > 255)) {temp[i][j].rgbtGreen = 255;}
            else temp[i][j].rgbtGreen = round(sqrt(pow(Gx_green, 2) + pow(Gx_green, 2)));

            if (round(sqrt(pow(Gx_blue, 2) + pow(Gx_blue, 2)) > 255)) {temp[i][j].rgbtRed = 255;}
            else temp[i][j].rgbtRed = round(sqrt(pow(Gx_blue, 2) + pow(Gx_blue, 2)));
        }
    }
    for(int l = 0; l < height; l++)
    {
        for(int m = 0; m < width; m++)
        {
            image[l][m].rgbtBlue = temp[l][m].rgbtBlue;
            image[l][m].rgbtGreen = temp[l][m].rgbtGreen;
            image[l][m].rgbtRed = temp[l][m].rgbtRed;
        }
    }
    return;
}



