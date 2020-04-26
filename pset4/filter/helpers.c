#include "helpers.h"
#include <string.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]);

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width]);

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]);

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]);

void swap(BYTE *a, BYTE *b);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float avg = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0f;
            int greyColor =  round(avg) + 0.5;
            image[i][j].rgbtBlue  = greyColor;
            image[i][j].rgbtGreen = greyColor;
            image[i][j].rgbtRed   = greyColor;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Sepia Algorithm
    // sepiaBlue  = .272 * originalRed + .534 * originalGreen + .131 * originalBlue
    // sepiaGreen = .349 * originalRed + .686 * originalGreen + .168 * originalBlue
    // sepiaRed   = .393 * originalRed + .769 * originalGreen + .189 * originalBlue

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            float blueSepia  = (0.272 * (float)image[i][j].rgbtRed) + (0.534 * (float)image[i][j].rgbtGreen) + (0.131 * (float)image[i][j].rgbtBlue);
            float greenSepia = (0.349 * (float)image[i][j].rgbtRed) + (0.686 * (float)image[i][j].rgbtGreen) + (0.168 * (float)image[i][j].rgbtBlue);
            float redSepia   = (0.393 * (float)image[i][j].rgbtRed) + (0.769 * (float)image[i][j].rgbtGreen) + (0.189 * (float)image[i][j].rgbtBlue);

            if (blueSepia > 255)
            {
                blueSepia = 255;
            }

            if (greenSepia > 255)
            {
                greenSepia = 255;
            }

            if (redSepia > 255)
            {
                redSepia = 255;
            }

            if (blueSepia < 0)
            {
                blueSepia = 0;
            }

            if (greenSepia < 0)
            {
                greenSepia = 0;
            }

            if (redSepia < 0)
            {
                redSepia = 0;
            }

            image[i][j].rgbtBlue  = round(blueSepia) + 0.5;
            image[i][j].rgbtGreen = round(greenSepia) + 0.5;
            image[i][j].rgbtRed   = round(redSepia) + 0.5;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //printf("%d\n",width);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            swap(&image[i][j].rgbtBlue, &image[i][width - j - 1].rgbtBlue);
            swap(&image[i][j].rgbtRed, &image[i][width - j - 1].rgbtRed);
            swap(&image[i][j].rgbtGreen, &image[i][width - j - 1].rgbtGreen);
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    float avgBlue = 0;
    float avgGreen = 0;
    float avgRed = 0;

    RGBTRIPLE copyImg[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copyImg[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // edges
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
            {
                // top left corner
                if (i == 0 && j == 0)
                {
                    avgBlue = (copyImg[i][j].rgbtBlue + copyImg[i][j + 1].rgbtBlue +
                               copyImg[i + 1][j].rgbtBlue + copyImg[i + 1][j + 1].rgbtBlue) / 4.0f;
                    avgGreen = (copyImg[i][j].rgbtGreen   + copyImg[i][j + 1].rgbtGreen +
                                copyImg[i + 1][j].rgbtGreen + copyImg[i + 1][j + 1].rgbtGreen) / 4.0f;
                    avgRed = (copyImg[i][j].rgbtRed   + copyImg[i][j + 1].rgbtRed +
                              copyImg[i + 1][j].rgbtRed + copyImg[i + 1][j + 1].rgbtRed) / 4.0f;
                }
                // top right corner
                else if (i == 0 && j == width - 1)
                {
                    avgBlue = (copyImg[i][j - 1].rgbtBlue   + copyImg[i][j].rgbtBlue +
                               copyImg[i + 1][j - 1].rgbtBlue + copyImg[i + 1][j].rgbtBlue) / 4.0f;
                    avgGreen = (copyImg[i][j - 1].rgbtGreen   + copyImg[i][j].rgbtGreen   +
                                copyImg[i + 1][j - 1].rgbtGreen  + copyImg[i + 1][j].rgbtGreen) / 4.0f;
                    avgRed = (copyImg[i][j - 1].rgbtRed    + copyImg[i][j].rgbtRed  +
                              copyImg[i + 1][j - 1].rgbtRed + copyImg[i + 1][j].rgbtRed) / 4.0f;
                }
                // bottom left corner
                else if (i == height - 1 && j == 0)
                {
                    avgBlue = (copyImg[i - 1][j].rgbtBlue + copyImg[i - 1][j + 1].rgbtBlue +
                               copyImg[i][j].rgbtBlue   + copyImg[i][j + 1].rgbtBlue) / 4.0f;
                    avgGreen = (copyImg[i - 1][j].rgbtGreen + copyImg[i - 1][j + 1].rgbtGreen +
                                copyImg[i][j].rgbtGreen   + copyImg[i][j + 1].rgbtGreen) / 4.0f;
                    avgRed = (copyImg[i - 1][j].rgbtRed + copyImg[i - 1][j + 1].rgbtRed +
                              copyImg[i][j].rgbtRed   + copyImg[i][j + 1].rgbtRed) / 4.0f;
                }
                // bottom right corner
                else if (i == height - 1 && j == width - 1)
                {
                    avgBlue = (copyImg[i - 1][j - 1].rgbtBlue + copyImg[i - 1][j].rgbtBlue +
                               copyImg[i][j - 1].rgbtBlue   + copyImg[i][j].rgbtBlue) / 4.0f;
                    avgGreen = (copyImg[i - 1][j - 1].rgbtGreen + copyImg[i - 1][j].rgbtGreen  +
                               copyImg[i][j - 1].rgbtGreen    + copyImg[i][j].rgbtGreen) / 4.0f;
                    avgRed = (copyImg[i - 1][j - 1].rgbtRed  + copyImg[i - 1][j].rgbtRed  +
                               copyImg[i][j - 1].rgbtRed   + copyImg[i][j].rgbtRed) / 4.0f;
                }
                // top edge
                else if (i == 0)
                {
                    avgBlue = (copyImg[i][j - 1].rgbtBlue   + copyImg[i][j].rgbtBlue   + copyImg[i][j + 1].rgbtBlue +
                               copyImg[i + 1][j - 1].rgbtBlue + copyImg[i + 1][j].rgbtBlue + copyImg[i + 1][j + 1].rgbtBlue) / 6.0f;
                    avgGreen = (copyImg[i][j - 1].rgbtGreen   + copyImg[i][j].rgbtGreen   + copyImg[i][j + 1].rgbtGreen +
                               copyImg[i + 1][j - 1].rgbtGreen  + copyImg[i + 1][j].rgbtGreen + copyImg[i + 1][j + 1].rgbtGreen) / 6.0f;
                    avgRed = (copyImg[i][j - 1].rgbtRed    + copyImg[i][j].rgbtRed   + copyImg[i][j + 1].rgbtRed +
                               copyImg[i + 1][j - 1].rgbtRed + copyImg[i + 1][j].rgbtRed + copyImg[i + 1][j + 1].rgbtRed) / 6.0f;
                }
                // bottom edge
                else if (i == height - 1)
                {
                    avgBlue = (copyImg[i - 1][j - 1].rgbtBlue + copyImg[i - 1][j].rgbtBlue + copyImg[i - 1][j + 1].rgbtBlue +
                               copyImg[i][j - 1].rgbtBlue   + copyImg[i][j].rgbtBlue   + copyImg[i][j + 1].rgbtBlue) / 6.0f;
                    avgGreen = (copyImg[i - 1][j - 1].rgbtGreen + copyImg[i - 1][j].rgbtGreen + copyImg[i - 1][j + 1].rgbtGreen +
                                copyImg[i][j - 1].rgbtGreen    + copyImg[i][j].rgbtGreen   + copyImg[i][j + 1].rgbtGreen) / 6.0f;
                    avgRed = (copyImg[i - 1][j - 1].rgbtRed  + copyImg[i - 1][j].rgbtRed + copyImg[i - 1][j + 1].rgbtRed +
                              copyImg[i][j - 1].rgbtRed   + copyImg[i][j].rgbtRed   + copyImg[i][j + 1].rgbtRed) / 6.0f;
                }
                // left edge
                else if (j == 0)
                {
                    avgBlue = (copyImg[i - 1][j].rgbtBlue + copyImg[i - 1][j + 1].rgbtBlue +
                               copyImg[i][j].rgbtBlue   + copyImg[i][j + 1].rgbtBlue +
                               copyImg[i + 1][j].rgbtBlue + copyImg[i + 1][j + 1].rgbtBlue) / 6.0f;
                    avgGreen = (copyImg[i - 1][j].rgbtGreen + copyImg[i - 1][j + 1].rgbtGreen +
                               copyImg[i][j].rgbtGreen    + copyImg[i][j + 1].rgbtGreen +
                               copyImg[i + 1][j].rgbtGreen  + copyImg[i + 1][j + 1].rgbtGreen) / 6.0f;
                    avgRed = (copyImg[i - 1][j].rgbtRed  + copyImg[i - 1][j + 1].rgbtRed +
                               copyImg[i][j].rgbtRed   + copyImg[i][j + 1].rgbtRed +
                               copyImg[i + 1][j].rgbtRed + copyImg[i + 1][j + 1].rgbtRed) / 6.0f;
                }
                // right edge
                else if (j == width - 1)
                {
                    avgBlue = (copyImg[i - 1][j - 1].rgbtBlue + copyImg[i - 1][j].rgbtBlue + copyImg[i][j - 1].rgbtBlue   + copyImg[i][j].rgbtBlue +
                               copyImg[i + 1][j - 1].rgbtBlue + copyImg[i + 1][j].rgbtBlue) / 6.0f;
                    avgGreen = (copyImg[i - 1][j - 1].rgbtGreen + copyImg[i - 1][j].rgbtGreen + copyImg[i][j - 1].rgbtGreen    + copyImg[i][j].rgbtGreen   +
                               copyImg[i + 1][j - 1].rgbtGreen  + copyImg[i + 1][j].rgbtGreen) / 6.0f;
                    avgRed = (copyImg[i - 1][j - 1].rgbtRed  + copyImg[i - 1][j].rgbtRed + copyImg[i][j - 1].rgbtRed + copyImg[i][j].rgbtRed +
                               copyImg[i + 1][j - 1].rgbtRed + copyImg[i + 1][j].rgbtRed) / 6.0f;
                }
            }
            else
            {
                // all non edge pixels
                avgBlue = (copyImg[i - 1][j - 1].rgbtBlue + copyImg[i - 1][j].rgbtBlue + copyImg[i - 1][j + 1].rgbtBlue +
                           copyImg[i][j - 1].rgbtBlue   + copyImg[i][j].rgbtBlue   + copyImg[i][j + 1].rgbtBlue +
                           copyImg[i + 1][j - 1].rgbtBlue + copyImg[i + 1][j].rgbtBlue + copyImg[i + 1][j + 1].rgbtBlue) / 9.0f;
                avgGreen = (copyImg[i - 1][j - 1].rgbtGreen + copyImg[i - 1][j].rgbtGreen + copyImg[i - 1][j + 1].rgbtGreen +
                            copyImg[i][j - 1].rgbtGreen    + copyImg[i][j].rgbtGreen   + copyImg[i][j + 1].rgbtGreen +
                            copyImg[i + 1][j - 1].rgbtGreen  + copyImg[i + 1][j].rgbtGreen + copyImg[i + 1][j + 1].rgbtGreen) / 9.0f;
                avgRed = (copyImg[i - 1][j - 1].rgbtRed  + copyImg[i - 1][j].rgbtRed + copyImg[i - 1][j + 1].rgbtRed +
                          copyImg[i][j - 1].rgbtRed   + copyImg[i][j].rgbtRed   + copyImg[i][j + 1].rgbtRed +
                          copyImg[i + 1][j - 1].rgbtRed + copyImg[i + 1][j].rgbtRed + copyImg[i + 1][j + 1].rgbtRed) / 9.0f;
            }

            // set colors to averages
            image[i][j].rgbtBlue  = round(avgBlue) + 0.5f;
            image[i][j].rgbtGreen = round(avgGreen) + 0.5f;
            image[i][j].rgbtRed   = round(avgRed) + 0.5f;
        }
    }

    return;
}

void swap(BYTE *a, BYTE *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

