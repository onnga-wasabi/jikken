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

int affine(double scaler, double theta, image_t* img_in, image_t* img_out)
{
    int i,j;
    int in_row,in_column;
    int out_row,out_column;
    double sn,cs;
    int ix,iy;
    double scalex,scaley;

    in_row=img_in->row;
    in_column=img_in->column;
    
    sn=sin((theta/180)*M_PI);
    cs=cos((theta/180)*M_PI);

    if((theta>=0 && theta<=90) || (theta>=180 && theta<=270)){
        out_row=abs(in_row*cs)+abs(in_column*sn);
        out_column=abs(in_column*cs)+abs(in_row*sn);
    }
    else{
        out_row=abs(in_column*cs)+abs(in_row*sn);
        out_column=abs(in_row*cs)+abs(in_column*sn);
    }
    
    out_row*=scaler;
    out_column*=scaler;

    sprintf(img_out->magic,"%s",img_in->magic);
    img_out->row=out_row;
    img_out->column=out_column;
    img_out->max=img_in->max;

    scalex=(double)(out_row/in_row);
    scaley=(double)(out_column/in_column);

    img_out->array=malloc(3*sizeof(unsigned char**));
    for(i=0;i<3;i++){
        img_out->array[i]=malloc(img_out->column*sizeof(unsigned char*));
        for(j=0;j<img_out->column;j++){
            img_out->array[i][j]=malloc(img_out->row*sizeof(unsigned char));
        }
    }

    for(i=-img_out->column/2;i<img_out->column/2;i++){
        for(j=-img_out->row/2;j<img_out->row/2;j++){

            ix=(j*cs-i*sn+in_row/2);
            iy=(j*sn+i*cs+in_column/2);

            if(ix>=0 && iy>=0 && ix<in_row && iy<in_column){
                img_out->array[0][i+img_out->column/2][j+img_out->row/2]=img_in->array[0][iy][ix];
                img_out->array[1][i+img_out->column/2][j+img_out->row/2]=img_in->array[1][iy][ix];
                img_out->array[2][i+img_out->column/2][j+img_out->row/2]=img_in->array[2][iy][ix];
            }
            else{
                img_out->array[0][i+img_out->column/2][j+img_out->row/2]=150;
                img_out->array[1][i+img_out->column/2][j+img_out->row/2]=150;
                img_out->array[2][i+img_out->column/2][j+img_out->row/2]=150;
            }
        }
    }
    printf("%d\n",img_out->row);
    printf("%d\n",img_out->column);

    return 0;
}//end of affine

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
    fprintf(fp,"%d %d\n",img->row,img->column);
    fprintf(fp,"%d\n",img->max);
    for(i=0;i<img->column;i++){
        for(j=0;j<img->row;j++){
            fwrite(&img->array[0][i][j],sizeof(unsigned char),1,fp);
            fwrite(&img->array[1][i][j],sizeof(unsigned char),1,fp);
            fwrite(&img->array[2][i][j],sizeof(unsigned char),1,fp);
        }
    }
    return 0;

}//end of make_image

int main(int argc, char** argv)
{
    int i;
    image_t img_in,img_out;

    if(argc<5){
        fprintf(stderr,"引数が足りません、入力ファイル \
出力ファイル 拡大率 回転角 の順で実行してください.\n");
        exit(1);
    }
    
    read_image(argv[1],&img_in);
    affine(atof(argv[3]),atof(argv[4]),&img_in,&img_out);
    make_image(argv[2],&img_out);

    free(img_in.array);
    return 0;
}//end of main

