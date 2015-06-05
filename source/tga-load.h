#ifndef TGA_LOAD_H
#define TGA_LOAD_H

typedef struct _RGBA{
    int R;
    int G;
    int B;
    int A;
}RGBA;

class tga{
public:
    tga();
    tga(char[]);
    ~tga();
    int tga_alloc(long width, long height, int bpp);
    int tga_load(char[]);
    int tga_save(char[]);
    void tga_free();

    RGBA get_pixel(long x, long y);
    void set_pixel(long x, long y, RGBA col);

    long height, width;
    int bpp;
    unsigned char *img;
};

#endif // TGA_LOAD_H
