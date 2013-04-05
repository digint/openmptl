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
#include <arch/core.hpp>

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
class Lcd
{
protected:
  static constexpr unsigned lcdbuf_size = ((res_x * res_y) / 8); /* 1 bit per pixel */
  unsigned char lcdbuf[lcdbuf_size];

private:

  unsigned buf_index(unsigned x, unsigned y) {
    return (x * res_y + y * res_y * columns) / 8;
  };

public:

  using font = LcdFont<font_width>;

  static constexpr unsigned resolution_x = res_x;
  static constexpr unsigned resolution_y = res_y;
  static constexpr unsigned rows = res_y / 8;
  static constexpr unsigned columns = res_x / font_width;

  void clear(void) {
    for (unsigned i = 0; i < lcdbuf_size; i++) {
      lcdbuf[i] = 0;
    }
  }

  void draw_char(unsigned x, unsigned y, const unsigned char ch) {
    if(x > columns)
      return;
    if(y > rows)
      return;

    font::copy_char(&lcdbuf[buf_index(x, y)], ch);
  }

  void draw_char_inverse(unsigned x, unsigned y, const unsigned char ch) {
    if(x > columns)
      return;
    if(y > rows)
      return;

    font::copy_char_inv(&lcdbuf[buf_index(x, y)], ch);
  }

  void print_line(unsigned row, const char *p, bool inv = false ) {
    unsigned x = 0;
    while(*p) {
      if(inv) {
        draw_char_inverse(x, row, (const unsigned char)(*p));
      }
      else {
        draw_char(x, row, (const unsigned char)(*p));
      }
      x++;
      p++;
    }
  }

  void print_line_inv(unsigned row, const char *p ) { print_line(row, p, true); }

#if 0
  void scroll_up(void) {
    memmove(lcdbuf, &lcdbuf[buf_index(0, 1)], lcdbuf_size - columns);
    for(unsigned i = lcdbuf_size - columns; i < lcdbuf_size; i++)
      lcdbuf[i] = 0;
  }
#endif

  virtual void update(void) = 0;
};


/////  NOKIA 3310  /////


template<typename spi_type,
         typename lcd_ds,   //< data / command selection
         typename lcd_reset,
         typename lcd_e
         >
class Lcd_Nokia3310 : public Lcd< 84, 48 >
{
  struct DeviceConfig {
    static constexpr SpiMasterSelection         master_selection = SpiMasterSelection::master;

    static constexpr freq_t                     max_frequency    = 4_mhz;
    static constexpr unsigned                   data_size        = 8;
    static constexpr SpiClockPolarity           clk_pol          = SpiClockPolarity::high;
    static constexpr SpiClockPhase              clk_phase        = SpiClockPhase::second_edge;
    static constexpr SpiDataDirection           data_dir         = SpiDataDirection::one_line_tx;
    static constexpr SpiSoftwareSlaveManagement ssm              = SpiSoftwareSlaveManagement::enabled;
    static constexpr SpiFrameFormat             frame_format     = SpiFrameFormat::msb_first;
  };

  using spi_device = SpiDevice< spi_type, DeviceConfig >;

  static void enable_slave_select(void) {
    lcd_e::enable();
  }
  static void disable_slave_select(void) {
    spi_device::wait_not_busy();
    lcd_e::disable();
  }

  static void select_data() {
    spi_device::wait_not_busy();
    lcd_ds::set(); /* select data */
  }

  static void select_command() {
    spi_device::wait_not_busy();
    lcd_ds::reset(); /* select command */
  }

  static void send_byte(unsigned char data) {
    spi_device::send_blocking(data);
  }

public:

  using resources = ResourceList<
    typename spi_device::resources,
    typename lcd_ds::resources,
    typename lcd_reset::resources,
    typename lcd_e::resources
    >;

  static void enable(void) {
    spi_device().reconfigure();
  }

  void update(void) {
    enable_slave_select();

    /*  Set base address X=0 Y=0 */
    select_command();
    send_byte(0x80);  // set X address of RAM
    send_byte(0x40);  // set Y address of RAM
    // send_byte(0x08 | 0x5);   // display control: inverse video mode

    /* Serialize the video buffer */
    select_data();
    for (unsigned i = 0; i < lcdbuf_size; i++) {
      send_byte(lcdbuf[i]);
    }
    disable_slave_select();
  }

  /* 127 <= value <= 0 */
  static void set_contrast(unsigned char value) {
    enable_slave_select();
    select_command();
    send_byte(0x20 | 0x1);    // function set: use extended instruction set
    send_byte(0x80 | value);  // set Vop (contrast)
    send_byte(0x20 | 0x0);    // function set: use basic instruction set, horizontal addressing
    disable_slave_select();
  }

  static void init(unsigned char contrast, unsigned char temp_coeff, unsigned char bias)
  {
    contrast   &= 0x7f;
    temp_coeff &= 0x03;
    bias       &= 0x07;

    lcd_ds::set();
    lcd_e::disable();
    lcd_reset::disable();

    /* toggle display reset pin */
    lcd_reset::enable();
    Core::nop(10000);
    lcd_reset::disable();
    Core::nop(10000);

    enable();

    /* command sequence (init) */
    enable_slave_select();
    select_command();
    send_byte(0x20 | 0x1);        // function set: use extended instruction set
    send_byte(0x80 | contrast);   // set Vop (contrast)
    send_byte(0x04 | temp_coeff); // set temperature coefficent
    send_byte(0x10 | bias);       // set bias system (BSx)
    send_byte(0x20 | 0x0);        // function set: use basic instruction set, horizontal addressing
    send_byte(0x08 | 0x0);        // display control: display blank
    send_byte(0x08 | 0x4);        // display control: normal mode
    disable_slave_select();
  }

  static constexpr unsigned char default_contrast   = 0x45;
  static constexpr unsigned char default_temp_coeff = 0x2;
  static constexpr unsigned char default_bias       = 0x3;  // 1:48

  static void init(void) {
    init(default_contrast, default_temp_coeff, default_bias);
  }
};

#endif // LCD_HPP_INCLUDED
