#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

#define BLOCK 512

int main(int argc, char *argv[])
{
    // Check for correct usage
    if (argc != 2)
    {
        printf("Usage: ./recover image.jpg\n");
        return 1;
    }
    
    // Open card.raw
    
    FILE *fp = fopen(argv[1], "r");
    FILE *out_fp = NULL; 
    
    unsigned char buffer[BLOCK]; 
    int counter = 0; 
    char file_name[8] = "000.jpeg"; 
    
    // You can put both &buffer or buffer, since it needs a pointer, but buffer also is already an address 
    
    while (fread(buffer, sizeof(unsigned char), BLOCK, fp) == BLOCK)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // We have a JPEG! Create it: 
            // Write to a file, but finish the file once the JPEG is done
            
            // Close existing file except if NULL
            
            if (out_fp != NULL && counter > 0)
            {
                fclose(out_fp);
            }
            
            // We want to save files with the name e.g. 000.jpg for the first file
            // %03i means three integer digits to represent the file name! Counter is the number we want to put in
            
            //FILE *new_img;
            // new_img = fopen(filename, "w");
            
            sprintf(file_name, "%03i.jpg", counter);

            out_fp = fopen(file_name, "w");
            
            counter++;
            
        }
        
        // If condition to write to blocks
       
        if (out_fp != NULL)
        {
            fwrite(buffer, sizeof(unsigned char), 512, out_fp);
        }
    }
    
    fclose(fp);
    
    
    
    /**
    // Put EOF!
    for (int i = 0; i < 100000; i++)
    {
        
        fread(buffer, 4, sizeof(BYTE), file_pointer);
        fread(buffer2, 4, sizeof(unsigned char), file_pointer);
        
        if (buffer2[0] == 0xff && buffer2[1] == 0xd8 && buffer2[2] == 0xff && (buffer2[3] & 0xf0) == 0xe0)
        {
            y++;
            printf("%i", y);
        }
        
        // & is a bitwise operator; See wikipedia for usage
        
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            printf("WORKS");
            
        }
        
    }
    **/
    
    
}
