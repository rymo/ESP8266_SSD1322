/*********************************************************************
This is a library for our Monochrome OLEDs based on SSD1322 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

These displays use SPI to communicate, 4 or 5 pins are required to
interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#if ARDUINO >= 100
 #include "Arduino.h"
 #define WIRE_WRITE Wire.write
#else
 #include "WProgram.h"
  #define WIRE_WRITE Wire.send
#endif

#ifdef __SAM3X8E__
 typedef volatile RwReg PortReg;
 typedef uint32_t PortMask;
#else
  typedef volatile uint8_t PortReg;
  typedef uint8_t PortMask;
#endif

#include <SPI.h>
#include <Adafruit_GFX.h>

#define BLACK 0
#define WHITE 1
#define INVERSE 2

#define SSD1322_I2C_ADDRESS   0x3C	// 011110+SA0+RW - 0x3C or 0x3D
// Address for 128x32 is 0x3C
// Address for 128x64 is 0x3D (default) or 0x3C (if SA0 is grounded)

/*=========================================================================
    SSD1322 Displays
    -----------------------------------------------------------------------
    The driver is used in multiple displays (128x64, 128x32, etc.).
    Select the appropriate display below to create an appropriately
    sized framebuffer, etc.

    SSD1322_256_64  256x64 pixel display

    -----------------------------------------------------------------------*/
   #define SSD1322_256_64
/*=========================================================================*/

#if defined SSD1322_256_64
  #define SSD1322_LCDWIDTH                  256
//  #define SSD1322_LCDHEIGHT                 64
  #define SSD1322_LCDHEIGHT                 64
#endif

#define SSD1322_SETCOMMANDLOCK 0xFD
#define SSD1322_DISPLAYOFF 0xAE
#define SSD1322_DISPLAYON 0xAF
#define SSD1322_SETCLOCKDIVIDER 0xB3
#define SSD1322_SETDISPLAYOFFSET 0xA2
#define SSD1322_SETSTARTLINE 0xA1
#define SSD1322_SETREMAP 0xA0
#define SSD1322_FUNCTIONSEL 0xAB
#define SSD1322_DISPLAYENHANCE 0xB4
#define SSD1322_SETCONTRASTCURRENT 0xC1
#define SSD1322_MASTERCURRENTCONTROL 0xC7
#define SSD1322_SETPHASELENGTH 0xB1
#define SSD1322_DISPLAYENHANCEB 0xD1
#define SSD1322_SETPRECHARGEVOLTAGE 0xBB
#define SSD1322_SETSECONDPRECHARGEPERIOD 0xB6
#define SSD1322_SETVCOMH 0xBE
#define SSD1322_NORMALDISPLAY 0xA6
#define SSD1322_INVERSEDISPLAY 0xA7
#define SSD1322_SETMUXRATIO 0xCA
#define SSD1322_SETCOLUMNADDR 0x15
#define SSD1322_SETROWADDR 0x75
#define SSD1322_WRITERAM 0x5C
#define SSD1322_ENTIREDISPLAYON 0xA5
#define SSD1322_ENTIREDISPLAYOFF 0xA4
#define SSD1322_SETGPIO 0xB5
#define SSD1322_EXITPARTIALDISPLAY 0xA9
#define SSD1322_SELECTDEFAULTGRAYSCALE 0xB9



#define MIN_SEG	0x1C
#define MAX_SEG	0x5B

// Scrolling #defines
#define SSD1322_ACTIVATE_SCROLL 0x2F
#define SSD1322_DEACTIVATE_SCROLL 0x2E
#define SSD1322_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1322_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1322_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1322_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1322_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

class Adafruit_SSD1322 : public Adafruit_GFX {
 public:
  Adafruit_SSD1322(int8_t SID, int8_t SCLK, int8_t DC, int8_t RST, int8_t CS);
  Adafruit_SSD1322(int8_t DC, int8_t RST, int8_t CS);
  Adafruit_SSD1322(int8_t RST);

  void begin(uint8_t i2caddr = SSD1322_I2C_ADDRESS, bool reset=true);
  void ssd1322_command(uint8_t c);
  void ssd1322_data(uint8_t c);
  void ssd1322_data32(uint32_t c);


  void clearDisplay(void);
  void invertDisplay(uint8_t i);
  void display(boolean clear = true);

  void startscrollright(uint8_t start, uint8_t stop);
  void startscrollleft(uint8_t start, uint8_t stop);

  void startscrolldiagright(uint8_t start, uint8_t stop);
  void startscrolldiagleft(uint8_t start, uint8_t stop);
  void stopscroll(void);

  void dim(boolean dim);

  void drawPixel(int16_t x, int16_t y, uint16_t color);

  void fill(uint8_t colour);

  void fastDrawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t color);

  virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

//  void drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color);
//  void drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color);

 private:
  int8_t _i2caddr, sid, sclk, dc, rst, cs;
  void fastSPIwrite(uint8_t c);
  void fastSPIwrite32(uint32_t d);



  boolean hwSPI;
  PortReg *mosiport, *clkport, *csport, *dcport;
  PortMask mosipinmask, clkpinmask, cspinmask, dcpinmask;

  inline void drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color) __attribute__((always_inline));
  inline void drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) __attribute__((always_inline));

};
