#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_header(FILE *fp,char *magic_number, int *size_row, int *size_column, int *max_ind)
{
	int i;
	char *buffer;

	buffer=calloc(sizeof(char *),BUFSIZ);
	for(i=0;i<3;i++)
	{
		fgets(buffer,BUFSIZ,fp);

		if(i==0)
		{
			sprintf(magic_number,"%s",buffer);
		}
		else if(i==1)
		{
			*size_row=atoi(strtok(buffer," "));
			*size_column=atoi(strtok(NULL," "));
		}
		else
		{
			*max_ind=atoi(strtok(buffer," "));
		}
	}
	return 0;
} // end of read_header

int read_image(FILE *fp, unsigned char ***image, int size_row, int size_column)
{
	int i,j;

	for(i=0;i<size_column;i++)
	{
		for(j=0;j<size_row;j++)
		{
			fread(&image[0][i][j],sizeof(unsigned char),1,fp);
			fread(&image[1][i][j],sizeof(unsigned char),1,fp);
			fread(&image[2][i][j],sizeof(unsigned char),1,fp);
		}
	}
	return 0;
} // end of read_image

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

int edit_image(unsigned char ***image,int size_row, int size_column)
{
	int i,j;

	for(i=0;i<size_column;i++)
	{
		for(j=0;j<size_row;j++)
		{
			if(j>size_row/3 && j<2*size_row/3)
				image[1][i][j]=0;
		}
	}
	return 0;
} // end of edit_image

int make_ppm(unsigned char ***image, char *magic_number, int size_row, int size_column, int max_ind)
{
	int i,j;
	FILE *fp;

	fp=fopen("./result/data2.ppm","wb");
	fprintf(fp,"%s",magic_number);
	fprintf(fp,"%d %d\n",size_row,size_column);
	fprintf(fp,"%d\n",max_ind);
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

	FILE *fp;
	char *magic_number;
	int size_row,size_column;
	int max_ind;

	magic_number=calloc(sizeof(char *),2);
	
	if((fp=fopen("./result/data.ppm","rb"))==NULL)
	{
		fprintf(stderr,"failed to read file");
		exit(1);
	}
	read_header(fp, magic_number, &size_row, &size_column, &max_ind);

	/////////////////////画像用/////////////////////////////
	unsigned char ***image;
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
	read_image(fp,image,size_row,size_column);
//	make_image(image,size_row,size_column);
	edit_image(image,size_row,size_column);
	make_ppm(image,magic_number,size_row,size_column,max_ind);

	return 0;
} // end of main

