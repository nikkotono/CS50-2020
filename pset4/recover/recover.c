#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // argument count check
    if (argc != 2)
    {
        printf("Usage: ./recover image");
        return 1;
    }

    // open mem card
    char *inputfile = argv[1];
    FILE *cardFile = fopen(inputfile, "r");

    if (cardFile == NULL)
    {
        printf("Forensic image could not be opened\n");
        return 1;
    }

    BYTE buffer[512];
    char filename[8];

    FILE *img = NULL;
    BYTE readStatus = 1;

    int numOfPics = 0;
    bool foundJpeg = false;

    while (readStatus)
    {

        // read first block of data while capturing status
        // status will be 1 when block is read completely
        // and 0 when eof is reached.
        readStatus = fread(buffer, sizeof(buffer), 1, cardFile);

        // start of a new jpeg is found
        foundJpeg = buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0;

        // if we found a jpeg and its the first image
        // create new file and write
        if (foundJpeg && img == NULL)
        {
            sprintf(filename, "%03i.jpg", numOfPics++);
            img = fopen(filename, "w");
            fwrite(buffer, 512, 1, img);
            // printf("block 1\n");
        }

        // else if we found a jpeg and have a previous image
        // close previous image, open new one
        else if (foundJpeg && img)
        {
            fclose(img);
            sprintf(filename, "%03i.jpg", numOfPics++);
            img = fopen(filename, "w");
            fwrite(buffer, 512, 1, img);
            // printf("block 2\n");
        }

        // if we did not found a header and img
        // is open, keep writing until we find jpeg.
        // using readstatus to evade last picture write
        if (!foundJpeg && img)
        {
            fwrite(buffer, 512, readStatus, img);
            // printf("block 3\n");
        }
    }
    fclose(img);
}