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

#include "screenitem.hpp"
#include "printf.h"

const char * DataRow::c_str() const {
  sprintf(buf, "%4s: %08u", name, value); /* strlen = 14 */
  return buf;
}
const char * DataRowHex::c_str() const {
  sprintf(buf, "%4s: %08x", name, value); /* strlen = 14 */
  return buf;
}

char DataRow::buf[15]; /* used for DataRow and DataRowHex */

