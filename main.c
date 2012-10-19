/*
 * main.c -
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

#include <system.h>
#include <gearbox/clutch.h>
#include <gearbox/gear.h>
#include <pedals/accelerator.h>
#include <pedals/paddles.h>
#include <parameters/parameters.h>
#include <gearbox/controller.h>
#include <stdio.h>

/* PT threads */
pt_clutch       clutch;
pt_gear         gear;
pt_accelerator  accelerator;
pt_paddles      paddles;
pt_controller   controller;

int main()
{
  int even = 1;
  unsigned short val = 700;
  int i = 1;
  /* Do basic hw and system initialization */

  /* Initialize the system */
  init_parameters();

  /* Initialize pt threads, all hw must be initialized before this point */
  init_clutch(&clutch);
  init_gear(&gear);
  init_accelerator(&accelerator);
  init_paddles(&paddles);
  init_controller(&controller);

  PID_Initiate();

  printf ("Starting main loop !\n");
  while (1) {
    PT_SCHEDULE(thread_clutch(&clutch));
    thread_clutch(&clutch);
    PT_SCHEDULE(thread_gear(&gear));
    PT_SCHEDULE(thread_accelerator(&accelerator));
    PT_SCHEDULE(thread_controller(&controller));
    PT_SCHEDULE(thread_paddles(&paddles));
    /*                 Cur  Tar  P  I  D  Mul */
    val = PID_Regulate(val, 800, 9, 9, 2, 10);
    printf ("Iteration: %04d - Value %d\n", i++, val);
    sleep (1);
  }

#ifdef HOST
  return 0; /* Just to keep the compiler happy */
#endif
}
