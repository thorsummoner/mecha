#include <iomanip>
#include <iostream>
#include <math.h>
#include <png.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <assert.h>
using namespace std;


struct STGA
{
    STGA()
    {
        data = (unsigned char*)0;
        width = 0;
        height = 0;
        byteCount = 0;
    }

    ~STGA() { delete[] data; data = 0; }

    void destroy() { delete[] data; data = 0; }

    int width;
    int height;
    unsigned char byteCount;
    unsigned char* data;
};
bool loadTGA(const char *filename, STGA& tgaFile)
{
    FILE *file;
    unsigned char type[4];
    unsigned char info[6];

        file = fopen(filename, "rb");

        if (!file) {
            cout << "\nfile open failed!\n";
            return false;
        }

    fread (&type, sizeof (char), 3, file);
    fseek (file, 12, SEEK_SET);
    fread (&info, sizeof (char), 6, file);

    //image type either 2 (color) or 3 (greyscale)
    if (type[1] != 0 || (type[2] != 2 && type[2] != 3))
    {
        fclose(file);
        cout << "\nimage type either 2 (color) or 3 (greyscale)\n";
        return false;
    }

    tgaFile.width = info[0] + info[1] * 256;
    tgaFile.height = info[2] + info[3] * 256;
    tgaFile.byteCount = info[4] / 8;

    if (tgaFile.byteCount != 3 && tgaFile.byteCount != 4) {
        fclose(file);
        cout << "\nSome other error...\n";
        return false;
    }

    long imageSize = tgaFile.width * tgaFile.height * tgaFile.width * tgaFile.byteCount;

    //allocate memory for image data
    tgaFile.data = (unsigned char *) malloc( imageSize ); //new unsigned char[imageSize];
    assert( imageSize );

    //read in image data
    fread(tgaFile.data, sizeof(unsigned char), imageSize, file);

    //close file
    fclose(file);

    return true;
}

/**
 * Class wrapper for terminal actions.
 */
namespace term
{
    int size [] = {80, 24};
    void calc()
    {
        #ifdef TIOCGSIZE
            struct ttysize ts;
            ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
            term::size[0] = ts.ts_cols;
            term::size[1] = ts.ts_lines;
        #elif defined(TIOCGWINSZ)
            struct winsize ts;
            ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
            term::size[0] = ts.ws_col;
            term::size[1] = ts.ws_row;
        #endif /* TIOCGSIZE */
    }
    void echo(char msg)
    {
        cout << msg;
    }
    void close()
    {
        cout << "\e[0m" << endl;
        exit(0);
    }
}
int main()
{
    term::calc();

    // cout << "\e[2J\e[0;0fTerminal is: " << term::size[0] << 'x' << term::size[1] << "\r";

    char str[term::size[0] * term::size[1]];



    // int i = term::size[1];
    // while (i>0) {
    //     int j = term::size[0];
    //     while (j>0) {
    //       cout << "\e[48;5;6m\e[37m#";
    //       --j;
    //     }
    //     --i;
    //     if (i>0)
    //     {
    //         cout << "\n";
    //     }
    // }
    // cout << "\r";

    int i = 255-15;
    int j = 0;
    while (i>-1) {
        cout << "\e[38;5;" << 256-i << "m#";
        --i;
        ++j;
        if (j == 6)
        {
            j = 0;
            cout << "  ";
            int k = 6;
            while (k>0) {
                cout << "\e[0m" << std::setw (4) << 256 - i - k << "\e[48;5;" << 256 - i - k << "m[         ]";
                --k;
            }

            cout << "\e[0m" << endl;
        }
    }
    cout << "\e[0m";

    char filename [22] = "Redwingblackbird1.tga";
    STGA tgaFile;
    if (!loadTGA(filename, tgaFile)) {
        cout << "\nFailed to load Targa.\n";
        exit(0);
    }
    cout << "\nTarga Loaded!!!\n";
    // cout << "data:      " << tgaFile.data      << "\n";
    cout << "width:     " << tgaFile.width     << "\n";
    cout << "height:    " << tgaFile.height    << "\n";
    cout << "byteCount: " << tgaFile.byteCount * 1<< "\n";
    cout << "pixels:    " << tgaFile.width * tgaFile.height << "\n";
    cout << "pixels ch: " << strlen((char*)tgaFile.data) / (tgaFile.byteCount * 1) << "\n";

    int col = 1;
    int row = 1;
    for (int i = 0; i < tgaFile.width * tgaFile.height + 1; ++i)
    {
        row = floor(i / tgaFile.width) + 1;
        col = i - (floor(i / tgaFile.width) * tgaFile.width) + 1;
        // cout << "[" << row << ", " << col << "], ";
        cout << "[" << row << ", " << col << "]: Index: ";

        int j = 0;
        while (j<(tgaFile.byteCount * 1)) {
            printf( "%.0d ", tgaFile.data[i*3+j]);
            ++j;
        }

        cout << endl;
    }

    printf( "%.08x\n", tgaFile.data );


    term::close();
}