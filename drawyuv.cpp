// drawyuv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2/opencv.hpp"

int draw_rect_yuv(unsigned char *data, int imgw, int imgh, int x, int y, int w, int h);
int main(int argc, char* argv[])
{
    cv::Mat im, imyuv0, imyuv,im2, imgray;
    im = cv::imread("c:\\users\\yikaifan\\test.jpg");
    im.data[0] = 0; im.data[1] = 0; im.data[2] = 255;
    
    cv::cvtColor(im ,imyuv0, CV_BGR2YUV_I420);
    cv::cvtColor(im ,imyuv, CV_BGR2YUV_I420);
    cv::cvtColor(im, imgray, CV_BGR2GRAY);

    uchar *data = imyuv.data;
    //draw_rect_yuv(data, im.cols, im.rows, 200, 200,200,400);

    int imgw = im.cols;
    int imgh = im.rows;
    memset(imyuv.data, 0, imgh*imgw*3/2);
    memcpy(imyuv.data, imyuv0.data, imgw*imgh);

    unsigned char *uvdatau = imyuv0.data+imgh*imgw;
    unsigned char *uvdatav = imyuv0.data+imgh*imgw+imgh*imgw/4;
    for (int i=0; i<imgh/2; i++) {
        unsigned char *uvdata = imyuv.data+imgh*imgw+i*imgw;
        for (int j=0; j<imgw/2; j++) {
            uvdata[j*2] = uvdatau[i*imgw/2+j];
            uvdata[j*2+1] = uvdatav[i*imgw/2+j];
        }
    }
    draw_rect_yuv(data, im.cols, im.rows, 0, 100,150,250);

    cv::cvtColor(imyuv, im2, CV_YUV420sp2RGB);

    cv::imshow("image",im2);

    cv::waitKey(0);


	return 0;
}

int draw_rect_yuv(unsigned char *data, int imgw, int imgh, int x, int y, int w, int h){
    if (x<0 || y<0 || x+w>=imgw || y+h>=imgh || w<=4 || h<=4) {
        return -1;
    }
    int line_color[3] = {29, 103, 239};
    int line_width = 2;
    unsigned char *tlstart = data + y*imgw+x;

    //two horizontal lines, y channels
        memset(tlstart, line_color[0], w);
        memset(tlstart+imgw, line_color[0], w);
        memset(tlstart+(h-1)*imgw, line_color[0], w);
        memset(tlstart+h*imgw, line_color[0], w);

    //two horizontal lines, uv
    int halfw = imgw/2;
    int halfh = imgh/2;
    int halfrw = w/2;
    int halfrh = h/2;
    //for (int i=0; i<halfh; i++) {
        tlstart = data+imgw*imgh + y/2 * imgw+x;
        for (int j=0; j<w/2; j++) {
            tlstart[j*2] = line_color[1];
            tlstart[j*2+1] = line_color[2];
        }  
        memcpy(tlstart+imgw, tlstart, w);
    //}
        tlstart = data+imgw*imgh + (y+h)/2 * imgw+x;
        for (int j=0; j<w/2; j++) {
            tlstart[j*2] = line_color[1];
            tlstart[j*2+1] = line_color[2];
        }
        memcpy(tlstart-imgw, tlstart, w);

    unsigned char *tlstarty = data + y*imgw+x;
    unsigned char *tlstartuv = data+ imgw*imgh + y/2*imgw+x;

    //two vertical lines, y
    for (int i=0; i<halfrh; i++) {
        tlstart[x] = line_color[0];
        tlstart[x+1] = line_color[0];
        tlstart[x+w-1] = line_color[0];
        tlstart[x+w-2] = line_color[0];
    }
    //two vertical lines, uv
    for (int i=0; i<halfrh; i++) {
        tlstartuv[i*imgw] = line_color[1];
        tlstartuv[i*imgw+1] = line_color[2];
        tlstartuv[i*imgw+2] = line_color[1];
        tlstartuv[i*imgw+1+2] = line_color[2];
        tlstartuv[i*imgw+w] = line_color[1];
        tlstartuv[i*imgw+w+1] = line_color[2];
        tlstartuv[i*imgw+w-2] = line_color[1];
        tlstartuv[i*imgw+w+1-2] = line_color[2];

    }
    return 0;
}

