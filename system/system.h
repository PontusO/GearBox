/*
 * system.h - Part of an open source gearbox controller
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
#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <pt/pt.h>
#include <parameters/param_db.h>

/**
 * \addtogroup system
 * @{
 */

/**
 * \file
 * System settings and configurations
 * \author
 * Pontus oldberg <pontus.oldberg@invector.se>
 *
 */

/**
 * Control macro that allows an unused parameter in the input list.
 * <br>Usage:
 *
 * \code
 * void test (int a)
 * {
 *   IDENTIFIER_NOT_USED(a);
 * }
 * \endcode
 */
#define IDENTIFIER_NOT_USED(i)  (void)(i)

/**
 * Signal macro that sends a signal to the receiving process
 */
#define SEND_SIG(sig)  sig = 1;

#ifndef HOST
#include <C8051F020.h>

/**
 * Global signal type. Used to signal between threads.
 */
typedef char signal_t;

#else
/**
 * Global signal type. Used to signal between threads.
 */
typedef int signal_t;

struct bit_field {
  unsigned char b0:1;
  unsigned char b1:1;
  unsigned char b2:1;
  unsigned char b3:1;
  unsigned char b4:1;
  unsigned char b5:1;
  unsigned char b6:1;
  unsigned char b7:1;
};

union port {
  unsigned char P;
  struct bit_field b;
};

union port Port0;
#define P0    Port0.P
#define P0_0  Port0.b.b0
#define P0_1  Port0.b.b1
#define P0_2  Port0.b.b2
#define P0_3  Port0.b.b3
#define P0_4  Port0.b.b4
#define P0_5  Port0.b.b5
#define P0_6  Port0.b.b6
#define P0_7  Port0.b.b7

union port Port1;
#define P1    Port1.P
#define P1_0  Port1.b.b0
#define P1_1  Port1.b.b1
#define P1_2  Port1.b.b2
#define P1_3  Port1.b.b3
#define P1_4  Port1.b.b4
#define P1_5  Port1.b.b5
#define P1_6  Port1.b.b6
#define P1_7  Port1.b.b7

union port Port2;
#define P2    Port2.P
#define P2_0  Port2.b.b0
#define P2_1  Port2.b.b1
#define P2_2  Port2.b.b2
#define P2_3  Port2.b.b3
#define P2_4  Port2.b.b4
#define P2_5  Port2.b.b5
#define P2_6  Port2.b.b6
#define P2_7  Port2.b.b7

union port Port3;
#define P3    Port3.P
#define P3_0  Port3.b.b0
#define P3_1  Port3.b.b1
#define P3_2  Port3.b.b2
#define P3_3  Port3.b.b3
#define P3_4  Port3.b.b4
#define P3_5  Port3.b.b5
#define P3_6  Port3.b.b6
#define P3_7  Port3.b.b7

#endif

/** @} */

#endif // SYSTEM_H_INCLUDED
