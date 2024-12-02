#include <iostream>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <chrono>


#pragma pack(1)
#pragma once

using namespace std;

typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned char uchar;

struct bit_map {
    vector<vector<uchar>> r;
    vector<vector<uchar>> g;
    vector<vector<uchar>> b;
};

typedef struct tagBITMAPFILEHEADER
{
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

int rows;
int cols;

bit_map image;

bool fillAndAllocate(char *&buffer, const char *fileName, int &rows, int &cols, int &bufferSize)
{
    std::ifstream file(fileName);

    if (file)
    {
        file.seekg(0, std::ios::end);
        std::streampos length = file.tellg();
        file.seekg(0, std::ios::beg);

        buffer = new char[length];
        file.read(&buffer[0], length);

        PBITMAPFILEHEADER file_header;
        PBITMAPINFOHEADER info_header;

        file_header = (PBITMAPFILEHEADER)(&buffer[0]);
        info_header = (PBITMAPINFOHEADER)(&buffer[0] + sizeof(BITMAPFILEHEADER));
        rows = info_header->biHeight;
        cols = info_header->biWidth;
        bufferSize = file_header->bfSize;
        return 1;
    }
    else
    {
        cout << "File" << fileName << " doesn't exist!" << endl;
        return 0;
    }
}

void getPixlesFromBMP24(int end, int rows, int cols, char *fileReadBuffer)
{
    image.r.resize(rows, vector<uchar>(cols, 0));
    image.g.resize(rows, vector<uchar>(cols, 0));
    image.b.resize(rows, vector<uchar>(cols, 0));

    int count = 1;
    int extra = cols % 4;
    for (int i = 0; i < rows; i++)
    {
        count += extra;
        for (int j = cols - 1; j >= 0; j--)
            for (int k = 0; k < 3; k++)
            {
                switch (k)
                {
                    case 0:
                        image.r[i][j] = fileReadBuffer[end - count];
                        count++;
                        break;
                    case 1:
                        image.g[i][j] = fileReadBuffer[end - count];
                        count++;
                        break;
                    case 2:
                        image.b[i][j] = fileReadBuffer[end - count];
                        count++;
                        break;
                    default:
                        break;
                }
            }
    }
}

void writeOutBmp24(char *fileBuffer, const char *nameOfFileToCreate, int bufferSize)
{
    ofstream write(nameOfFileToCreate);
    if (!write)
    {
        cout << "Failed to write " << nameOfFileToCreate << endl;
        return;
    }
    int count = 1;
    int extra = cols % 4;
    for (int i = 0; i < rows; i++)
    {
        count += extra;
        for (int j = cols - 1; j >= 0; j--)
            for (int k = 0; k < 3; k++)
            {
                switch (k)
                {
                    case 0:
                        fileBuffer[bufferSize - count] = image.r[i][j];
                        count++;
                        break;
                    case 1:
                        fileBuffer[bufferSize - count] = image.g[i][j];
                        count++;
                        break;
                    case 2:
                        fileBuffer[bufferSize - count] = image.b[i][j];
                        count++;
                        break;
                    default:
                        break;
                }
            }
    }
    write.write(fileBuffer, bufferSize);
}

void mirrorOne() {
    bit_map img_copy = image;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            image.r[i][j] = img_copy.r[i][cols-j];
            image.g[i][j] = img_copy.g[i][cols-j];
            image.b[i][j] = img_copy.b[i][cols-j];
        }
    }
}

int main(int argc, char *argv[]) {
    char *fileBuffer;
    int bufferSize;
//    char *fileName = argv[1];
    char *fileName = "input.bmp";
//    auto started = chrono::high_resolution_clock::now();

    if (!fillAndAllocate(fileBuffer, fileName, rows, cols, bufferSize)) {
        cout << "File read error" << endl;
        return 1;
    }

    getPixlesFromBMP24(bufferSize, rows, cols, fileBuffer);
    cout<<bufferSize<<endl;
    cout<<image.r.size()*image.r[0].size()<<endl;
//    mirrorOne();
//    mirrorTwo();
//    sharping();
//    sepia();
//    cross();
    writeOutBmp24(fileBuffer, "filtered.bmp", bufferSize);

//    auto done = chrono::high_resolution_clock::now();
//    cout << chrono::duration_cast<chrono::milliseconds>(done - started).count() << endl;

    return 0;
}


//void mirrorTwo() {
//    bit_map img_copy = image;
//    for (int i = 0; i < rows; ++i) {
//        for (int j = 0; j < cols; ++j) {
//            image.r[i][j] = img_copy.r[rows-i][j];
//            image.g[i][j] = img_copy.g[rows-i][j];
//            image.b[i][j] = img_copy.b[rows-i][j];
//        }
//    }
//}

//unsigned char convolution(char channel, int i, int j) {
//    int result = 0;
//    if (channel == 'r'){
//    	result = img_copy.r[i-1][j]*(-1) + img_copy.r[i][j-1]*(-1) + img_copy.r[i][j]*(5) + img_copy.r[i][j+1]*(-1) + img_copy.r[i+1][j]*(-1);
//	}
//	if (channel == 'g'){
//    	result = img_copy.g[i-1][j]*(-1) + img_copy.g[i][j-1]*(-1) + img_copy.g[i][j]*(5) + img_copy.g[i][j+1]*(-1) + img_copy.g[i+1][j]*(-1);
//	}
//	if (channel == 'b'){
//    	result = img_copy.b[i-1][j]*(-1) + img_copy.b[i][j-1]*(-1) + img_copy.b[i][j]*(5) + img_copy.b[i][j+1]*(-1) + img_copy.b[i+1][j]*(-1);
//	}
//    return result;
//}

//void sharping() {
//    bit_map img_copy = image;
//    for (int i = 1; i < rows - 1; ++i) {
//        for (int j = 1; j < cols - 1; ++j) {
//            image.r[i][j] = convolution('r', i, j);
//            image.g[i][j] = convolution('g', i, j);
//            image.b[i][j] = convolution('b', i, j);
//        }
//    }
//}

//void sepia() {
//    bit_map img_copy = image;
//    for (int i = 0; i < rows; ++i) {
//        for (int j = 0; j < cols; ++j) {
//            int red = (img_copy.r[i][j] * 0.393) + (img_copy.g[i][j] * 0.769) + (img_copy.b[i][j] * 0.189);
//            int green = (img_copy.r[i][j] * 0.349) + (img_copy.g[i][j] * 0.686) + (img_copy.b[i][j] * 0.168);
//            int blue = (img_copy.r[i][j] * 0.272) + (img_copy.g[i][j] * 0.534) + (img_copy.b[i][j] * 0.131);
//
//            if (red >= 255)
//			image.r[i][j] = 255;
//            else
//			image.r[i][j] = red;
//
//            if (green >= 255)
//			image.g[i][j] = 255;
//            else
//			image.g[i][j] = green;
//
//            if (blue >= 255)
//			image.b[i][j] = 255;
//            else
//			image.b[i][j] = blue;
//        }
//    }
//}
//
//void cross() {
//    for (int i = 1; i < rows - 1; ++i) {
//        for (int j = 0; j < cols; ++j) {
//            if (i == j) {
//                image.r[i][j] = 255;
//                image.g[i][j] = 255;
//                image.b[i][j] = 255;
//
//                image.r[i + 1][j] = 255;
//                image.g[i + 1][j] = 255;
//                image.b[i + 1][j] = 255;
//
//                image.r[i - 1][j] = 255;
//                image.g[i - 1][j] = 255;
//                image.b[i - 1][j] = 255;
//            }
//            if ((i + j) == rows) {
//                image.r[i][j] = 255;
//                image.g[i][j] = 255;
//                image.b[i][j] = 255;
//
//                image.r[i + 1][j] = 255;
//                image.g[i + 1][j] = 255;
//                image.b[i + 1][j] = 255;
//
//                image.r[i - 1][j] = 255;
//                image.g[i - 1][j] = 255;
//                image.b[i - 1][j] = 255;
//            }
//        }
//    }
//}


