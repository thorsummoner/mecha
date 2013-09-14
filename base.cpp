#include <iomanip>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <assert.h>
using namespace std;

#include "libtga.cpp"

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
    // for (int i = 0; i < tgaFile.width * tgaFile.height + 1; ++i)
    for (int i = 240; i < 260; ++i)
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

        cout << " ::: ";

        cout << endl;
    }

    term::close();
}