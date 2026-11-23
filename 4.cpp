
#include <iostream>
#include <cstdio>
#include <vector>
#include <gd.h>
#include <omp.h>
using namespace std;

int colors[8][3] = {
    {255,0,0}, {0,255,0}, {0,0,255}, {255,255,0},
    {255,0,255}, {0,255,255}, {255,128,0}, {128,0,255}
};

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cout << "Usage: ./image input.png output.png schedule.png\n";
        return 1;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (!fp) {
        cout << "Cannot open input file: " << argv[1] << endl;
        return 1;
    }

    gdImagePtr img = gdImageCreateFromPng(fp);
    fclose(fp);

    if (!img) {
        cout << "Cannot read PNG image!" << endl;
        return 1;
    }

    int w = gdImageSX(img), h = gdImageSY(img);
    int n = w * h, th, s, ch;

    cout << "Enter threads (1-8): ";
    cin >> th;
    cout << "Schedule (0-Default, 1-Static, 2-Dynamic, 3-Guided): ";
    cin >> s;
    cout << "Enter chunk size: ";
    cin >> ch;

    if (th < 1 || th > 8 || s < 0 || s > 3 || ch < 1) {
        cout << "Invalid input\n";
        gdImageDestroy(img);
        return 1;
    }

    vector<int> input(n), gray(n), owner(n);

    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++) {
            int p = gdImageGetPixel(img, x, y);
            input[y*w+x] = (gdImageRed(img,p)<<16) |
                           (gdImageGreen(img,p)<<8) |
                            gdImageBlue(img,p);
        }

    omp_set_num_threads(th);
    double t = omp_get_wtime();

    if (s == 0) {
        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            int r=input[i]>>16, g=(input[i]>>8)&255, b=input[i]&255;
            int a=(r+g+b)/3;
            gray[i]=(a<<16)|(a<<8)|a;
            owner[i]=omp_get_thread_num();
        }
    }
    else if (s == 1) {
        #pragma omp parallel for schedule(static,ch)
        for (int i = 0; i < n; i++) {
            int r=input[i]>>16, g=(input[i]>>8)&255, b=input[i]&255;
            int a=(r+g+b)/3;
            gray[i]=(a<<16)|(a<<8)|a;
            owner[i]=omp_get_thread_num();
        }
    }
    else if (s == 2) {
        #pragma omp parallel for schedule(dynamic,ch)
        for (int i = 0; i < n; i++) {
            int r=input[i]>>16, g=(input[i]>>8)&255, b=input[i]&255;
            int a=(r+g+b)/3;
            gray[i]=(a<<16)|(a<<8)|a;
            owner[i]=omp_get_thread_num();
        }
    }
    else {
        #pragma omp parallel for schedule(guided,ch)
        for (int i = 0; i < n; i++) {
            int r=input[i]>>16, g=(input[i]>>8)&255, b=input[i]&255;
            int a=(r+g+b)/3;
            gray[i]=(a<<16)|(a<<8)|a;
            owner[i]=omp_get_thread_num();
        }
    }

    t = omp_get_wtime() - t;

    gdImagePtr out = gdImageCreateTrueColor(w,h);
    gdImagePtr vis = gdImageCreateTrueColor(w,h);

    for (int i = 0; i < n; i++) {
        int x=i%w, y=i/w, id=owner[i];
        gdImageSetPixel(out,x,y,gray[i]);

        int c=(colors[id][0]<<16)|(colors[id][1]<<8)|colors[id][2];
        gdImageSetPixel(vis,x,y,c);
    }

    FILE *f=fopen(argv[2],"wb"), *v=fopen(argv[3],"wb");

    if (!f || !v) {
        if(f) fclose(f);
        if(v) fclose(v);
        gdImageDestroy(out);
        gdImageDestroy(vis);
        gdImageDestroy(img);
        return 1;
    }

    gdImagePng(out,f);
    gdImagePng(vis,v);
    fclose(f);
    fclose(v);

    cout << "Grayscale image: " << argv[2] << endl;
    cout << "Scheduling image: " << argv[3] << endl;
    cout << "Time: " << t << " seconds" << endl;

    gdImageDestroy(img);
    gdImageDestroy(out);
    gdImageDestroy(vis);

    return 0;
}
