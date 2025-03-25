#include "geoMetric.h"
void geoMetric::renderRectangle(const geoMetricFormat& geo_size , const colorData& color_data , SDL_Renderer* gScreen){
    SDL_Rect fill_rect = {geo_size.x_ , geo_size.y_ , geo_size.w_ , geo_size.h_};
    SDL_SetRenderDrawColor(gScreen,color_data.red_ , color_data.green_ , color_data.blue_ ,0xff);
    SDL_RenderFillRect(gScreen,&fill_rect);
}

void geoMetric::renderOutline(const geoMetricFormat& geo_size , const colorData& color_data , SDL_Renderer* gScreen){
    SDL_Rect outRect =  {geo_size.x_ , geo_size.y_ , geo_size.w_ , geo_size.h_};
    SDL_SetRenderDrawColor(gScreen,color_data.red_ , color_data.green_ , color_data.blue_ ,0xff);
    SDL_RenderDrawRect(gScreen,&outRect);
}
