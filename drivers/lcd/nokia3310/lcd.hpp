/*
 * CppCore - C++ microprocessor core library
 *
 * Copyright 2012 Axel Burri <axel@tty0.ch>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef LCD_HPP_INCLUDED
#define LCD_HPP_INCLUDED

#include "arch/gpio.hpp"
#include "arch/spi.hpp"



/* NOTE: font_width is the font INCLUDING character separator (e.g. use 6 for 8x5 font) */
template<unsigned font_width>
class LcdFont
{
private:
  static constexpr unsigned char_width = font_width - 1;

  static const unsigned char font[128 - 32][char_width]; /* chars from 32 - 128 */
  static const unsigned char font_illegal_char[char_width];

  static const unsigned char * char_ptr(const char ch) {
    if(ch < 32)
      return font_illegal_char;
    return font[ch - 32];
  }

public:

  static void copy_char(unsigned char *dst, const char ch) {
    const unsigned char * font_char = char_ptr(ch);
    for(unsigned i = 0; i < char_width; i++) {
      *dst++ = font_char[i];
    }
    *dst = 0x00;
  }

  static void copy_char_inv(unsigned char *dst, const char ch) {
    const unsigned char * font_char = char_ptr(ch);
    for(unsigned i = 0; i < char_width; i++) {
      *dst++ = ~(font_char[i]);
    }
    *dst = 0xff;
  }
};


template< unsigned res_x,
          unsigned res_y,
          unsigned font_width = 6 /* NOTE: actual font width is 8x5, one bit is for character separator */
          >
class Lcd : protected LcdFont<font_width>
{
protected:
  static constexpr unsigned lcdbuf_size = ((res_x * res_y) / 8); /* 1 bit per pixel */
  unsigned char lcdbuf[lcdbuf_size];

private:

  unsigned buf_index(unsigned x, unsigned y) {
    return (x * res_y + y * res_y * columns) / 8;
  };

public:

  static constexpr unsigned resolution_x = res_x;
  static constexpr unsigned resolution_y = res_y;
  static constexpr unsigned rows = res_y / 8;
  static constexpr unsigned columns = res_x / font_width;

  void clear(void) {
    for (unsigned i = 0; i < lcdbuf_size; i++) {
      lcdbuf[i] = 0;
    }
  }

  void drawChar(unsigned x, unsigned y, const unsigned char ch) {
    if(x > columns) return; // TODO: exception
    if(y > rows) return;

    LcdFont<font_width>::copy_char(&lcdbuf[buf_index(x, y)], ch);
  }

  void drawCharInverse(unsigned x, unsigned y, const unsigned char ch) {
    if(x > columns) return; // TODO: exception
    if(y > rows) return;

    LcdFont<font_width>::copy_char_inv(&lcdbuf[buf_index(x, y)], ch);
  }

  void printLine(unsigned row, const char *p, bool inv = false ) {
    unsigned x = 0;
    while(*p) {
      if(inv) {
        drawCharInverse(x, row, (const unsigned char)(*p));
      }
      else {
        drawChar(x, row, (const unsigned char)(*p));
      }
      x++;
      p++;
    }
  }

  //  void printLine(int row, const char *p ) { printLine(row, p, false); }
  void printLineInv(unsigned row, const char *p ) { printLine(row, p, true); }

#if 0
  void scrollUp(void) {
    memmove(lcdbuf, &lcdbuf[buf_index(0, 1)], lcdbuf_size - columns);
    for(unsigned i = lcdbuf_size - columns; i < lcdbuf_size; i++)
      lcdbuf[i] = 0;
  }
#endif

  virtual void update(void) = 0; /* pure virtual function */
};



/////  NOKIA 3310  /////

template<int spi_no,
         typename lcd_ds,   //< data / command selection
         typename lcd_reset,
         typename lcd_e
         >
class Lcd_Nokia3310 : public Lcd< 84, 48 >
{

  static void sendChar(unsigned char data) {
    lcd_ds::set(); /* select data */
    spi_master::sendByte(data);
  }
  static void sendCommand(unsigned char data) {
    lcd_ds::reset(); /* select command */
    spi_master::sendByte(data);
  }

public:

  typedef SpiMaster<spi_no,
                    4_mhz,  // max_frequency = 4 MHz
                    8,        // 8bit data
                    cSpi::ClockPolarity::high,
                    cSpi::ClockPhase::second_edge> spi_master;

  typedef ResourceList< typename spi_master::resources,
                        typename lcd_ds::resources,
                        typename lcd_reset::resources,
                        typename lcd_e::resources
                        > resources;

  static void configure(void) {
    spi_master::configure();
  }

  // Enable slave select
  static void enable(void) {
    lcd_e::enable();
  }
  // Disable slave select
  static void disable(void) {
    lcd_e::disable();
  }

  void update(void) {  /* implement pure virtual function from class Lcd */
    enable();
    //  Set base address X=0 Y=0
    sendCommand(0x80);
    sendCommand(0x40);

    lcd_ds::set(); /* select data */
    //  Serialize the video buffer.
    for (unsigned i = 0; i < lcdbuf_size; i++) {
      spi_master::sendByte(lcdbuf[i]);
    }
    disable();
  }

  static void setContrast(unsigned char value) {
    enable();
    sendCommand(0x21);          // LCD Extended Commands.
    sendCommand(0x80 | value);  // Set LCD Vop (Contrast).
    sendCommand(0x20);          // LCD Standard Commands, horizontal addressing mode.
    disable();
  }

  static void init(void) {
    spi_master::init();

    lcd_ds::init();
    lcd_ds::set();    // D/C high

    lcd_reset::init();
    lcd_e::init();
    lcd_e::disable();
    lcd_reset::disable();

    // Toggle display reset pin.
    lcd_reset::enable();
    Core::nop(10000);
    lcd_reset::disable();
    Core::nop(10000);

    // Send sequence of commands
    enable();
    sendCommand(0x21);  // LCD Extended Commands.
    sendCommand(0xC8);  // Set LCD Vop (Contrast).
    sendCommand(0x06);  // Set Temp coefficent.
    sendCommand(0x13);  // LCD bias mode 1:48.
    sendCommand(0x20);  // LCD Standard Commands, Horizontal addressing mode.
    sendCommand(0x08);  // LCD blank
    sendCommand(0x0C);  // LCD in normal mode.
    disable();

    // Clear and Update
    //    clear();
    //    update();
  }
};

#endif // LCD_HPP_INCLUDED
