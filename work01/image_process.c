#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct{
    char* magic[3];
    int row,column,max;
    unsigned char*** array;
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
    //memoryの動的確保
    img->array=malloc(3*sizeof(unsigned char**));
    for(i=0;i<3;i++){
        img->array[i]=malloc(img->column*sizeof(unsigned char*));
        for(j=0;j<(img->column);j++){
            img->array[i][j]=malloc(img->row*sizeof(unsigned char));
        }
    }
    //読み込み
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
    int i,j,k;//for
    double sn,cs;//sin,cos
    sn=sin((theta/180)*M_PI);
    cs=cos((theta/180)*M_PI);

    int in_row,in_column;//入力画像のサイズ
    in_row=img_in->row;
    in_column=img_in->column;

    int out_row,out_column;//出力画像のサイズ
    double ix,iy;//出力画像の画素から計算した元画像の座標
    int nx,ny;//出力画像の画素から計算した元画像の座標

    //出力画像のサイズを計算
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

    //メモリの動的確保
    img_out->array=malloc(3*sizeof(unsigned char**));
    for(i=0;i<3;i++){
        img_out->array[i]=malloc(out_column*sizeof(unsigned char*));
        for(j=0;j<out_column;j++){
            img_out->array[i][j]=malloc(out_row*sizeof(unsigned char));
        }
    }

    //画像の中心を軸にして回転させるため,画像サイズの半分ずらす
    for(i=-out_column/2;i<out_column/2;i++){
        for(j=-out_row/2;j<out_row/2;j++){

            //回転後の座標を計算
            //拡大率でわると元画像が内接する長方形のサイズになる
            ix=(j*cs-i*sn)/scaler;
            iy=(j*sn+i*cs)/scaler;

            //画像の中心が元画像の中心と
            //同じになるように元画像の半分戻す
            ix+=in_row/2;
            iy+=in_column/2;
            
            //倍精度をint型にキャスト
            nx=ix;
            ny=iy;

            //ix,iyはバイリニア補間の重みになる
            ix=ix-nx;
            iy=iy-ny;


            if(nx>=0 && ny>=0 && nx<in_row-1 && ny<in_column-1){
                for(k=0;k<3;k++){
                    img_out->array[k]\
                        [i+out_column/2][j+out_row/2]=(int)(\
                    (double)img_in->array[k][ny][nx]*(1-ix)*(1-iy)+\
                    (double)img_in->array[k][ny][nx+1]*ix*(1-iy)+\
                    (double)img_in->array[k][ny+1][nx]*(1-ix)*iy+\
                    (double)img_in->array[k][ny+1][nx+1]*ix*iy);
                }
            }
            //元画像が値を持っていない（元画像の外側を参照しているとき）
            else{
                img_out->array[0][i+out_column/2][j+out_row/2]=150;
                img_out->array[1][i+out_column/2][j+out_row/2]=150;
                img_out->array[2][i+out_column/2][j+out_row/2]=150;
            }
        }
    }

    //header部分
    sprintf(img_out->magic,"%s",img_in->magic);
    img_out->row=out_row;
    img_out->column=out_column;
    img_out->max=img_in->max;

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
        fprintf(stderr,"引数が足りません,入力ファイル \
出力ファイル 拡大率 回転角 の順で実行してください.\n");
        exit(1);
    }
    
    read_image(argv[1],&img_in);
    affine(atof(argv[3]),atof(argv[4]),&img_in,&img_out);
    free(img_in.array);
    make_image(argv[2],&img_out);
    free(img_out.array);

    return 0;
}//end of main
