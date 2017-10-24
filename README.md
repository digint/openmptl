Introduction
============

OpenMPTL is a C++ template library for lowlevel access to
microprocessor registers and peripherals. It provides classes for
generic register access and peripheral resources management, as well
as a simulation framework. It is fast and deterministic, ideal for
real-time operating systems.

Key Features:

  * Simple and comprehensive declaration and usage of microprocessor
    register classes
  * Framework for generic resources declaration
    (e.g. auto-configuration of peripheral enable registers)
  * Automatic generation of interrupt handler tables from resources
  * Compile-Time errors on resource conflicts or wrong register access
  * Supports bit-band access to register bits for faster access times
  * Simulation framework, showing all lowlevel register access and
    allowing to implement reactions to specific register bits.


OpenMPTL benefits from the C++11 template metaprogramming features, is
an header-only library and does not depend on RTTI, exceptions or any
external library.

Official home page: <https://digint.ch/openmptl>


Supported Architechtures
========================

OpenMPTL provides generic functionality for the ARM Cortex family, as
well as specific implementations for STM32 microcontrollers.

## openmptl-reg-arm-cortex

Register declarations for many ARM Cortex microcontrollers are
available, but not yet streamlined:

<https://github.com/digint/openmptl-reg-arm-cortex>


Documentation
=============

The documentation is provided within the source code. The [API
Reference Documentation] is available for online browsing.


**Recommended starting points for high-level usage**:

  * `projects/stm32f4discovery-ledtest/src/ledtest.cpp`
  * `projects/stm32f4discovery/src/kernel.hpp`
  * `projects/stm32f4discovery/src/kernel.cpp`
  * `projects/stm32f4discovery/src/startup.hpp`
  * `drivers/joystick/stm32f103stk/joystick.hpp`

**Recommended starting points for low-level usage**:

  * `arch/arm/cortex/stm32/common/usart.hpp`
  * `include/register.hpp`
  * `include/register_access.hpp`

  [API Reference Documentation]: https://digint.ch/openmptl/doc/api/openmptl/index.html


### Example usage (high level)

    #include <arch/gpio.hpp>
    using led_green = mptl::gpio_led< 'D', 12 >;
    led_green::on();


### Example usage (low level)

    #include <arch/reg/spi.hpp>
    using SPIx = SPI<1>;
    auto reg = SPIx::CR1::load();
    reg &= SPIx::CR1::SPE::value | SPIx::CR1::CRCNEXT::value | SPIx::CR1::CRCEN::value;
    SPIx::CR1::store(reg);

A **load()** call results in:

    constexpr volatile T * value_ptr = reinterpret_cast<volatile T *>(addr);
    auto reg = *value_ptr;

A **store()** call results in:

    *value_ptr = reg;

Where:

  * `addr` (constexpr) holds the non-pointer-type address of SPIx::CR1
  * `T` is the register type (e.g. uint32_t or uint8_t)


Simulation Framework
--------------------

The simulation framework allows you to run embedded code in a
simulated environment. It traces every register access and allows you
to react on specific bit changes.

### Example (ledtest)

    # cd projects/stm32f4discovery-ledtest
    # make SIMULATION=1
    # ./stm32f4discovery-ledtest
    <...>
    RCC::CR         ::bitset()    0x00010000   [ ........ .......1 ........ ........ ]
    [INFO] reacting to RCC::CR::HSEON, by setting RCC::CR::HSERDY
    RCC::CR         ##bitset()    0x00020000   [ ........ ......1. ........ ........ ]
    <...>
    RCC::AHB1ENR    ::bitset()    0x00000008   [ ........ ........ ........ ....1... ]
    GPIOD::MODER    ::load()      0x00000000   [ 00000000 00000000 00000000 00000000 ]
    GPIOD::MODER    ::store()     0x01000000   [ 00000001 00000000 00000000 00000000 ]
    GPIOD::BSRR     ::store()     0x00001000   [ 00000000 00000000 00010000 00000000 ]

### Example (stm32f103stk-demo)

    # cd projects/stm32f103stk-demo
    # make SIMULATION=1
    # ../../bin/sim_terminal.sh ./stm32f103stk-demo
    <...>
    Welcome to OpenMPTL terminal console!
    # help
    List of commands:
       cpuid      prints SCB::CPUID register hexadecimal value
       heap       allocate (leak) 1k on heap
       nrf        test the NRF24L01 chip (spi)


Folder Hierarchy
================

  * `arch`: Architecture dependent template library
  * `bin`: Helper applications
  * `config`: Makefiles, included by projects
  * `doc`: doxygen documentation build root
  * `drivers`: Drivers for specific peripherals
  * `include`: Main OpenMPTL class declarations
  * `lib`: Auxiliary libraries
  * `projects`: Example projects
    * `stm32f103stk-demo`: Example project for the [stm32f103stk] evaluation board
    * `stm32f4discovery`: Example project for the [stm32f4discovery] evaluation board
    * `stm32f4discovery-ledtest`: Simple example for the [stm32f4discovery] evaluation board
    * `unittest`: OpenMPTL unit tests
  * `sim`: Compilation units for simulation framework

  [stm32f103stk]: https://www.olimex.com/Products/ARM/ST/STM32-103STK/
  [stm32f4discovery]: http://www.st.com/en/evaluation-tools/stm32f4discovery.html


Development
===========

Source Code Repository
----------------------

The source code for OpenMPTL is managed using Git.

Official repository:

    git clone https://dev.tty0.ch/openmptl.git

Mirror on GitHub:

    git clone https://github.com/digint/openmptl.git


How to Contribute
-----------------

Your contributions are welcome!

If you would like to contribute or have found bugs:

  * Visit the [OpenMPTL project page on GitHub] and use the
    [issues tracker] there.
  * Talk to us on Freenode in `#openmptl`.
  * Contact the author via email (the email address can be found in
    the sources).

Any feedback is appreciated!

  [OpenMPTL project page on GitHub]: http://github.com/digint/openmptl
  [issues tracker]: http://github.com/digint/openmptl/issues


License
=======

OpenMPTL is free software, available under the [GNU General Public
License, Version 3][GPLv3].

Some register definition files in `arch/*/reg` contain derivative
representations of CMSIS System View Description (SVD) files, and are
subject to the "End User License Agreement for STMicroelectronics"
(see "STM_License.html" in the containing directory).

  [GPLv3]: http://www.gnu.org/licenses/gpl.html
