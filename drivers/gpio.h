/*
 * gpio.h - An open source gearbox controller
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
#ifndef GPIO_H_INCLUDED
#define GPIO_H_INCLUDED

enum GPIO_DIRECTION {
  GPIO_INPUT,
  GPIO_OUTPUT
};

/* These typedefs need to be changed depending on CPU architecture */
typedef unsigned char       gpio_dir;
typedef unsigned char       gpio_port;
typedef unsigned char       port_addr;

/**
 * This function will set the direction of the specified pin of the selected
 * port.
 *
 * \param port The port containing the desired pin
 * \param pin  The pin that you want to control
 * \param dir  The direction of the pin (GPIO_INPUT or GPIO_OUTPUT).
 */
void set_gpio_dir(port_addr *port, unsigned char pin, gpio_dir dir);

/**
 * This function reads a specified port and returns the masked value.
 *
 * \param port  The selected port to read from
 * \param mask  A mask that is used before returning the value to caller. Use
 *              0xff (on an 8 bit machine) to get the entire port value.
 */
gpio_port get_port(port_addr *port, gpio_port mask);

/**
 * This function reads the specified port pin and returns the value.
 *
 * \param port  The selected port to read from
 * \param pin   The selected pin that you want to read.
 */
unsigned char get_port_pin(port_addr *port, unsigned char pin);

/**
 * Function to set an entire port to a specified value.
 *
 * \param port  The port that is to be configured
 * \param value The value the port shall be configured with.
 */
void set_port(port_addr *port, gpio_port value);

/**
 * Sets a specific pin to a value.
 *
 * \param port  The port that shall be altered.
 * \param pin   The pin in question.
 * \param value The value that should be written to the pin.
 */
void set_port_pin(port_addr *port, unsigned char pin, unsigned char value);

#endif // GPIO_H_INCLUDED
