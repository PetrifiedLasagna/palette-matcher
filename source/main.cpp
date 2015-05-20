#include <stdio.h>
#include <stdlib.h>

#include "../GJ/tga-load.h"

char args[] = {'s', 'p', 'o'};

///returns 1 if colors are equal else 0
bool RGBA_equal(RGBA c1, RGBA c2){
    if(c1.R == c2.R)
        if(c1.G == c2.G)
            if(c1.B == c2.B)
                return 1;
    return 0;
}

int RGBA_diff(RGBA c1, RGBA c2){
    int rdiff = abs(c1.R - c2.R);
    int gdiff = abs(c1.G - c2.G);
    int bdiff = abs(c1.B - c2.B);

    return (rdiff + gdiff + bdiff);
}

///returns number of unique colors in tga image. 0 if invalid image
int RGBA_get_unique(tga *src, RGBA **colors){
    if(src->img == NULL)
        return 0;

    int src_cnum = 0, src_cmax = 20;
    RGBA *src_colors = NULL;

    src_colors = (RGBA*) malloc(sizeof(RGBA) * src_cmax);
    for(int y = 0; y < src->height; y++){
        for(int x = 0; x < src->width; x++){
            if(src_cnum == 0) {
                src_colors[src_cnum++] = src->get_RGB(x, y);

            } else {
                RGBA col = src->get_RGB(x, y);
                bool exists = false;

                for(int i = 0; i < src_cnum; i++){
                    if(!RGBA_equal(src->get_RGB(x, y), col)){
                        exists = true;
                        break;
                    }
                }

                if(!exists){
                    src_colors[src_cnum++] = col;

                    if(src_cnum == src_cmax){
                        src_cmax += 20;
                        src_colors = (RGBA*) realloc(src_colors, sizeof(RGBA) * src_cmax);
                    }
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

        for(int i = 1; i < argc; i++){
            if(argv[i][0] == '-'){
                if(argv[i][1] == args[0]){
                    source = &argv[i][2];
                } else if(argv[i][1] == args[1]){
                    palette = &argv[i][2];
                } else if(argv[i][1] == args[2]){
                    outname = &argv[i][2];
                }
            }
        }

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
            if(outname == NULL)
                outname = (char*)"output.tga";

            printf("source: %s\npalette: %s\noutput name: %s\n\n", source, palette, outname);
            tga pal(palette);
            tga src(source);

            //int src_cnum = 0;
            //RGBA *src_colors = NULL;
            int pal_cnum = 0;
            RGBA *pal_colors = NULL;

            if(src.img != NULL && pal.img != NULL){
                pal_cnum = RGBA_get_unique(&pal, &pal_colors);

                tga ret;
                if(!ret.tga_alloc(src.width, src.height, src.bpp)){

                    for(int y = 0; y < src.height; y++){
                        for(int x = 0; x < src.width; x++){
                            RGBA color = src.get_RGB(x, y);

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

                            ret.set_RGB(x, y, pal_colors[color_index]);
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
    } else {
        printf("You must enter two images in this format\n-s\"path\\to\\img.tga\"\n-p\"path\\to\\pal.tga\"\n");
    }

    printf("complete");

    return 0;
}
