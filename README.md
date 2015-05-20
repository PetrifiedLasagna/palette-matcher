Used to change colors of an image to closest match in a palette image. Currently useful if you are porting an image to a platform with limited colors or just for fun. However I will be adding support where you can export all of the unique colors of an image, modify the palette image, and then use it to change the appearance of the source image.

Currently only supports Targa(TGA) images that are 24/32 bit.

Command Line Options:

-s%Path to source% E.G. -sfolder1\myimg.tga     REQUIRED

-p%Path to palette% E.G. -pfolder1\mypal.tga    REQUIRED

-o%output filename% E.G. -onewimg.tga    Optional(default output.tga)


TODO:
-Add exporting/importing palette to source image
-Maybe add option to scale new image