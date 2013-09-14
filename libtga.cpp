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