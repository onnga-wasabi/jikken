#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct image_header{
	char format[3];
	int size_x;
	int size_y;
	int max;
};

int header_read(struct image_header* header, char* filename)
{
	int i;
	FILE *fp_in;
	char *buffer;

	fp_in=calloc(sizeof(char),BUFSIZ);
	buffer=calloc(sizeof(char),BUFSIZ);

	sprintf(buffer,"./%s",filename);
	if((fp_in=fopen(buffer,"r"))==NULL)
	{
		fprintf(stderr,"入力ファイルが開けません\n");
		exit(1);
	}

	for(i=0;i<3;i++)
	{
		fgets(buffer,BUFSIZ,fp_in);
		if(i==0)
		{
			sprintf(header->format,"%s",buffer);
		}
		else if(i==1)
		{
			header->size_x=atoi(strtok(buffer," "));
			header->size_y=atoi(strtok(NULL," "));
		}
		else
		{
			header->max=atoi(strtok(buffer," "));
		}
	}

	return 0;
}//end of header_read

int image_read(struct image_header* header, unsigned char* image, char* filename)
{	
	int i;
	FILE *fp_in;
	char *buffer;

	fp_in=calloc(sizeof(char),BUFSIZ);
	buffer=calloc(sizeof(char),BUFSIZ);

	sprintf(buffer,"./%s",filename);
	if((fp_in=fopen(buffer,"r"))==NULL)
	{
		fprintf(stderr,"入力ファイルが開けません\n");
		exit(1);
	}

	for(i=0;i<3;i++)
		fgets(buffer,BUFSIZ,fp_in);
	int column=0;
	while(fgets(buffer,BUFSIZ,fp_in)!=NULL)
	{
		image[0]=atoi(strtok(buffer," "));
		image[1]=atoi(strtok(NULL," "));
		image[2]=atoi(strtok(NULL," "));
		for(i=1;i<header->size_x;i++)
		{
			image[(column*header->size_x+i)*3+0]=atoi(strtok(NULL," "));
			image[(column*header->size_x+i)*3+1]=atoi(strtok(NULL," "));
			image[(column*header->size_x+i)*3+2]=atoi(strtok(NULL," "));
		}
		column++;
	}

	return 0;
}//end of image_read

int image_modify(struct image_header* header, unsigned char* image)
{
	int i,j;
	for(i=0;i<header->size_y;i++)
	{
		for(j=0;j<header->size_x;j++)
		{	
			if(i>(header->size_y/2))
			{
				image[(i*header->size_x+j)*3+0]=header->max-image[(i*header->size_x+j)*3+0];
				image[(i*header->size_x+j)*3+1]=header->max-image[(i*header->size_x+j)*3+1];
				image[(i*header->size_x+j)*3+2]=header->max-image[(i*header->size_x+j)*3+2];
			}
		}
	}

	return 0;
}//end of image_modify

int image_write(struct image_header* header, unsigned char* image, char* filename)
{	
	int i,j;
	char *buffer;
	FILE *fp_out;

	fp_out=calloc(sizeof(char),BUFSIZ);
	buffer=calloc(sizeof(char),BUFSIZ);

	sprintf(buffer,"./result/%s",filename);
	if((fp_out=fopen(buffer,"w"))==NULL)
	{
		fprintf(stderr,"出力ファイルが開けません\n");
		exit(1);
	}
	fprintf(fp_out,"%s\n",header->format);
	fprintf(fp_out,"%d %d\n",header->size_x,header->size_y);
	fprintf(fp_out,"%d\n",header->max);
	for(i=0;i<header->size_y;i++)
	{
		for(j=0;j<header->size_x;j++)
		{
			fprintf(fp_out,"%d ",image[(i*header->size_x+j)*3+0]);
			fprintf(fp_out,"%d ",image[(i*header->size_x+j)*3+1]);
			fprintf(fp_out,"%d ",image[(i*header->size_x+j)*3+2]);
		}
		fprintf(fp_out,"\n");
	}

	return 0;
}//end of image_write

int main(int argc, char** argv)
{
	struct image_header header;
	unsigned char* image;
	char* buffer;

	if(argc<3)
	{
		fprintf(stderr,"引数が%d個足りません:入力ファイルと出力ファイルを指定してください\n",3-argc);
		exit(1);
	}
	buffer=calloc(sizeof(char),BUFSIZ);

	header_read(&header,argv[1]);
	image=calloc(sizeof(char),header.size_y*header.size_x*3);
	image_read(&header,image,argv[1]);
	image_modify(&header,image);
	image_write(&header,image,argv[2]);

	free(buffer);
	free(image);

	return 0;
}//end of image_write
