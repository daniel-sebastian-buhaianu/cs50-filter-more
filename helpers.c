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
			// calculate gray shade using the average method
			BYTE gray_hex = round( (image[i][j].rgbtBlue +
						image[i][j].rgbtGreen +
						image[i][j].rgbtRed) / 3.0 );

			// replace rgb with new gray shade
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
	// Border the image with black pixels
	int bi_height = height + 2;
	int bi_width = width + 2;
	RGBTRIPLE bordered_image[bi_height][bi_width];
	border_image(height, width, image, bordered_image);

	// Apply blur to image
	for (int i = 1; i < bi_height - 1; i++)
	{
		for (int j = 1; j < bi_width - 1; j++)
		{
			image[i - 1][j - 1] = calc_blur_rgb(i, j, bi_height, bi_width, bordered_image);
		}
	}
    return;
}

// Detect edges
void edges (int height, int width, RGBTRIPLE image[height][width])
{
	// Border the image with black pixels
	int bi_height = height + 2;
	int bi_width = width + 2;
	RGBTRIPLE bordered_image[bi_height][bi_width];
	border_image(height, width, image, bordered_image);

	// Detect edges for each pixel in image[][]
	for (int i = 1; i < bi_height - 1; i++)
	{
		for (int j = 1; j < bi_width - 1; j++)
		{
			image[i - 1][j - 1] = calc_edge(i, j, bi_height, bi_width, bordered_image);
		}
	}
	return;
}

// Border a 2-D image array with black pixels
void border_image(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE bordered_image[height + 2][width + 2])
{
	int bi_height = height + 2;
	int bi_width = width + 2;
	RGBTRIPLE black_pixel = { .rgbtRed = 0, .rgbtGreen = 0, .rgbtBlue = 0 };
	for (int i = 0; i < bi_height; i++)
	{
		for (int j = 0; j < bi_width; j++)
		{
			if (i == 0 || i == bi_height - 1 || j == 0 || j == bi_width - 1)
			{
				bordered_image[i][j] = black_pixel;
			}
			else
			{
				bordered_image[i][j] = image[i][j];
			}
		}
	}
	return;
}

// Calculate blur rgb values for pixel image[i][j]
RGBTRIPLE calc_blur_rgb(int index_i, int index_j, int height, int width, RGBTRIPLE image[height][width])
{
	RGBTRIPLE triple, blur_triple;
	float blur_red = 0.0, blur_green = 0.0, blur_blue = 0.0;
	for (int i = index_i - 1; i <= index_i + 1; i++)
	{
		for (int j = index_j - 1; j <= index_j + 1; j++)
		{
			triple = image[i][j];
			blur_red += triple.rgbtRed / 9.0;
			blur_green += triple.rgbtGreen / 9.0;
			blur_blue += triple.rgbtBlue / 9.0;
		}
	}
	blur_triple.rgbtRed = round(blur_red);
	blur_triple.rgbtGreen = round(blur_green);
	blur_triple.rgbtBlue = round(blur_blue);
	return blur_triple;
}

// Calculate edge rgb values for pixel image[i][j]
RGBTRIPLE calc_edge (int index_i, int index_j, int height, int width, RGBTRIPLE image[height][width])
{
	RGBTRIPLE triple, Gx_triple, Gy_triple, result_triple;
	short Gx[3][3] = {
		{ -1, 0, 1 },
		{ -2, 0, 2 },
		{ -1, 0, 1 }
	};
	short Gy[3][3] = {
		{ -1, -2, -1 },
		{ 0, 0, 0 },
		{ 1, 2, 1 }
	};
	int Gx_red = 0, Gx_green = 0, Gx_blue = 0;
	int Gy_red = 0, Gy_green = 0, Gy_blue = 0;
	float result_red = 0.0, result_green = 0.0, result_blue = 0.0;
	int i = index_i - 1, i_GxGy = 0;
	int j = index_j - 1, j_GxGy = 0;

	// Calculate rgb values for Gx and Gy
	while (i <= index_i + 1)
	{
		while (j <= index_j + 1)
		{
			triple = image[i][j];
			Gx_red += triple.rgbtRed * Gx[i_GxGy][j_GxGy];
			Gy_red += triple.rgbtRed * Gy[i_GxGy][j_GxGy];
			Gx_green += triple.rgbtGreen * Gx[i_GxGy][j_GxGy];
			Gy_green += triple.rgbtGreen * Gy[i_GxGy][j_GxGy];
			Gx_blue += triple.rgbtBlue * Gx[i_GxGy][j_GxGy];
			Gy_blue += triple.rgbtBlue * Gy[i_GxGy][j_GxGy];
			j++, j_GxGy++;
		}
		i++, i_GxGy++;
	}

	// Calculate the result triple rgb values
	// result = round( square_root(Gx * Gx + Gy * Gy) ) % 255 for each rgb
	result_red = (int)round( sqrt( Gx_red * Gx_red + Gy_red * Gy_red ) ) % 0xff;
	result_green = (int)round( sqrt( Gx_green * Gx_green + Gy_green * Gy_green ) ) % 0xff;
	result_blue = (int)round( sqrt( Gx_blue * Gx_blue + Gy_blue * Gy_blue ) ) % 0xff;

	// Assign rgb vals to result triple and return
	result_triple.rgbtRed = result_red;
	result_triple.rgbtGreen = result_green;
	result_triple.rgbtBlue = result_blue;
	return result_triple;
}
