#ifndef WATCHYTIMES_H
#define WATCHYTIMES_H

#include <Watchy.h>
#include "bitmaps.h"
#include "FontHeadline.h"
#include "FontDate.h"
#include "FontSubheadline.h"

class WatchyTimes : public Watchy
{
    public:
        WatchyTimes();
        void drawWatchFace();
        void drawWeather();
        void drawBattery();
        void drawPixel( int16_t x, int16_t y, uint16_t col );
        void drawBitmapCol( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color1 );
        int getPixel( int16_t x, int16_t y, const uint8_t *bitmap );
        void drawMyRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1);
};

#endif
