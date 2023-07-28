# IFS_Cpp
I made basically the same project 2 years ago in school, so I did it again, but a bit cleaner and (hopefully) way faster, because it's in Cpp and not Java.

If I lose more brain cells, I will implement it in C, but I don't want to kill myself (yet).\
There is no gui of the cpp program but I added a parser in Java to parse the output of the Cpp-program into a very, very much smaller picture.

I added my output (on mediafire), but if you really want to run the source code for yourself as it is, you probably should adjust MAX in "TestMain.cpp" if you don't want 340MB of plot data.

The plot is stored as lines of x "\t" y values with double precision.\
This approach is more or less precise but produces a lot of data.\
I have not experimented yet with storing the plot more efficiently, but implementing this would probably be a nightmare, so I don't care enough.

./IFS_Cpp contains the cpp source code for the main program\
./src contains the Java source code for the plot to image parser\
./plot\ parsed\ to\ image\ with\ ParseFromCpp.java contains an image of the output.\
https://www.mediafire.com/file/6171pcfa3ohihrn/plot.zip/file contains a zip of my output of the main program

When you plot your first IFS's you'll see...

TODO: I think I will implement a faster way to calculate everything by first storing every "hit" in a double array and then either compressing this before writing to the plot-data or by calculating the non-white portions of the int[][] in the java file from this double array. Writing so much data just slows everything so much down. But if you want precise data, you have to do this with this bottleneck. If you want fast good-looking pictures and not "scientific" data you can plot yourself, the second option is better. \
In the 2-year-old Java version it was like the second approach. There was no output plot.csv and the image was created while the programm was rendering it. This of course makes it so that if you want to have a good look at your plot you have to rerender everything, but I think there is a world of both solutions. \
Test with a chep implementation of the second approach and then when getting the HQ-renders, do the thing with the first approach.
