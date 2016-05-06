### Econotag ###

The Econotag is an open hardware development kit based on the FreeScale MC13224 ARM7 802.15.4 System on Chip. Amongst other things, it provides 96KB of RAM and 128KB of flash and a variety of I/O.

![econotag-r2-bottom](econotag-r2-bottom.jpg "econotag-r2-bottom")
![econotag-r2-top](econotag-r2-top.jpg "econotag-r2-top")

The MC13224v is an ARM7 microcontroller with integrated 802.15.4 radio and Open Source development environment hosted at The MC1322X Project. The Econotag combines this with an FT2232H to provide access to UART1 and JTAG; using an econotag it is possible to load, flash, and debug code on the MC13224v "hands-free" using only a USB connection to the computer.

The econotag breaks out all of the important pins on the MC13224v to allow development using all of its features. It also has flexible power supply options and can easily be modified for very low power operation (less than 2uA sleep currents are possible with a modified econotag).

The econotag is well supported in Contiki. The following is a short list of Contiki examples that are known to work:

  *  Radio layer (nullmac/nullrdc with 802.15.4 autoacks)
  *  RIME
  *  6lowpan
  *  RPL
  *  REST/CoAP

Some links to econotag wiki pages:

  ~~~
 [[Econotag RPL border router with Windows]]
 [[Econotag Debugging with OpenOCD]]
 [[Mobile Econotag]]
 [[Managing Multiple Econotags]]
  ~~~
***
