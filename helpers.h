#include "bmp.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]);

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]);

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width]);

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]);

// Calculate blur rgb values for pixel image[i][j]
RGBTRIPLE calc_blur_rgb(int index_i, int index_j, int height, int width, RGBTRIPLE image[height][width]);
