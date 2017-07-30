#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Usage: ./whodunit input file \n");
        return 1;
    }

    // Open input file 
    FILE* clue_ptr = fopen(argv[1], "r");
    if (clue_ptr == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 2;
    }

    // Open output file
    FILE* answer_ptr = fopen(argv[2], "w");
    if (answer_ptr == NULL)
    {
        fclose(clue_ptr);
        fprintf(stderr, "Could not create %s.\n", "answer.bmp");
        return 3;
    }

    // Read input file's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, clue_ptr);

    // Read input file's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, clue_ptr);

    // Ensure input file is a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(answer_ptr);
        fclose(clue_ptr);
        fprintf(stderr, "File format not supported.\n");
        return 4;
    }

    // Write output file's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, answer_ptr);

    // Write output file's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, answer_ptr);

    // Padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Iterate over input file's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // Iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // Temporary storage
            RGBTRIPLE triple;

            // Read RGB triple from input file
            fread(&triple, sizeof(RGBTRIPLE), 1, clue_ptr);

            // write RGB triple to answer.bmp
            if(triple.rgbtRed == 255)
            {
		triple.rgbtRed = 0;
  		triple.rgbtBlue = 100;
		triple.rgbtGreen = 60;
            } 
            fwrite(&triple, sizeof(RGBTRIPLE), 1, answer_ptr);
	    
	    
        }

        // Skip over padding then add it back
        fseek(clue_ptr, padding, SEEK_CUR);

        for (int k = 0; k < padding; k++)
            fputc(0x00, answer_ptr);
    }

    // Close input file
    fclose(clue_ptr);

    // Close output file
    fclose(answer_ptr);

    return 0;
}