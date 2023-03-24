#include "bmp.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]);

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]);

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width]);

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]);

// Border a 2-D image array with black pixels
void border_image(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE bordered_image[height + 2][width + 2]);

// Calculate blur rgb values for pixel image[i][j]
RGBTRIPLE calc_blur_rgb(int index_i, int index_j, int height, int width, RGBTRIPLE image[height][width]);

// Calculate edge rgb values for pixel image[i][j]
RGBTRIPLE calc_edge(int index_i, int index_j, int height, int width, RGBTRIPLE image[height][width]);
