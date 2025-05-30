#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Create a buffer for a block of data
    uint8_t buffer[512];

    // Counter for the number of JPEGs found
    int jpeg_count = 0;

    // File pointer for the output JPEG file
    FILE *jpeg_file = NULL;

    // While there's still data left to read from the memory card
    while (fread(buffer, 1, 512, card) == 512)
    {
        // Check if the block starts with a JPEG signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // If a JPEG file is already open, close it
            if (jpeg_file != NULL)
            {
                fclose(jpeg_file);
            }

            // Create a new JPEG file
            char filename[8];
            sprintf(filename, "%03i.jpg", jpeg_count);
            jpeg_file = fopen(filename, "w");
            if (jpeg_file == NULL)
            {
                printf("Could not create JPEG file.\n");
                fclose(card);
                return 1;
            }

            // Increment the JPEG counter
            jpeg_count++;
        }

        // If a JPEG file is open, write the block to it
        if (jpeg_file != NULL)
        {
            fwrite(buffer, 1, 512, jpeg_file);
        }
    }

    // Close any remaining files
    if (jpeg_file != NULL)
    {
        fclose(jpeg_file);
    }

    // Close the memory card file
    fclose(card);

    return 0;
}
