#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate over each pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate the average of the red, green, and blue values
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            // Use floating-point division and round the result for better accuracy
            int average = round((red + green + blue) / 3.0);

            // Set all color channels to the average value
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate over each pixel in the image
    for (int i = 0; i < height; i++)
    {
        // Iterate over each column in the first half of the row
        for (int j = 0; j < width / 2; j++)
        {
            // Swap the pixel (i, j) with the pixel (i, width - j - 1)
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image using box blur
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a temporary image to store the new pixel values
    RGBTRIPLE temp[height][width];

    // Iterate over each pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumRed = 0, sumGreen = 0, sumBlue = 0;
            int count = 0;

            // Iterate over the 3x3 box centered at (i, j)
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    // Check if the neighboring pixel is within bounds
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        sumRed += image[ni][nj].rgbtRed;
                        sumGreen += image[ni][nj].rgbtGreen;
                        sumBlue += image[ni][nj].rgbtBlue;
                        count++;
                    }
                }
            }

            // Calculate the average color values
            temp[i][j].rgbtRed =
                (unsigned char) ((sumRed + count / 2) / count); // Round to nearest integer
            temp[i][j].rgbtGreen = (unsigned char) ((sumGreen + count / 2) / count);
            temp[i][j].rgbtBlue = (unsigned char) ((sumBlue + count / 2) / count);
        }
    }

    // Copy the blurred image back to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
}

// Function to clamp a value between 0 and 255
int clamp(int value)
{
    if (value < 0)
        return 0;
    if (value > 255)
        return 255;
    return value;
}

// Detect edges using the Sobel operator
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Define the Sobel kernels
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Create a temporary image to store the new pixel values
    RGBTRIPLE temp[height][width];

    // Iterate over each pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int Gx_red = 0, Gx_green = 0, Gx_blue = 0;
            int Gy_red = 0, Gy_green = 0, Gy_blue = 0;

            // Iterate over the 3x3 box centered at (i, j)
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    // Check if the neighboring pixel is within bounds
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        // Multiply by Gx kernel
                        Gx_red += image[ni][nj].rgbtRed * Gx[di + 1][dj + 1];
                        Gx_green += image[ni][nj].rgbtGreen * Gx[di + 1][dj + 1];
                        Gx_blue += image[ni][nj].rgbtBlue * Gx[di + 1][dj + 1];

                        // Multiply by Gy kernel
                        Gy_red += image[ni][nj].rgbtRed * Gy[di + 1][dj + 1];
                        Gy_green += image[ni][nj].rgbtGreen * Gy[di + 1][dj + 1];
                        Gy_blue += image[ni][nj].rgbtBlue * Gy[di + 1][dj + 1];
                    }
                    // If out of bounds, treat as black (0, 0, 0)
                    else
                    {
                        // No need to add anything since black pixels contribute 0
                    }
                }
            }

            // Calculate the final Sobel values using sqrt(Gx^2 + Gy^2)
            int final_red = round(sqrt(Gx_red * Gx_red + Gy_red * Gy_red));
            int final_green = round(sqrt(Gx_green * Gx_green + Gy_green * Gy_green));
            int final_blue = round(sqrt(Gx_blue * Gx_blue + Gy_blue * Gy_blue));

            // Clamp the values to the range [0, 255]
            temp[i][j].rgbtRed = clamp(final_red);
            temp[i][j].rgbtGreen = clamp(final_green);
            temp[i][j].rgbtBlue = clamp(final_blue);
        }
    }

    // Copy the edge-detected image back to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
}
