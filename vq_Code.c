#include <stdio.h>
#include <stdlib.h>

#define rows 512       
#define cols 512
#define L 4     //input vector's size = L*L
#define N 256     //number of sample in codebook
#define NUM (rows*cols)/(L*L)    //input vectors' number
#define MAX 0x7FFFFFFF
#define Mtime 3
int image[rows][cols],index_inbo[NUM] = {0},codebook[N][L*L];
int input_vector[NUM][L*L],output_vector[NUM][L*L];

int cal_Dis(int *a,int *b)    //calculate sum of square of the distance between 16 dimension vector
{
    int sum_sq = 0;
    for(int i = 0; i < L*L; i++)
        sum_sq += (a[i]-b[i])*(a[i] - b[i]);
    return sum_sq;
}

void Vector_Quantization()    //vector quantization
{
    int minD,nowD;
    int sum[N][L*L] = {{0}},count[N] = {0},sum1 = 0;

    for(int i = 0; i < N; i++)    //initialize  codebook
    {
        for(int j = 0; j < L*L; j++)
        {
            codebook[i][j] = input_vector[i*63+32][j]; 
        }
    }


    for(int i = 0; i < NUM; i++)    //find nearest 
    {
        minD = MAX;
        for(int j = 1; j < N; j++)
        {
            nowD = cal_Dis(input_vector[i],codebook[j]);
            if(minD > nowD)
            {
                minD = nowD;
                index_inbo[i] = j;
            }
        }
        
    }

  for(int time = 0;time < Mtime;time ++)
  {
    for(int i = 0; i < NUM; i++)    //calculate center of each region
    {
        for(int j = 0; j < L*L; j++)
        {
            sum[index_inbo[i]][j] += input_vector[i][j];
        }
        count[index_inbo[i]]++;
    }

    for(int i = 0; i < N; i++)    //update codebook
    {  
    	if(count[i] != 0)
        for(int j = 0; j < L*L; j++)
        {
        	
             codebook[i][j] = sum[i][j]/count[i];
        }
    }

    for(int i = 0; i < NUM; i++)    //calculate quantization value in new codebook
    {
        minD = MAX;
        for(int j = 1; j<N; j++)
        {
            nowD = cal_Dis(input_vector[i],codebook[j]);
            if(minD > nowD)
            {
                minD = nowD;
                index_inbo[i] = j;
            }
        }
        
    }
  }
}



int main(int argc ,char ** argv)
{
    FILE *fpi,*fpo;    //in file,out file
    int row = 0;
    unsigned char ch;

    fpi = fopen(argv[1],"rb");    //initialize file point
    if(fpi == NULL)
    {
        printf("input file open failed\n");
        exit(0);
    }
    

    fpo = fopen(argv[2],"wb");
    if(fpo == NULL)
    {
        printf("output file open failed\n");
        exit(0);
    }


    for(int i = 0; i < rows; i++)    //read image
    {
        for(int j = 0; j < cols; j++)
        {
            fread(&ch,1,1,fpi);
            image[i][j] = ch;
        }
    }


    for(int i = 0; i < rows; i += 4)    //segment 16-dismension vector
    {
        for(int j = 0; j < cols; j += 4)
        {
            for(int k = 0; k < 4; k++)
                for(int l = 0; l < 4; l++)
                    input_vector[row][l+k*4] = image[i+k][j+l];
            row ++;
        }
    }

    printf("Compressing ... \n");
    Vector_Quantization();    //vector quantization

    for(int i = 0; i < NUM; i++)    //get output vector
    {
        for(int j = 0; j < L*L; j++)
        {
            output_vector[i][j] = codebook[index_inbo[i]][j];
        }
    }

    row = 0;

    for(int i = 0; i < rows; i += 4)
    {
        for(int j = 0; j < cols; j += 4)
        {
            for(int k = 0; k < 4; k++)
                for(int l = 0; l < 4; l++)
                    image[i+k][j+l] = output_vector[row][l+k*4];
            row ++;
        }
    }
    
    int com = 0;
    for(int i = 0; i < NUM; i++)    //output
    {
        fwrite(&index_inbo[i],1,1,fpo);
        com ++;
    }

    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < L*L; j++)
        {
            fwrite(&codebook[i][j],1,1,fpo);
            com ++;
        }
    }

    
    printf("Compress finish\n");
    printf("Size of origin image is %d bytes\n",512*512);
    printf("Size of compressed image is %d bytes\n",com);
    printf("Compression rate is %f\n",(float)512*512/com);


    fclose(fpi);
    fclose(fpo);
    return 0;
}
