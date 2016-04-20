#include <stdio.h>
#include <stdlib.h>

#include "tga-load.h"

char args[] = {'s', 'p', 'o', 'e', 'i', 'd', 'm'};

///returns 1 if colors are equal else 0
bool RGBA_equal(RGBA c1, RGBA c2){
    if(c1.R == c2.R)
        if(c1.G == c2.G)
            if(c1.B == c2.B)
                if(c1.A == c2.A)
                    return 1;
    return 0;
}

int RGBA_diff(RGBA c1, RGBA c2){
    int rdiff = abs(c1.R - c2.R);
    int gdiff = abs(c1.G - c2.G);
    int bdiff = abs(c1.B - c2.B);
    int adiff = abs(c1.A - c2.A);

    return (rdiff + gdiff + bdiff + adiff);
}

///returns number of unique colors in tga image. 0 if invalid image
int RGBA_get_unique(tga *src, RGBA **colors, int diff, int maxc){
    if(src->img == NULL)
        return 0;

    int src_cnum = 0, src_cmax = 20;
    RGBA *src_colors = NULL;

    src_colors = (RGBA*) malloc(sizeof(RGBA) * src_cmax);
    for(int y = 0; y < src->height; y++){
        for(int x = 0; x < src->width; x++){
            if(src_cnum == 0) {
                src_colors[src_cnum++] = src->get_pixel(x, y);

            } else {
                RGBA col = src->get_pixel(x, y);
                bool exists = false;

                for(int i = 0; i < src_cnum; i++){
                    if(RGBA_diff(src_colors[i], col) <= diff){
                        exists = true;
                        break;
                    }
                }
                if(maxc == -1 || src_cnum < maxc){
                    if(!exists){
                        src_colors[src_cnum++] = col;

                        if(src_cnum == src_cmax){
                            src_cmax += 20;
                            src_colors = (RGBA*) realloc(src_colors, sizeof(RGBA) * src_cmax);
                        }
                    } else {
                        int index = 0;
                        int lowest = 256*4+1;
                        RGBA coltmp;

                        for(int i = 0; i < src_cnum; i++){
                            int tmp = RGBA_diff(col, src_colors[i]);
                            if(tmp < lowest){
                                index = i;
                                lowest = tmp;
                            }
                        }

                        coltmp = {src_colors[index].R,
                                src_colors[index].G,
                                src_colors[index].B,
                                src_colors[index].A};

                        src->set_pixel(x, y, coltmp);
                    }
                } else {
                    int index = 0;
                    int lowest = 256*4+1;
                    RGBA coltmp;

                    for(int i = 0; i < src_cnum; i++){
                        int tmp = RGBA_diff(col, src_colors[i]);
                        if(tmp < lowest){
                            index = i;
                            lowest = tmp;
                        }
                    }

                    coltmp = {src_colors[index].R,
                            src_colors[index].G,
                            src_colors[index].B,
                            src_colors[index].A};

                    src->set_pixel(x, y, coltmp);
                }
            }
        }
    }

    *colors = (RGBA*) realloc(src_colors, sizeof(RGBA) * src_cnum);

    return src_cnum;
}

int main(int argc, char *argv[]){

    if(argc > 1) {
        char *source = NULL;
        char *palette = NULL;
        char *outname = NULL;
        int cmdset = 0;
        int diff = 0;
        int colm = -1;

        for(int i = 1; i < argc; i++){
            if(argv[i][0] == '-'){
                if(argv[i][1] == args[0]){
                    source = &argv[i][2];
                } else if(argv[i][1] == args[1]){
                    palette = &argv[i][2];
                } else if(argv[i][1] == args[2]){
                    outname = &argv[i][2];
                } else if(argv[i][1] == args[3]){
                    source = &argv[i][2];
                    cmdset = 1;
                } else if(argv[i][1] == args[4]){
                    palette = &argv[i][2];
                    cmdset = 2;
                } else if(argv[i][1] == args[5]){
                    diff = strtol(&argv[i][2], NULL, 10);
                } else if(argv[i][1] == args[6]){
                    colm = strtol(&argv[i][2], NULL, 10);
                }
            }
        }
        if(outname == NULL)
            outname = (char*)"output.tga";

        if(cmdset == 0){
            bool cont = true;
            if(source == NULL){
                printf("Source image missing: -s\"path\\to\\img.tga\"");
                cont = false;
            }
            if(palette == NULL){
                printf("Source image missing: -p\"path\\to\\pal.tga\"");
                cont = false;
            }

            if(cont){
                printf("source: %s\npalette: %s\noutput name: %s\n\n", source, palette, outname);
                tga pal(palette);
                tga src(source);

                //int src_cnum = 0;
                //RGBA *src_colors = NULL;
                int pal_cnum = 0;
                RGBA *pal_colors = NULL;

                if(src.img != NULL && pal.img != NULL){
                    pal_cnum = RGBA_get_unique(&pal, &pal_colors, 0, -1);

                    tga ret;
                    if(!ret.tga_alloc(src.width, src.height, src.bpp)){

                        for(int y = 0; y < src.height; y++){
                            for(int x = 0; x < src.width; x++){
                                RGBA color = src.get_pixel(x, y);

                                int color_diff = 255*4 + 1;
                                int color_index = 0;

                                for(int i = 0; i < pal_cnum; i++){
                                    RGBA color2 = pal_colors[i];
                                    int color_diff2 = RGBA_diff(color, color2);

                                    if(color_diff2 < color_diff){
                                        color_diff = color_diff2;
                                        color_index = i;
                                    }
                                }

                                ret.set_pixel(x, y, pal_colors[color_index]);
                            }
                        }
                    } else printf("Failed to allocate image memory\n");

                    if(ret.tga_save(outname))
                        printf("Failed to save formatted image\n");
                    else
                        printf("Image successfully saved\n");
                    free(pal_colors);
                }
            }
        } else if(cmdset > 0){
            if(source != NULL){
                if(palette == NULL || cmdset == 1)
                    palette = (char*)"Not Given";
                printf("source: %s\npalette: %s\noutput name: %s\nminimum diff: %d\nmaximum colors %d\n\n",
                       source, palette, outname, diff, colm);

                tga src(source);
                RGBA *src_colors = NULL;
                int src_coln = 0;

                if(src.img != NULL){
                    src_coln = RGBA_get_unique(&src, &src_colors, diff, colm);
                    tga ret;

                    if(cmdset == 1){
                        int src_colnNew = 0;
                        for(int i = 0; i < src_coln; i++)
                            if(src_colors[i].A != -1)
                                src_colnNew++;
                        if(!ret.tga_alloc(src_colnNew, 1, src.bpp)){
                            for(int i = 0; i < src_coln; i++){
                                if(src_colors[i].A != -1)
                                    ret.set_pixel(i, 0, src_colors[i]);
                            }

                            if(ret.tga_save(outname))
                                printf("Failed to save source palette\n");
                            else
                                printf("Image saved succsefully\n");

                        }else printf("Failed to allocate image memory\n");
                    } else if(cmdset == 2){
                        tga pal(palette);
                        RGBA *pal_colors = NULL;
                        int pal_coln = 0;

                        if(pal.img != NULL){
                            if(!ret.tga_alloc(src.width, src.height, src.bpp)){
                                pal_coln = RGBA_get_unique(&pal, &pal_colors, -1, -1);

                                for(int y = 0; y < src.height; y++){
                                    for(int x = 0; x < src.width; x++){
                                        RGBA tmp = src.get_pixel(x, y);
                                        for(int i = 0; i < src_coln; i++){
                                            if(RGBA_equal(tmp, src_colors[i]))
                                                ret.set_pixel(x, y, pal_colors[i]);
                                        }
                                    }
                                }

                                if(ret.tga_save(outname))
                                    printf("Failed to save converted image\n");
                                else
                                    printf("Converted image saved succsefully\n");

                                free(pal_colors);

                            } else printf("Failed to allocate image memory");
                        } else printf("Failed to open palette image\n");
                    }

                    free(src_colors);
                } else printf("Failed to open source image\n");
            }
        }
    } else {
        printf("You can use the following command sets. Do not try to mix command set or you\nwill get undesired results\nR = required\nO = optional\n----------------------\n");
        printf("Convert image colors to closest match in palette image\n\nR -sPath\\to\\img.tga\nR -pPath\\to\\palette.tga\nO -oPath\\for\\output.tga\n----------------------\n");

        printf("Export or Import palette. ");
        printf("In order for this to work correctly you must first\nexport the palette of the source and then import your modified version of the\nexported palette\n\n");
        printf("R -ePath\\to\\source.tga\nO -iPath\\to\\palette.tga\nO -oPath\\for\\output.tga\nO -d#(min color difference)\nO -m#(Max colors)\n\n");
    }

    printf("complete");

    return 0;
}
