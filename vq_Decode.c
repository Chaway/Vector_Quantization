#include <stdio.h>
#include <stdlib.h>

#define rows 512       
#define cols 512
#define L 4      //input vector's size = L*L
#define N 256     //number of sample in codebook
#define NUM (rows*cols)/(L*L)    //input vectors' number

int index_inbo[NUM] = {0},codebook[N][L*L];
int output_vector[NUM][L*L],image[rows][cols];


int main(int argc ,char ** argv)
{
    FILE *fpi,*fpo;
    int row;
    unsigned char ch;

    fpi = fopen(argv[1],"rb");    //initialize FILE*
    if(fpi == NULL)
    {
        printf("open input file failed\n");
        exit(0);
    }

    fpo = fopen(argv[2],"wb");
    if(fpo == NULL)
    {
        printf("open output file failed\n");
        exit(0);
    }
   
    for(int i = 0; i < NUM; i++)
    {
        fread(&ch,1,1,fpi);
        index_inbo[i] = ch;
    }
    
    printf("Decompressing ...\n");

    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < L*L; j++)
        {
            fread(&ch,1,1,fpi);
            codebook[i][j] = ch;
        }
    }

    for(int i = 0; i < NUM; i++)    //get output vectors
    {
        for(int j = 0; j < L*L; j++)
        {
            output_vector[i][j] = codebook[index_inbo[i]][j];
        }
    }

    for(int i = 0; i < rows; i += 4)    //get output image
    {
        for(int j = 0; j < cols; j += 4)
        {
            for(int k = 0; k < 4; k++)
                for(int l = 0; l < 4; l++)
                    image[i+k][j+l] = output_vector[row][l+k*4];
            row ++;
        }
    }

    for(int i = 0; i < rows; i++)    //output image image.raw
    {
        for(int j = 0; j < cols; j++)
        {
            fwrite(&image[i][j],1,1,fpo);
        }
    }

	printf("Decompress finished\n");

    fclose(fpi);
    fclose(fpo);
    return 0;
}

