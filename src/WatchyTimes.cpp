// Based on the project https://github.com/spinalcode/Mickey

#include "WatchyTimes.h"
#include <stdlib.h>     //srand, rand
#include <math.h>
#include "subheadlines.h"

const float LOW_VOLTAGE = 3.7;

const char *lows [10] = {"ZERO", "ONE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE"};
const char *teensone [11] = {"","TEN", "ELEVEN", "TWELVE", "THIR", "FOUR", "FIF", "SIX", "SEVEN", "EIGHT", "NINE"};
const char *teenstwo [11] = {"", "","", "TEEN", "TEEN", "TEEN", "TEEN", "TEEN", "TEEN", "TEEN", "TEEN"};
const char *tens [10] = {"ZERO", "TEN", "TWENTY", "THIRTY", "FORTY", "FIFTY", "SIXTY", "SEVENTY", "EIGHTY", "NINETY"};

const int16_t articleImage_x1 = 4;
const int16_t articleImage_y1 = 107;
const int16_t articleBitmap_w = 67;
const int16_t articleBitmap_h = 88;

WatchyTimes::WatchyTimes(){} //constructor

void WatchyTimes::drawWatchFace()
{
   drawMyRect(0, 0, 200, 200, GxEPD_WHITE);
   drawBitmapCol(0, 0, epd_bitmap_newspaper, 200, 200, GxEPD_BLACK);

   // Draw Headline (Time)

      // Hour
      display.setFont( &Helvetica_LT_Black11pt7b );
      display.setTextColor(GxEPD_BLACK);

      int16_t  x1, y1;
      uint16_t w, h;
      String hourString = "IT'S ";
      String minuteString;
      int curHour = currentTime.Hour;
      
      if ( curHour > 12 ) curHour -= 12;

      if ( curHour < 10 )
      {
          //display.print(lows[curHour]);
          hourString += lows[curHour];
      }
      else if ( curHour < 20 ) 
      {
        hourString += teensone[curHour-9];
      }  
      //display.print( hours[ curHour - 1 ] );

      display.getTextBounds( hourString, 0, 0, &x1, &y1, &w, &h);

      // center the text
      display.setCursor( 100 - w / 2, 61);
      display.print( hourString );

      // Minute

      if (currentTime.Minute == 0) 
      {
         minuteString = "O'CLOCK";
      } 
      else if (currentTime.Minute < 10) 
      {
          minuteString = "OH ";
          minuteString += lows[currentTime.Minute];
      } 
      else if (currentTime.Minute < 20) 
      {
        minuteString = teensone[currentTime.Minute-9];
        if (currentTime.Minute > 12) 
        {
            minuteString += teenstwo[currentTime.Minute%10];
        }
      } 
      else 
      {
        minuteString = tens[currentTime.Minute/10];
        minuteString += " ";
        if (currentTime.Minute%10 > 0) 
        {
          minuteString += lows[currentTime.Minute%10];
        }
      } 

      // center the text
      display.getTextBounds( minuteString, 0, 0, &x1, &y1, &w, &h);
      display.setCursor( 100 - w / 2, 81);

      display.print( minuteString );
      

   // Draw Date
      String dayOfWeek = dayShortStr( currentTime.Wday );
      dayOfWeek.toUpperCase();

      display.setFont( &Helvetica_LT_Black6pt7b );
      display.setTextColor(GxEPD_BLACK);
      display.setCursor(7, 17);
      display.print( dayOfWeek );
      display.setCursor(170, 17);
      display.print( currentTime.Day );

   // Draw random subheadline

      display.setFont( &Palatino_LT_Bold6pt7b );
      display.setTextColor(GxEPD_BLACK);

      int lines = (sizeof(subheadlines) / sizeof(subheadlines[0]));

      srand( currentTime.Minute * currentTime.Hour * currentTime.Wday );
      int randLine;

      randLine = rand() % lines;

      //Serial.println( lines );
      //Serial.println( randLine );

      display.getTextBounds( subheadlines[randLine], 0, 0, &x1, &y1, &w, &h);
      display.setCursor( 100 - w / 2, 98);
      display.print( subheadlines[randLine] );

      // Draw Article headline

      float volt = getBatteryVoltage();

      if ( volt <= LOW_VOLTAGE )
         drawBattery();
      else
         drawWeather();
      
}

void WatchyTimes::drawWeather()
{
   weatherData currentWeather = getWeatherData();
   int8_t temperature = currentWeather.temperature;
   int16_t weatherConditionCode = currentWeather.weatherConditionCode;   

  //weatherConditionCode = 500;

   //Serial.println("WeatherCode" + currentWeather.weatherConditionCode);

   display.setFont( &Palatino_LT_Bold6pt7b );
   display.setTextColor(GxEPD_BLACK);
   display.setCursor(74, 114);

   drawMyRect( articleImage_x1, articleImage_y1, articleBitmap_w, articleBitmap_h, GxEPD_WHITE);

   if(weatherConditionCode > 801) //Cloudy
   {
      drawBitmapCol( articleImage_x1, articleImage_y1, epd_bitmap_cloudy,  articleBitmap_w, articleBitmap_h, GxEPD_BLACK);
      display.print("CLOUDY"); 
      display.setCursor(74, 125);
      display.print("BUT OK"); 
   }
   else if(weatherConditionCode == 801 ) //Few Clouds
   {
      drawBitmapCol( articleImage_x1, articleImage_y1, epd_bitmap_cloudy,  articleBitmap_w, articleBitmap_h, GxEPD_BLACK);
      display.print("SOME");
      display.setCursor(74, 125);
      display.print("CLOUDS"); 
   }
   else if(weatherConditionCode == 800 ) //Clear
   {
      drawBitmapCol( articleImage_x1, articleImage_y1, epd_bitmap_rain,  articleBitmap_w, articleBitmap_h, GxEPD_BLACK);
      display.print("NICE"); 
      display.setCursor(74, 125);
      display.print("AND"); 
      display.setCursor(74, 136);
      display.print("CLEAR");
   }
   else if(weatherConditionCode >= 700 ) //Atmosphere
   {
      drawBitmapCol( articleImage_x1, articleImage_y1, epd_bitmap_rain,  articleBitmap_w, articleBitmap_h, GxEPD_BLACK);
      display.print("MOODY ");
   }
   else if(weatherConditionCode >= 600 ) //Snow
   {
      drawBitmapCol( articleImage_x1, articleImage_y1, epd_bitmap_snow,  articleBitmap_w, articleBitmap_h, GxEPD_BLACK);
      display.print("LET");
      display.setCursor(74, 125);
      display.print("IT"); 
      display.setCursor(74, 136);
      display.print("SNOW");
   }
   else if(weatherConditionCode >= 500 ) //Rain
   {      
      drawBitmapCol( articleImage_x1, articleImage_y1, epd_bitmap_rain,  articleBitmap_w, articleBitmap_h, GxEPD_BLACK);
      display.print("STILL ");
      display.setCursor(74, 125);
      display.print("PURPLE"); 
      display.setCursor(74, 136);
      display.print("RAIN");
   }
   else if(weatherConditionCode >= 300 ) //Drizzle
   {
      // Draw Image
      drawBitmapCol( articleImage_x1, articleImage_y1, epd_bitmap_rain,  articleBitmap_w, articleBitmap_h, GxEPD_BLACK);
      display.print("DRIZZLE ");
      display.setCursor(74, 125);
      display.print("FOR"); 
      display.setCursor(74, 136);
      display.print("SHIZZLE");
   }
   else if(weatherConditionCode >= 200 ) //Thunderstorm
   {
      drawBitmapCol( articleImage_x1, articleImage_y1, epd_bitmap_rain,  articleBitmap_w, articleBitmap_h, GxEPD_BLACK);
      display.print("THUNDER ");
      display.setCursor(74, 125);
      display.print("CAT IS"); 
      display.setCursor(74, 136);
      display.print("BACK");
   }else
   {
      drawBitmapCol( articleImage_x1, articleImage_y1, epd_bitmap_rain,  articleBitmap_w, articleBitmap_h, GxEPD_BLACK);
      display.print("WIFI ");
      display.setCursor(74, 125);
      display.print("IS"); 
      display.setCursor(74, 136);
      display.print("GONE");
      return;
   }

    //display.println(temperature);
}

void WatchyTimes::drawBattery()
{
   display.setFont( &Palatino_LT_Bold6pt7b );
   display.setTextColor(GxEPD_BLACK);
   display.setCursor(74, 114);

   display.println( "LOW ON" );
   display.setCursor(74, 125);
   display.println( "JUICE" );

   // Show battery bitmap
   drawMyRect( articleImage_x1, articleImage_y1, articleBitmap_w, articleBitmap_h, GxEPD_WHITE);
   drawBitmapCol( articleImage_x1, articleImage_y1, epd_bitmap_battery,  articleBitmap_w, articleBitmap_h, GxEPD_BLACK);
}

void WatchyTimes::drawPixel( int16_t x, int16_t y, uint16_t col )
{
  display.drawPixel(x, y, col);
}

void WatchyTimes::drawBitmapCol( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color1 )
{
  

  int16_t i, j, byteWidth = ( w + 7 ) / 8;
  for( j=0; j<h; j++ ) 
  {
    for( i=0; i<w; i++ ) 
    {
      if(( pgm_read_byte( bitmap + j * byteWidth + i / 8 ) & ( 128 >> ( i & 7 ))) == 0 ) 
      {
        drawPixel(x+i, y+j, color1);
      }
    }
  }
}

int WatchyTimes::getPixel( int16_t x, int16_t y, const uint8_t *bitmap )
{
  int16_t imageWidth = pgm_read_byte( bitmap );
  int16_t byteWidth = ( imageWidth + 7 ) / 8;
  return ( pgm_read_byte( bitmap + 2 + y * byteWidth + x / 8 ) & ( 128 >> ( x & 7 ) ) );
}

void WatchyTimes::drawMyRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1)
{
  for(int j=0; j<h; j++) {
    for(int i=0; i<w; i++ ) {
      drawPixel(x+i, y+j, color1);
    }
  }  
}


