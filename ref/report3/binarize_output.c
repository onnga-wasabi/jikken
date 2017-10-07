#include <stdio.h>
#include <stdlib.h>

int make_image(unsigned char ***image,int size_row, int size_column)
{
	int i,j;

	for(i=0;i<size_column;i++)
	{
		for(j=0;j<size_row;j++)
		{
			if(j<=size_row/3)
			{
				image[0][i][j]=0;
				image[1][i][j]=0;
				image[2][i][j]=255;
			}
			else if(j>size_row/3 && j<2*size_row/3)
			{
				image[0][i][j]=255;
				image[1][i][j]=255;
				image[2][i][j]=255;
			}
			else
			{
				image[0][i][j]=255;
				image[1][i][j]=0;
				image[2][i][j]=0;
			}
		}
	}
	return 0;
} // end of make_image

int make_ppm(unsigned char ***image, int size_row, int size_column)
{
	int i,j;
	FILE *fp;

	fp=fopen("./result/data.ppm","wb");
	fprintf(fp,"P6\n");
	fprintf(fp,"%d %d\n",size_row,size_column);
	fprintf(fp,"255\n");
	for(i=0;i<size_column;i++)
	{
		for(j=0;j<size_row;j++)
		{
			fwrite(&image[0][i][j],sizeof(unsigned char),1,fp);
			fwrite(&image[1][i][j],sizeof(unsigned char),1,fp);
			fwrite(&image[2][i][j],sizeof(unsigned char),1,fp);
		}

	}
	return 0;
} //end of make_ppm

int main(int argc, char **argv)
{
	int i,j;
	unsigned char ***image;
	int size_row,size_column;

	size_row=400;
	size_column=200;
	/////////////////////画像用/////////////////////////////
	image=calloc(sizeof(unsigned char **),3);
	for(i=0;i<3;i++)
		image[i]=calloc(sizeof(unsigned char *),size_row);
	for(i=0;i<3;i++)
	{
		for(j=0;j<size_row;j++)
		{
			image[i][j]=calloc(sizeof(int),size_column);
		}
	}
	////////////////////////////////////////////////////////
	make_image(image,size_row,size_column);
	make_ppm(image,size_row,size_column);

	return 0;
} // end of main

