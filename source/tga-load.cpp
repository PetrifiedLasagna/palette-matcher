#include <stdio.h>
#include <stdlib.h>
#include "tga-load.h"

tga::tga(){
    width = height = bpp = 0;
    img = NULL;
}

tga::tga(char name[]) : tga(){
    int ret = tga_load(name);
    if(ret)
        printf("Error loading image: %d\n", ret);
}

tga::~tga(){
    tga_free();
}

int tga::tga_alloc(long width, long height, int bpp){
    if(img!=NULL)
        return 1;

    this->width = width;
    this->height = height;
    this->bpp = bpp;

    img = (unsigned char*) malloc(sizeof(unsigned char) * width * height * bpp);

    return 0;
}

int tga::tga_load(char name[]){
    FILE *f;
    f = fopen(name, "rb");

    if(!f || img!=NULL)
        return 1;

    int offset = 0, colort = 0, imaget = 0;
    fread(&offset, 1, 1, f);
    fread(&colort, 1, 1, f);
    fread(&imaget, 1, 1, f);

    if(colort!=0 && imaget!=2)
        return 2;

    fseek(f, 12, SEEK_SET);

    width = height = bpp = 0;

    fread(&width, 2, 1, f);
    fread(&height, 2, 1, f);
    fread(&bpp, 1, 1, f);

    if((width<=0 || height<=0) || (bpp!=24 && bpp!=32))
        return 3;

    bpp /= 8;
    img = (unsigned char*) malloc(width * height * bpp);

    if(img==NULL)
        return 4;

    fseek(f, offset+18, SEEK_SET);
    fread(img, width*height*bpp, 1, f);

    fclose(f);

    return 0;
}

int tga::tga_save(char name[]){
    FILE *f;
    f = fopen(name, "wb");

    if(!f || img == NULL)
        return 1;

    typedef struct TGAH {
        char offset;

        char colort;
        char imgt;

        char bpp;
        char orien;

        char palb;
        short palstr;
        short pallen;

        short orx;
        short ory;
        short width;
        short height;
    };

    TGAH inf = {0,
                0, 2,
                bpp * 8,0,
                0, 0, 0,
                0, 0, width, height};

    fwrite(&inf.offset, 1, 1, f);

    fwrite(&inf.colort, 1, 1, f);
    fwrite(&inf.imgt, 1, 1, f);

    fwrite(&inf.palstr, 2, 1, f);
    fwrite(&inf.pallen, 2, 1, f);
    fwrite(&inf.palb, 1, 1, f);

    fwrite(&inf.orx, 2, 1, f);
    fwrite(&inf.ory, 2, 1, f);
    fwrite(&inf.width, 2, 1, f);
    fwrite(&inf.height, 2, 1, f);

    fwrite(&inf.bpp, 1, 1, f);
    fwrite(&inf.orien, 1, 1, f);

    fwrite(img, width*height*bpp, 1, f);

    fclose(f);

    return 0;
}

void tga::tga_free(){
    if(img == NULL)
        return;

    free(img);
    width = height = bpp = 0;
}

RGBA tga::get_RGB(long x, long y){
    RGBA ret;

    ret.R = img[(x + width * y) * bpp];
    ret.G = img[(x + width * y) * bpp + 1];
    ret.B = img[(x + width * y) * bpp + 2];

    if(bpp == 3)
        ret.A = 255;
    else
        ret.A = img[x + width * y + 3];

    return ret;
}

void tga::set_RGB(long x, long y, RGBA col){
    img[(x + width * y) * bpp] = col.R;
    img[(x + width * y) * bpp + 1] = col.G;
    img[(x + width * y) * bpp + 2] = col.B;

    if(bpp == 4)
        img[x + width * y + 3] = col.A;
}

