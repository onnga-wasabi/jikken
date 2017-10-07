#include <stdio.h>
#include <stdlib.h>

int make_ppm(unsigned char ***image,int size_row,int size_column)
{
	int j,k;
	FILE *fp;
	
	fp=fopen("./result/data1.ppm","w");
	fprintf(fp,"P3\n%d %d\n255\n",size_row,size_column);

	for(j=0;j<size_column;j++)
	{
		for(k=0;k<size_row;k++)
		{
			fprintf(fp,"%d ",image[0][j][k]);
			fprintf(fp,"%d ",image[1][j][k]);
			fprintf(fp,"%d ",image[2][j][k]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	return 0;
} // end of make_ppm

int main(int argc,char **argv)
{
	unsigned char*** image;
	int size_row;
	int size_column;
	int i,j,k;	//for文用
	FILE *fp;

	size_row=400;
	size_column=200;

	image=calloc(sizeof(char **),3);
	for(i=0;i<3;i++)
		image[i]=calloc(sizeof(char *),size_column);
	for(i=0;i<3;i++)
	{
		for(j=0;j<size_row;j++)
			image[i][j]=calloc(sizeof(char),size_row);
	}
	for(j=0;j<size_column;j++)
	{
		for(k=0;k<size_row;k++)
		{
			if(k<(size_row/3))
				image[1][j][k]=255;
			else if(k<(2*size_row/3))
			{
				image[0][j][k]=255;
				image[1][j][k]=255;
				image[2][j][k]=255;
			}
			else
				image[0][j][k]=255;
		}
	}

	make_ppm(image,size_row,size_column);
	free(image);

	return 0; } // end of main
