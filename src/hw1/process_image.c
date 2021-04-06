#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    int w = im.w;
    int h = im.h;
    // int num_channels = im.c;

    // padding
    if (x >= w ) {
        x = w - 1;
    }
    if (y >= h) {
        y = h - 1;
    }

    int idx = (w * h) * c + w * y + x;
    // printf("image size (W, H, C): %d, %d, %d , inputs (X, Y, C): %d, %d, %d, output: %f\n",w, h, num_channels,x, y, c, *(im.data + idx));
    
    return *(im.data + idx);
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
        int w = im.w;
    int h = im.h;
    // int num_channels = im.c;

    // padding
    if (x >= w ) {
        x = w - 1;
    }
    if (y >= h) {
        y = h - 1;
    }
    int idx = (w * h) * c + w * y + x;
    *(im.data + idx) = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            for (int c = 0; c < im.c; c++) {
                set_pixel(copy, i, j, c, get_pixel(im, i, j, c));
            }
        }
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    double colorWeights[] = {0.299, 0.587, 0.114};
    double value = 0;
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            value  = 0;
            for (int channel = 0; channel < im.c; channel++) {
                value += colorWeights[channel] * get_pixel(im, i, j, channel);
            }
            set_pixel(gray, i, j, 0, value);
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            set_pixel(im, i, j, c, get_pixel(im, i, j, c) + v);
        }
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            for (int c = 0; c < im.c; c++) {
                if (get_pixel(im, i, j, c) > 1) {
                    set_pixel(im, i ,j, c, 1);
                } else if (get_pixel(im, i, j, c) < 0){
                    set_pixel(im, i ,j, c, 0);
                }
            }
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    float V, m, C, S, R, G, B, HPrime, H; 
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            R = get_pixel(im, i, j, 0);
            G = get_pixel(im, i, j, 1);
            B = get_pixel(im, i, j, 2);
            // calculate V
            V = three_way_max(R, G, B);

            // calculate S
            m = three_way_min(R, G, B);
            C = V - m;
            if (V == 0) {
                S = 0;
            } else {
                S = C / V;
            }

            // calculate H
            if (C == 0) {
                HPrime = 0;
            } else if (V == R) {
                HPrime = (G - B) / C;
            } else if (V == G) {
                HPrime = (B - R) / C + 2;
            } else if (V == B) {
                HPrime = (R - G) / C + 4;
            }
            if (HPrime < 0) {
                H = HPrime / 6 + 1;
            } else {
                H = HPrime / 6;
            }

            // replace RGB with HSV
            set_pixel(im, i, j, 0, H);
            set_pixel(im, i, j, 1, S);
            set_pixel(im, i, j, 2, V);
        }
    }

}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    float H, S, V, F, P, Q, T;
    int Hi;
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            // calculate initial values
            H = get_pixel(im, i, j, 0) * 6;
            S = get_pixel(im, i, j, 1);
            V = get_pixel(im, i, j, 2);
            Hi = floor(H);
            F = H - Hi;
            P = V * (1 - S);
            Q = V * (1 - F * S);
            T = V * (1 - (1 - F) * S);

            // assign values to different channels
            switch (Hi)
            {
            case 0:
                set_pixel(im, i, j, 0, V);
                set_pixel(im, i, j, 1, T);
                set_pixel(im, i, j, 2, P);
                break;
            case 1:
                set_pixel(im, i, j, 0, Q);
                set_pixel(im, i, j, 1, V);
                set_pixel(im, i, j, 2, P);
                break;
            case 2:
                set_pixel(im, i, j, 0, P);
                set_pixel(im, i, j, 1, V);
                set_pixel(im, i, j, 2, T);
                break;
            case 3:
                set_pixel(im, i, j, 0, P);
                set_pixel(im, i, j, 1, Q);
                set_pixel(im, i, j, 2, V);
                break;
            case 4:
                set_pixel(im, i, j, 0, T);
                set_pixel(im, i, j, 1, P);
                set_pixel(im, i, j, 2, V);
                break;
            case 5:
                set_pixel(im, i, j, 0, V);
                set_pixel(im, i, j, 1, P);
                set_pixel(im, i, j, 2, Q);
                break;
            default:
                break;
            }
        }
    }
}

// TODO Fill this in Extra Credit
void scale_image(image im, int c, float v) {
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            set_pixel(im, i, j, c, v * get_pixel(im, i, j, c));
        }
    }
    clamp_image(im);
}

