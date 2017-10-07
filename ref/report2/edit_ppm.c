#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_header(FILE *fp_in, char *magic_number,int *size_row, int *size_column, int *max_ind)
{
	int i;
	char *buffer;

	buffer=calloc(sizeof(char *),BUFSIZ);
	for(i=0;i<3;i++)
	{
		fgets(buffer,BUFSIZ,fp_in);

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
}
int read_image(FILE *fp_in, int size_row,unsigned char ***image)
{
	int i,j;
	char *buffer;

	buffer=calloc(sizeof(char *),BUFSIZ);

	j=0;
	while(fgets(buffer,BUFSIZ,fp_in)!=NULL)
	{

		image[0][j][0]=atoi(strtok(buffer," "));
		image[1][j][0]=atoi(strtok(NULL," "));
		image[2][j][0]=atoi(strtok(NULL," "));

		for(i=1;i<size_row;i++)
		{
			image[0][j][i]=atoi(strtok(NULL," "));
			image[1][j][i]=atoi(strtok(NULL," "));
			image[2][j][i]=atoi(strtok(NULL," "));
		}
		j++;
	}
	return 0;
}
int edit_image(unsigned char ***image, int size_row, int size_column)
{
	int i,j;

	for(i=0;i<size_column;i++)
	{
		for(j=0;j<size_row;j++)
		{
			if(j>=size_row/3 && j<2*size_row/3)
			{
				image[0][i][j]=0;
				image[1][i][j]=0;
			}	
		}
	}
	return 0;
}
int make_ppm(unsigned char ***image,int size_row,int size_column,char *magic_number,int max_ind)
{
	int i,j;
	FILE *fp;
	
	fp=fopen("./result/data.ppm","w");
	fprintf(fp,"%s%d %d\n%d\n",magic_number,size_row,size_column,max_ind);

	for(i=0;i<size_column;i++)
	{
		for(j=0;j<size_row;j++)
		{
			fprintf(fp,"%d ",image[0][i][j]);
			fprintf(fp,"%d ",image[1][i][j]);
			fprintf(fp,"%d ",image[2][i][j]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	return 0;
}
int main(char *argc,char **argv)
{
	int i,j;	//for文用

	FILE *fp_in;
	FILE *fp_out;
	char *buffer;
	char *magic_number;		//フォーマット用
	int size_row, size_column;	//画像サイズ
	int max_ind;			//輝度値用

	magic_number=calloc(sizeof(char *),2);
	buffer=calloc(sizeof(char *),BUFSIZ);

	//ファイル指定
	if((fp_in=fopen("../report1/result/data1.ppm","r"))==NULL)
	{
		fprintf(stderr,"data1のファイルを読み込めません。");
		exit(1);
	}
	read_header(fp_in,magic_number,&size_row,&size_column,&max_ind);

	//確認用
	printf("magic_number:%s",magic_number);
	printf("size:%d %d\n",size_row,size_column);
	printf("indensity:%d\n",max_ind);

	//////////////////////////////画像用///////////////////////////
	unsigned char ***image;
	image=calloc(sizeof(char **),3);
	for(i=0;i<3;i++)
		image[i]=calloc(sizeof(char *),size_row);

	for(i=0;i<3;i++)
	{
		for(j=0;j<size_row;j++)
			image[i][j]=calloc(sizeof(int),size_column);
	}
	///////////////////////////////////////////////////////////////

	read_image(fp_in, size_row,image);
	edit_image(image,size_row,size_column);
	make_ppm(image,size_row,size_column,magic_number,max_ind);
	return 0;
}
