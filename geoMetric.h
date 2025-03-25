#ifndef GEOMETRIC_H
#define GEOMETRIC_H
#include "CommonFunc.h"
typedef struct geoMetricFormat{
public:
    geoMetricFormat(int x , int y , int w , int h){
        x_ = x;
        y_ = y;
        w_ = w;
        h_ = h;
    }
    int x_ ;
    int y_ ;
    int w_ ;
    int h_ ;

};
typedef struct colorData{
    colorData(Uint8 r,Uint8 g , Uint8 b){
        red_ = r;
        green_ = g;
        blue_ = b ;
    }
public:
    Uint8 red_;
    Uint8 green_ ;
    Uint8 blue_ ;
};
class geoMetric{
public:
     static void renderRectangle(const geoMetricFormat& geo_size , const colorData& color_data , SDL_Renderer* gScreen);
     static void renderOutline(const geoMetricFormat& geo_size , const colorData& color_data , SDL_Renderer* gScreen);
};
#endif // GEOMETRIC_H
