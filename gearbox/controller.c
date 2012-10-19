/*
 * controller.c - Part of an open source gearbox controller
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
#include <gearbox/controller.h>
#include <rules/rules.h>
#include <gearbox/clutch.h>
#include <gearbox/gear.h>

signal_t  DO_SHIFT;

void init_controller(pt_controller *controller)
{
  PT_INIT (&controller->pt);
  /* Use semaphore as mutex, so init to 1 */
  PT_SEM_INIT (&controller->mutex, 1);
}

/*
 * Function for requesting the controller to do a gear shift.
 */
unsigned char request_shift_sequence (pt_controller *controller,
  unsigned char command)
{
  PT_BEGIN (&controller->pt);

  PT_SEM_WAIT (&controller->pt, &controller->mutex);
  {
    controller->cmd.command = command;
    SEND_SIG (DO_SHIFT);
  }
  PT_SEM_SIGNAL (&controller->pt, &controller->mutex);

  PT_EXIT (&controller->pt);
  PT_END (&controller->pt);
}
/*
 * This is the main gear shift controller process. It reacts on one signal
 * comming from the input trigger process. The signal is a simple one bit
 * signal simply meaning "do the shift". A control structure determines what
 * kind of shift sequence that should be performed. Three shift commands are
 * available, SHIFT_UP, SHIFT_DOWN and SHIFT_NEUTRAL.
 * The controller process main purpose is to translate shift commands from the
 * input trigger process to gear commands that the pt_gear process understands,
 * but it is also responisble for determining the legality of incoming shift
 * commands. For instance, a user shall not be able to downshift into first gear
 * when storming down the straight in over 200 km/h. This would destroy the
 * gearbox immediatly.
 * The rules engine is responsible for evaluating all input parameters such as
 * rpm, speed, current gear, wanted gear, pedal status etc
 */
PT_THREAD(thread_controller(pt_controller *controller))
{
  PT_BEGIN (&controller->pt);

  /* The main loop for this thread */
  while (1) {
restart:
    /* Wait for a gear shift event */
    PT_WAIT_UNTIL (&controller->pt, DO_SHIFT);

    /* So, someone told us to do a gear shift, so we need to evaluate it first */
    /* First lock out further shift attempts */
    PT_SEM_WAIT (&controller->pt, &controller->mutex);
    controller->result = evalute_rules (controller);
    PT_SEM_SIGNAL (&controller->pt, &controller->mutex);
    /* Check that the rules engine think it's a good idea to shift now */
    if (controller->result < 0)
      goto restart;

    /* It's obviously ok to go ahead and do the shift now */
    /* Start by disengaging the clutch */
    SEND_SIG (CLUTCH_DISENGAGE);

    /* Now wait for the clutch controller to report that the clutch has
       disengaged fully and we can proceed with the shift process */
    PT_WAIT_UNTIL (&controller->pt, clutch_is_disengaged());

    /* Now we can go ahead with configuring and setting the new gear */
    if (controller->cmd.command == SHIFT_UP) {
      /* Requesting to shift gear up */
      if (controller->gear < get_max_gear())
        controller->gear++;
    } else if (controller->cmd.command == SHIFT_DOWN) {
      // if (controller->gear
    }
  }

  PT_END (&controller->pt);
}
