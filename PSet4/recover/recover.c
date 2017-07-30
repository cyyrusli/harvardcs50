#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char* argv[]){
	
		// ensure proper usage
		if (argc != 2)
		{
				fprintf(stderr, "Usage: ./recover infile\n");
				return 1;
		}
		
		char* infile = argv[1];
		
		FILE* fp = fopen(infile, "r");	

		if(fp == NULL)
		{	
				printf("Could not open %s.\n", infile);
				return 3;
		}

		// starting bytes of the jpeg file
		uint8_t checkjpg1[4] = {0xff, 0xd8, 0xff, 0xe0};
		uint8_t checkjpg2[4] = {0xff, 0xd8, 0xff, 0xe1};

		// track the jpeg files found in the bytestream and use it to create filenames
		int jpgcount = 0;

		// open outfile
		int open = 0;
		FILE* outp;

	  // read 512b blocks from file
		uint8_t buffer[512];
		uint8_t check[4];
		fread(buffer, 512, 1, fp);	

		while(fread(buffer, 512, 1, fp) > 0)
		{
				// add first four bytes into check buffer
				for(int i = 0; i < 4; i++)
				{
						check[i] = buffer[i];
				}

				// check for jpeg signature
				if((memcmp(checkjpg1, check, 4) == 0 ) || (memcmp(checkjpg2, check, sizeof(check)) == 0))
				{
						// create the file
						char filename[8];
						sprintf(filename, "%03d.jpg", jpgcount);

						if(open == 0)
						{
								outp = fopen(filename, "w");
								fwrite(buffer, sizeof(buffer), 1, outp);
								open = 1;
						}
						if(open == 1)
						{
								fclose(outp);
								outp = fopen(filename, "w");
								fwrite(buffer, sizeof(buffer), 1, outp);
								jpgcount++;
						}
				}
				else
				{
						if(open == 1)
						{
								fwrite(buffer, sizeof(buffer), 1, outp);
						}
				}
		}
    
    if(outp)
    {
      fclose(outp);
    }

		fclose(fp);
		return 0;
    
}