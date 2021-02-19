#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{

    // Proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open memory directory
    FILE *ogfile = fopen(argv[1], "r");

    // Unsupported file
    if (ogfile == NULL)
    {
        printf("File not supported\n");
        return 1;
    }

    // Image counter
    int j = 0;

    // Create space on RAM
    char *image = malloc(8 * sizeof(char));
    FILE *img = NULL;

    // Where data is passed to RAM
    BYTE *buffer = malloc(512 * sizeof(BYTE));

    // Repeat until End Of File
    while (fread(buffer, sizeof(BYTE), 512, ogfile) == 512)
    {
        // If start of JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Check if it's the first JPEG
            if (j == 0)
            {
                // Create new file
                {
                    sprintf(image, "%03i.jpg", j);
                    img = fopen(image, "w");
                    fwrite(buffer, sizeof(BYTE), 512, img);
                    j++;
                }

            }

            // Another image already exist
            else
            {
                // Close current file
                fclose(img);

                // Create new file
                sprintf(image, "%03i.jpg", j);
                img = fopen(image, "w");
                fwrite(buffer, sizeof(BYTE), 512, img);
                j++;
            }
        }

        // Not the start of a JPEG yet a image already exist
        else if (j > 0)
        {
            // Write buffer to file
            fwrite(buffer, sizeof(BYTE), 512, img);
        }

    }

    //Close open files when end of card is reached
    free(buffer);
    free(image);
    fclose(img);
}



