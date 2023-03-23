#include <math.h>
#include <stdarg.h>

#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			// get rgb values from image[i][j]
			BYTE blue_hex, green_hex, red_hex;
			blue_hex = image[i][j].rgbtBlue;
			green_hex = image[i][j].rgbtGreen;
			red_hex = image[i][j].rgbtRed;

			// calculate gray shade using the average method
			BYTE gray_hex = round( (blue_hex + green_hex + red_hex) / 3.0 );

			// replace rgb with gray
			image[i][j].rgbtBlue = gray_hex;
			image[i][j].rgbtGreen = gray_hex;
			image[i][j].rgbtRed = gray_hex;
		}
	}
	return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			// get rgb values from image[i][j]
			BYTE blue_hex, green_hex, red_hex;
			blue_hex = image[i][j].rgbtBlue;
			green_hex = image[i][j].rgbtGreen;
			red_hex = image[i][j].rgbtRed;

			// calculate sepia values using the harvard formula;
			red_hex = round( (float)(red_hex * 0.393) + (float)(green_hex * 0.769) + (float)(blue_hex * 0.189) );
			green_hex = round( (float)(red_hex * 0.349) + (float)(green_hex * 0.686) + (float)(blue_hex * 0.168) );
			blue_hex = round( (float)(red_hex * 0.272) + (float)(green_hex * 0.534) + (float)(blue_hex * 0.131) );

			// replace rgb with sepia
			image[i][j].rgbtBlue = blue_hex;
			image[i][j].rgbtGreen = green_hex;
			image[i][j].rgbtRed = red_hex;
		}
	}
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
	// create a "mirrored" copy of image (mirror effect)
	RGBTRIPLE mirrored_image[height][width];
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			mirrored_image[i][j] = image[i][width - 1 -j];
		}
	}

	// replace image[][] with mirrored_image[][]
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			image[i][j] = mirrored_image[i][j];
		}
	}
	return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			/*
				special cases: [0][j], [height - 1][j], [i][0], [i][width -1]
			*/
			if (i == 0)
			{
				if (j == 0)
				{
					image[i][j] = calc_blur_rgb(4, image[i][j], image[i][j + 1], image[i + 1][j], image[i + 1][j + 1]);
				}
				else if (j == width - 1)
				{
					image[i][j] = calc_blur_rgb(4, image[i][j], image[i][j - 1], image[i + 1][j - 1], image[i + 1][j]);
				}
				else
				{
					image[i][j] = calc_blur_rgb(6, image[i][j], image[i][j - 1], image[i][j + 1], image[i + 1][j - 1], image[i + 1][j], image[i + 1][j + 1]);
				}
			}
			else if (i == height - 1)
			{
				if (j == 0)
				{
					image[i][j] = calc_blur_rgb(4, image[i][j], image[i - 1][j], image[i - 1][j + 1], image[i][j + 1]);
				}
				else if (j == width - 1)
				{
					image[i][j] = calc_blur_rgb(4, image[i][j], image[i][j - 1], image[i - 1][j - 1], image[i - 1][j]);
				}
				else
				{
					image[i][j] = calc_blur_rgb(6, image[i][j], image[i][j - 1], image[i - 1][j - 1], image[i - 1][j], image[i - 1][j + 1], image[i][j + 1]);
				}
			}
			else if (j == 0)
			{
				image[i][j] = calc_blur_rgb(6, image[i][j], image[i - 1][j], image[i - 1][j + 1], image[i][j + 1], image[i + 1][j + 1], image[i + 1][j]);
			}
			else if (j == width - 1)
			{
				image[i][j] = calc_blur_rgb(6, image[i][j], image[i - 1][j], image[i - 1][j - 1], image[i][j - 1], image[i + 1][j - 1], image[i + 1][j]);
			}
			else
			{
				image[i][j] = calc_blur_rgb(9, image[i - 1][j - 1], image[i - 1][j], image[i - 1][j +1], image[i][j - 1], image[i][j], image[i][j + 1], image[i + 1][j - 1], image[i + 1][j], image[i + 1][j + 1]);

			}
		}
	}
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
	return;
}

// Calculate blur rgb values for a pixel based on N neighbours
RGBTRIPLE calc_blur_rgb(int count, ...)
{
	va_list ap;
	int j;
	RGBTRIPLE triple, blur_triple;
	float avg_red = 0.0, avg_green = 0.0, avg_blue = 0.0;

	va_start(ap, count);
	for (j = 0; j < count; j++)
	{
		triple = va_arg(ap, RGBTRIPLE);
		avg_red += triple.rgbtRed / (float)count;
		avg_green += triple.rgbtGreen / (float)count;
		avg_blue += triple.rgbtBlue / (float)count;
	}
	va_end(ap);

	blur_triple.rgbtRed = round(avg_red);
	blur_triple.rgbtGreen = round(avg_green);
	blur_triple.rgbtBlue = round(avg_blue);

	return blur_triple;
}
