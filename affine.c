#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char* magic[3];
    int row,column,max;
    char*** array
} image_t;

image_t* read_image(char* input)
{
    int i,j,k;
    char* buffer;
    FILE* fp;
    image_t* img;

    buffer=calloc(BUFSIZ,sizeof(char*));
    sprintf(buffer,"./%s",input);
    if((fp=fopen(buffer,"rb"))==NULL){
        fprintf(stderr,"入力ファイルが開けません.\n");
        exit(1);
    }

    //read img
    for(i=0;i<3;i++){
        fgets(buffer,BUFSIZ,fp);
        if(i==0)
            sprintf(img->magic,"%s",buffer);
        if(i==1){
            img->row=atoi(strtok(buffer," "));
            img->column=atoi(strtok(NULL," "));
        }
        if(i==2)
            img->max=atoi(strtok(buffer," "));
    }

    //memory array
    img->array=malloc(3*sizeof(char**));
    for(i=0;i<3;i++){
        img->array[i]=malloc(img->column*sizeof(char*));
        for(j=0;j<(img->column);j++){
            img->array[i][j]=malloc(img->row*sizeof(char));
        }
    }
    for(j=0;j<(img->column);j++){
        for(k=0;k<(img->row);k++){
            fread(img->array[0][j][k],sizeof(unsigned char),1,fp);
            fread(img->array[1][j][k],sizeof(char),1,fp);
            fread(img->array[2][j][k],sizeof(char),1,fp);
        }
    }
    printf("hoge\n");

    return &img;

}//end of read_image

int main(int argc, char** argv)
{
    image_t* img;

    if(argc<3){
        fprintf(stderr,"引数が足りません、入力ファイルと出力ファイルは必須です.\n");
        exit(1);
    }
    
    img=read_image(argv[1]);
    printf("%d\n",img->array[2][0][0]);


    return 0;
}
