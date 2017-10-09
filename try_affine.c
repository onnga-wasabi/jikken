#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct{
    char* magic[3];
    int row,column,max;
    char*** array;
} image_t;

int read_image(char* input, image_t* img)
{
    int i,j,k;
    char* buffer;
    FILE* fp;

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
            sprintf(img->magic,"%s",(strtok(buffer,"\n")));
        if(i==1){
            img->row=atoi(strtok(buffer," "));
            img->column=atoi(strtok(NULL," "));
        }
        if(i==2)
            img->max=atoi(strtok(buffer," "));
    } 
    //memory array
    img->array=malloc(3*sizeof(unsigned char**));
    for(i=0;i<3;i++){
        img->array[i]=malloc(img->column*sizeof(unsigned char*));
        for(j=0;j<(img->column);j++){
            img->array[i][j]=malloc(img->row*sizeof(unsigned char));
        }
    }
    for(j=0;j<(img->column);j++){
        for(k=0;k<(img->row);k++){
            fread(&(img->array[0][j][k]),sizeof(unsigned char),1,fp);
            fread(&(img->array[1][j][k]),sizeof(unsigned char),1,fp);
            fread(&(img->array[2][j][k]),sizeof(unsigned char),1,fp);
        }
    }

    return 0;

}//end of read_image

int make_image(char* out, image_t* img)
{
    int i,j;
    char* buffer;
    FILE* fp;

    buffer=calloc(BUFSIZ,sizeof(char*));
    sprintf(buffer,"./%s",out);
    if((fp=fopen(buffer,"wb"))==NULL){
        fprintf(stderr,"出力ファイルが開けません.\n");
        exit(1);
    }
    fprintf(fp,"P6\n");
    fprintf(fp,"512 %d\n",img->column);
    fprintf(fp,"%d\n",img->max);
    for(i=0;i<img->column;i++){
        for(j=0;j<img->row;j++){
            fwrite(&img->array[0][i][j],sizeof(unsigned char),1,fp);
            fwrite(&img->array[1][i][j],sizeof(unsigned char),1,fp);
            fwrite(&img->array[2][i][j],sizeof(unsigned char),1,fp);
            fwrite(&img->array[0][i][j],sizeof(unsigned char),1,fp);
            fwrite(&img->array[1][i][j],sizeof(unsigned char),1,fp);
            fwrite(&img->array[2][i][j],sizeof(unsigned char),1,fp);
        }
    }
    return 0;
}//end of make_image

int affine(float scaler, int theta, image_t* img_in, image_t* img_out)
{
    sprintf(img_out->magic,"%s",img_in->magic);
    img_out->row=(int)img_in->row*scaler;
    img_out->column=(int)img_in->column*scaler;
    return 0;
}

int main(int argc, char** argv)
{
    int i;
    image_t img_in,img_out;

    if(argc<5){
        fprintf(stderr,"引数が足りません、入力ファイル 出力ファイル 拡大率 回転角 の順で実行してください.\n");
        exit(1);
    }
    
    read_image(argv[1],&img_in);
    //make_image(argv[2],&img);
    free(img_in.array);

    return 0;
}
