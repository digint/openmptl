/*
 * OpenMPTL - C++ Microprocessor Template Library
 *
 * Copyright (C) 2012-2017 Axel Burri <axel@tty0.ch>
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

#ifndef FIFO_HPP_INCLUDED
#define FIFO_HPP_INCLUDED

#include <atomic>

namespace mptl {

template< typename Tp >
class fifo {
public:
  using char_type = Tp;

#if 0 // virtuals are not needed for now. disabled.
  virtual bool push(char_type c) = 0;
  virtual bool pushs(const char_type * c) = 0;
  virtual bool pop(char_type &c) = 0;
#endif
};


/* Lockfree implementation (single reader, single writer)
 *
 * We use acquire/release mode to protect the read and write index
 * variables. All we need here is a "happens-before" relationship to
 * dependent variables.
 * see: <http://gcc.gnu.org/wiki/Atomic/GCCMM/AtomicSync>
 */
template< typename Tp, unsigned int size >
class ring_buffer
: public fifo< Tp >
{
  std::atomic<unsigned int> atomic_write_index;
  std::atomic<unsigned int> atomic_read_index;
  Tp buf[size];

  unsigned int increment(unsigned int arg) {
    unsigned int ret = arg + 1;
    if(ret >= size)
      ret = 0;
    return ret;
  }

public:

#if 0 // no constructor. read "why constructors suck for static member variables of template classes" (TODO)
  ring_buffer() {
    SIM_TRACE("*** ring_buffer() ***");
    clear();
  }
#endif

  /**
   * NOTE: This function is not thread-safe. Make sure to call it
   * while no consumer/producer is accessing the fifo!
   */
  void reset(void) {
    atomic_write_index.store(0, std::memory_order_relaxed);
    atomic_read_index.store(0, std::memory_order_relaxed);
  }

  bool push(Tp data) {
    /* producer only: updates write_index after writing */
    unsigned int write_index = atomic_write_index.load(std::memory_order_acquire);

    unsigned int next_write_index = increment(write_index);
    if(next_write_index == atomic_read_index.load(std::memory_order_acquire)) {
      return false; /* buffer full */
    }

    buf[write_index] = data;
    atomic_write_index.store(next_write_index, std::memory_order_release);
    return true;
  }

  bool pop(Tp &data) {
    /* Consumer only: updates read_index after reading */
    unsigned int write_index = atomic_write_index.load(std::memory_order_acquire);
    unsigned int read_index = atomic_read_index.load(std::memory_order_acquire);

    if(read_index == write_index) {
      return false;  /* buffer empty */
    }

    data = buf[read_index];
    unsigned int next_read_index = increment(read_index);
    atomic_read_index.store(next_read_index, std::memory_order_release);
    return true;
  }

  /* NOTE: this does write nothing if the string does not fit into the buffer! */
  bool pushs(const Tp * data) {
    /* producer only: updates write_index after writing */
    unsigned int write_index = atomic_write_index.load(std::memory_order_acquire);
    unsigned int read_index = atomic_read_index.load(std::memory_order_acquire);

    unsigned int i = 0;
    while(data[i]) {
      unsigned int next_write_index = increment(write_index);
      if(next_write_index == read_index) {
        /* no update of atomic_write_index. all the copying before is discarded! */
        return false; /* buffer full */
      }
      buf[write_index] = data[i];
      write_index = next_write_index;
      i++;
    }

    atomic_write_index.store(write_index, std::memory_order_release);
    return true;
  }

  /* NOTE: this does write nothing if the string does not fit into the buffer! */
  bool pushs(const Tp * data, unsigned int count) {
    /* producer only: updates write_index after writing */
    unsigned int write_index = atomic_write_index.load(std::memory_order_acquire);
    unsigned int read_index = atomic_read_index.load(std::memory_order_acquire);

    unsigned int i = 0;
    while(i < count) {
      unsigned int next_write_index = increment(write_index);
      if(next_write_index == read_index) {
        /* no update of atomic_write_index. all the copying before is discarded! */
        return false; /* buffer full */
      }
      buf[write_index] = data[i];
      write_index = next_write_index;
      i++;
    }

    atomic_write_index.store(write_index, std::memory_order_release);
    return true;
  }

#if 0

  int read_available() {
    if (write_index >= read_index)
      return write_index - read_index;
    return write_index + size - read_index;
  }

  int write_available() {
    int ret = read_index - write_index - 1;
    if (write_index >= read_index)
      ret += size;
    return ret;
  }
#endif
};



template< typename Tp, unsigned int size >
class counted_ring_buffer
: public ring_buffer< Tp, size >
{
  unsigned int total;
  unsigned int overrun;
  unsigned int underrun;

  using base_type = ring_buffer< Tp, size >;

public:
#if 0 // no constructor. read "why constructors suck for static member variables of template classes" (TODO)
  counted_ring_buffer() : ring_buffer<Tp, size>(), overrun(0), underrun(0) {}
#endif

  void reset_counter(void) {
    total = 0;
    overrun = 0;
    underrun = 0;
  };

  void reset(void) {
    base_type::reset();
    reset_counter();
  }

  bool push(Tp c) {
    if(base_type::push(c) == false) {
      overrun++;
      return false;
    }
    total++;
    return true;
  }

  bool pop(Tp &c) {
    if(base_type::pop(c) == false) {
      underrun++;
      return false;
    }
    return true;
  }

  unsigned int get_total(void) { return total; }
  unsigned int get_overrun(void) { return overrun; }
  unsigned int get_underrun(void) { return underrun; }
};

} // namespace mptl

#endif // FIFO_HPP_INCLUDED
