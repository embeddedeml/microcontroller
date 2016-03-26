/***************************************************
 This is an Arduino Library for the Adafruit 2.2" SPI display.
 This library works with the Adafruit 2.2" TFT Breakout w/SD card
 ----> http://www.adafruit.com/products/1480
 
 Check out the links above for our tutorials and wiring diagrams
 These displays use SPI to communicate, 4 or 5 pins are required to
 interface (RST is optional)
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries.
 MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "LCD_ILI9341.h"
#include "Dio.h"
#include <util/delay.h>

// Constructor when using software SPI.  All output pins are configurable.
LCD_ILI9341::LCD_ILI9341(uint8_t dc, uint8_t rst, uint8_t cs) :
    LCD_GFX(ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT)
{
  _dc = dc;
  _rst = rst;
  _cs = cs;
}

void LCD_ILI9341::setSpi(Spi *spi)
{
  _spi = spi;
}

void LCD_ILI9341::spiwrite(uint8_t c)
{
  _spi->transmitSync(c);
}

void LCD_ILI9341::writecommand(uint8_t c)
{
  digitalWrite(_dc, LOW);
  digitalWrite(_cs, LOW);

  spiwrite(c);

  digitalWrite(_cs, HIGH);
}

void LCD_ILI9341::writedata(uint8_t c)
{
  digitalWrite(_dc, HIGH);
  digitalWrite(_cs, LOW);

  spiwrite(c);

  digitalWrite(_cs, HIGH);
}

// Rather than a bazillion writecommand() and writedata() calls, screen
// initialization commands and arguments are organized in these tables
// stored in PROGMEM.  The table may look bulky, but that's mostly the
// formatting -- storage-wise this is hundreds of bytes more compact
// than the equivalent code.  Companion function follows.
#define DELAY 0x80

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void LCD_ILI9341::commandList(uint8_t *addr)
{

  uint8_t numCommands, numArgs;
  uint16_t ms;

  numCommands = pgm_read_byte(addr++);   // Number of commands to follow
  while(numCommands--)
  {                 // For each command...
    writecommand(pgm_read_byte(addr++)); //   Read, issue command
    numArgs = pgm_read_byte(addr++);    //   Number of args to follow
    ms = numArgs & DELAY;          //   If hibit set, delay follows args
    numArgs &= ~DELAY;                   //   Mask out delay bit
    while(numArgs--)
    {                   //   For each argument...
      writedata(pgm_read_byte(addr++));  //     Read, issue argument
    }

    if(ms)
    {
      ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
      if(ms == 255)
        ms = 500;     // If 255, delay for 500 ms
      for(uint8_t i = 0; i < ms; i++)
      {
        _delay_ms(1);
      }
    }
  }
}

void LCD_ILI9341::begin(void)
{
  pinMode(_rst, OUTPUT);
  digitalWrite(_rst, LOW);
  pinMode(_dc, OUTPUT);
  pinMode(_cs, OUTPUT);

  // toggle RST low to reset
  digitalWrite(_rst, HIGH);
  _delay_ms(5);
  digitalWrite(_rst, LOW);
  _delay_ms(20);
  digitalWrite(_rst, HIGH);
  _delay_ms(150);

  writecommand(0xEF);
  writedata(0x03);
  writedata(0x80);
  writedata(0x02);

  writecommand(0xCF);
  writedata(0x00);
  writedata(0XC1);
  writedata(0X30);

  writecommand(0xED);
  writedata(0x64);
  writedata(0x03);
  writedata(0X12);
  writedata(0X81);

  writecommand(0xE8);
  writedata(0x85);
  writedata(0x00);
  writedata(0x78);

  writecommand(0xCB);
  writedata(0x39);
  writedata(0x2C);
  writedata(0x00);
  writedata(0x34);
  writedata(0x02);

  writecommand(0xF7);
  writedata(0x20);

  writecommand(0xEA);
  writedata(0x00);
  writedata(0x00);

  writecommand(ILI9341_PWCTR1);    //Power control
  writedata(0x23);   //VRH[5:0]

  writecommand(ILI9341_PWCTR2);    //Power control
  writedata(0x10);   //SAP[2:0];BT[3:0]

  writecommand(ILI9341_VMCTR1);    //VCM control
  writedata(0x3e); //¶Ô±È¶Èµ÷½Ú
  writedata(0x28);

  writecommand(ILI9341_VMCTR2);    //VCM control2
  writedata(0x86);  //--

  writecommand(ILI9341_MADCTL);    // Memory Access Control
  writedata(0x48);

  writecommand(ILI9341_PIXFMT);
  writedata(0x55);

  writecommand(ILI9341_FRMCTR1);
  writedata(0x00);
  writedata(0x18);

  writecommand(ILI9341_DFUNCTR);    // Display Function Control
  writedata(0x08);
  writedata(0x82);
  writedata(0x27);

  writecommand(0xF2);    // 3Gamma Function Disable
  writedata(0x00);

  writecommand(ILI9341_GAMMASET);    //Gamma curve selected
  writedata(0x01);

  writecommand(ILI9341_GMCTRP1);    //Set Gamma
  writedata(0x0F);
  writedata(0x31);
  writedata(0x2B);
  writedata(0x0C);
  writedata(0x0E);
  writedata(0x08);
  writedata(0x4E);
  writedata(0xF1);
  writedata(0x37);
  writedata(0x07);
  writedata(0x10);
  writedata(0x03);
  writedata(0x0E);
  writedata(0x09);
  writedata(0x00);

  writecommand(ILI9341_GMCTRN1);    //Set Gamma
  writedata(0x00);
  writedata(0x0E);
  writedata(0x14);
  writedata(0x03);
  writedata(0x11);
  writedata(0x07);
  writedata(0x31);
  writedata(0xC1);
  writedata(0x48);
  writedata(0x08);
  writedata(0x0F);
  writedata(0x0C);
  writedata(0x31);
  writedata(0x36);
  writedata(0x0F);

  writecommand(ILI9341_SLPOUT);    //Exit Sleep
  _delay_ms(120);
  writecommand(ILI9341_DISPON);    //Display on
}

void LCD_ILI9341::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{

  writecommand(ILI9341_CASET); // Column addr set
  writedata(x0 >> 8);
  writedata(x0 & 0xFF);     // XSTART 
  writedata(x1 >> 8);
  writedata(x1 & 0xFF);     // XEND

  writecommand(ILI9341_PASET); // Row addr set
  writedata(y0 >> 8);
  writedata(y0);     // YSTART
  writedata(y1 >> 8);
  writedata(y1);     // YEND

  writecommand(ILI9341_RAMWR); // write to RAM
}

void LCD_ILI9341::pushColor(uint16_t color)
{
  digitalWrite(_dc, HIGH);
  digitalWrite(_cs, LOW);

  spiwrite(color >> 8);
  spiwrite(color);

  digitalWrite(_cs, HIGH);
}

void LCD_ILI9341::drawPixel(int16_t x, int16_t y, uint16_t color)
{

  if((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
    return;

  setAddrWindow(x, y, x + 1, y + 1);

  digitalWrite(_dc, HIGH);
  digitalWrite(_cs, LOW);

  spiwrite(color >> 8);
  spiwrite(color);

  digitalWrite(_cs, HIGH);
}

void LCD_ILI9341::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{

  // Rudimentary clipping
  if((x >= _width) || (y >= _height))
    return;

  if((y + h - 1) >= _height)
    h = _height - y;

  setAddrWindow(x, y, x, y + h - 1);

  uint8_t hi = color >> 8, lo = color;

  digitalWrite(_dc, HIGH);
  digitalWrite(_cs, LOW);

  while(h--)
  {
    spiwrite(hi);
    spiwrite(lo);
  }

  digitalWrite(_cs, HIGH);
}

void LCD_ILI9341::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{

  // Rudimentary clipping
  if((x >= _width) || (y >= _height))
    return;
  if((x + w - 1) >= _width)
    w = _width - x;
  setAddrWindow(x, y, x + w - 1, y);

  uint8_t hi = color >> 8, lo = color;
  digitalWrite(_dc, HIGH);
  digitalWrite(_cs, LOW);
  while(w--)
  {
    spiwrite(hi);
    spiwrite(lo);
  }

  digitalWrite(_cs, HIGH);
}

void LCD_ILI9341::fillScreen(uint16_t color)
{
  fillRect(0, 0, _width, _height, color);
}

// fill a rectangle
void LCD_ILI9341::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{

  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= _width) || (y >= _height))
    return;
  if((x + w - 1) >= _width)
    w = _width - x;
  if((y + h - 1) >= _height)
    h = _height - y;

  setAddrWindow(x, y, x + w - 1, y + h - 1);

  uint8_t hi = color >> 8, lo = color;

  digitalWrite(_dc, HIGH);
  digitalWrite(_cs, LOW);

  for(y = h; y > 0; y--)
  {
    for(x = w; x > 0; x--)
    {
      spiwrite(hi);
      spiwrite(lo);
    }
  }
  digitalWrite(_cs, HIGH);
}

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t LCD_ILI9341::Color565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void LCD_ILI9341::setRotation(uint8_t m)
{

  writecommand(ILI9341_MADCTL);
  rotation = m % 4; // can't be higher than 3
  switch(rotation)
  {
  case 0:
    writedata(MADCTL_MX | MADCTL_BGR);
    _width = ILI9341_TFTWIDTH;
    _height = ILI9341_TFTHEIGHT;
    break;
  case 1:
    writedata(MADCTL_MV | MADCTL_BGR);
    _width = ILI9341_TFTHEIGHT;
    _height = ILI9341_TFTWIDTH;
    break;
  case 2:
    writedata(MADCTL_MY | MADCTL_BGR);
    _width = ILI9341_TFTWIDTH;
    _height = ILI9341_TFTHEIGHT;
    break;
  case 3:
    writedata(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
    _width = ILI9341_TFTHEIGHT;
    _height = ILI9341_TFTWIDTH;
    break;
  }
}

void LCD_ILI9341::invertDisplay(boolean i)
{
  writecommand(i ? ILI9341_INVON : ILI9341_INVOFF);
}
