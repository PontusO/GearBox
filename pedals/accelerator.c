/*
 * accelerator.c - Part of an open source gearbox controller
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
#include <float.h>

#include "system/system.h"
#include "pedals/accelerator.h"

#define ACC_SERVO_MAX       (float)455
#define ACC_SERVO_MIN       (float)233
#define ACC_PEDAL_MAX       (float)2900
#define ACC_PEDAL_MIN       (float)987

static unsigned int accelerator_servo;

void init_accelerator(pt_accelerator *accelerator)
{
  PT_INIT (&accelerator->pt);
}

/*
 * Map the accelerator pot values to the servo. This is a simple
 * linearisation between two straight lines.
 */
static unsigned int calc_accelerator (unsigned int value)
{
  float faktor, val, result;

  val = (float)value;
  /* Scaling factor */
  faktor = (ACC_SERVO_MAX - ACC_SERVO_MIN) / (ACC_PEDAL_MAX - ACC_PEDAL_MIN);
  /* Normalizing, scaling and adjustment to output */
  result = (val - ACC_PEDAL_MIN) * faktor + ACC_SERVO_MIN;

  return (unsigned int)result;
}

/*
 * Set accelerator servo
 */
static void set_accelerator_servo (unsigned int value)
{
  accelerator_servo = value;
}

/*
 * read the value of the accelerator pedal, it returns the averaged
 * value from the A/D driver.
 */
static unsigned int read_accelerator (void)
{
  return P0; /* Take the averaged value from the A/D driver */
}

PT_THREAD(thread_accelerator(pt_accelerator *accelerator))
{
  PT_BEGIN (&accelerator->pt);

  accelerator->accelerator = read_accelerator();
  accelerator->servo = calc_accelerator (accelerator->accelerator);
  set_accelerator_servo(accelerator->servo);

  /* We really want to do a small delay here, since it is not necessary
   * to do this extremely fast. For now we only yield. */
  PT_YIELD (&accelerator->pt);

  PT_END (&accelerator->pt);
}
