/*
 * gpio.c - An open source gearbox controller
 *
 * Copyright (C) 2010  Pontus Oldberg
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
#include <system/system.h>
#include "gpio.h"
/*
 * This function will set the direction of the specified pin of the selected
 * port.
 */
void set_gpio_dir(port_addr *port, unsigned char pin, gpio_dir dir)
{
  IDENTIFIER_NOT_USED(port);
  IDENTIFIER_NOT_USED(pin);
  IDENTIFIER_NOT_USED(dir);
}

/*
 * This function reads a specified port and returns the masked value.
 */
gpio_port get_port(port_addr *port, gpio_port mask)
{
  return *port & mask;
}

/*
 * This function reads the specified port pin and returns the value.
 */
unsigned char get_port_pin(port_addr *port, unsigned char pin)
{
  return (*port >> pin) & 0x01;
}

/*
 * Function to set an entire port to a specified value.
 */
void set_port(port_addr *port, gpio_port value)
{
  IDENTIFIER_NOT_USED(port);
  IDENTIFIER_NOT_USED(value);
}

/*
 * Sets a specific pin to a value.
 */
void set_port_pin(port_addr *port, unsigned char pin, unsigned char value)
{
  IDENTIFIER_NOT_USED(port);
  IDENTIFIER_NOT_USED(pin);
  IDENTIFIER_NOT_USED(value);
}
