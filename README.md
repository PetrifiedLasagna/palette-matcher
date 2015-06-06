Used to change colors of an image to closest match in a palette image. Currently useful if you are porting an image to a platform with limited colors or just for fun. However I will be adding support where you can export all of the unique colors of an image, modify the palette image, and then use it to change the appearance of the source image.


Currently only supports Targa(TGA) images that are 24/32 bit.


Command Line Sets:

Convert colors in image to closest match in palette image

-s%Path to source% E.G. -sfolder1\myimg.tga     REQUIRED

-p%Path to palette% E.G. -pfolder1\mypal.tga    REQUIRED

-o%output filename% E.G. -onewimg.tga    Optional(default output.tga)


--------------------------
Export palette of image

-e%Path to source% E.G -efolder1\myimg.tga REQUIRED

-o%output filename% E.G -onewimg.tga Optional(defualt output.tga)

-d#(minimum unique color difference) E.G -d41 Optional(default 0)

-m#(maximum colors) E.G -m500 Optional (default -1(unlimited))

--------------------------
Import palette to image. You should modify the palette that was exported with the -e command. You should also use the same arguments to get the desired result

-e%Path to source% E.G -efolder1\myimg.tga REQUIRED

-i%Path to palette% E.G -ifolder1\mypalette.tga REQUIRED

-o%output filename% E.G -onewimg.tga Optional(defualt output.tga)

-d#(minimum allowed color difference) E.G -d41 Optional(default 0)

-m#(maximum colors) E.G -m500 Optional (default -1(unlimited))

TODO:

COMPLETED -Add exporting/importing palette to source image

-Maybe add option to scale new image


License:

If you use this code or application, if it is modified or not, in any of your PUBLIC projects(finished application or source code) you must have a link to this repository somewhere in your application files(e.g. readme.txt) As well as mentioning my username and crediting with development of this specific code.
