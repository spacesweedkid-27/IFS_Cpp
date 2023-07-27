# IFS_Cpp
I made basically the same project 2 years ago in school, so I did it again, but a bit cleaner and (hopefully) way faster, because it's in Cpp and not Java.

If I lose more brain cells, I will implement it in C, but I don't want to kill myself (yet).\
There is no gui of the cpp program but I added a parser in Java to parse the output of the Cpp-program into a very, very much smaller picture.

I added my output (on mediafire), but if you really want to run the source code for yourself as it is, you probably should adjust MAX in "TestMain.cpp" if you don't want 340MB of plot data.\ 

The plot is stored as lines of x "\t" y values with double precision.\
This approach is more or less precise but produces a lot of data.\
I have not experimented yet with storing the plot more efficiently, but implementing this would probably be a nightmare, so I don't care enough.

./IFS_Cpp contains the cpp source code for the main program\
./src contains the Java source code for the plot to image parser\
./plot\ parsed\ to\ image\ with\ ParseFromCpp.java contains an image of the output.\
https://www.mediafire.com/file/6171pcfa3ohihrn/plot.zip/file contains a zip of my output of the main program

When you plot your first IFS's you'll see...
