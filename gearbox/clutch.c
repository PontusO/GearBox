/*
 * gearbox - An open source gearbox controller
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
#include <string.h>
#include <stdio.h>

#define CLUTCH_ENGAGED_POSITION     430
#define CLUTCH_DISENGAGED_POSITION  113
#define CLUTCH_ENGAGED_SWITCH       P1_6
#define CLUTCH_DISENGAGED_SWITCH    P1_5

static int clutch_position;
static unsigned char clutch_state;
signal_t CLUTCH_ENGAGE;
signal_t CLUTCH_DISENGAGE;

void set_clutch_servo (int position)
{
  clutch_position = position;
}

/*
 * This function simply returns the state of the clutch
 */
unsigned char get_clutch_state (void)
{
  return (clutch_state);
}

unsigned char clutch_is_disengaged(void)
{
  if (clutch_state == CLUTCH_DISENGAGED)
    return 1;
  else
    return 0;
}

unsigned char clutch_is_engaged(void)
{
  if (clutch_state == CLUTCH_ENGAGED)
    return 1;
  else
    return 0;
}

/*
 * Initialize the clutch controller. Observe that only one
 * instance of this function may be running at anyone time.
 */
PT_FACTORY_INIT_START(clutch)
{
  CLUTCH_ENGAGE = 0;
  CLUTCH_DISENGAGE = 0;
  clutch_state = CLUTCH_DISENGAGED;
  set_clutch_servo (CLUTCH_DISENGAGED_POSITION);
}
PT_FACTORY_INIT_END(clutch)

PT_THREAD(thread_clutch(pt_clutch *clutch))
{
  PT_BEGIN (&clutch->pt);

  printf ("Starting the clutch thread !\n");

  while (1) {
    /* Wait for the gb controller to engage the clutch */
    PT_WAIT_WHILE (&clutch->pt, !CLUTCH_ENGAGE);

    /* Start engaging clutch servo as fast as the servo will allow */
    set_clutch_servo (CLUTCH_ENGAGED_POSITION);
    clutch_state = CLUTCH_ENGAGING;

    /* Wait for the clutch engaged feedback switch to be activated */
    PT_WAIT_WHILE (&clutch->pt, !CLUTCH_ENGAGED_SWITCH);
    clutch_state = CLUTCH_ENGAGED;
    CLUTCH_ENGAGE = 0;

    /* Wait for the gb controller to disengage the clutch */
    PT_WAIT_WHILE (&clutch->pt, !CLUTCH_DISENGAGE);

    /* Start disengaging clutch servo as fast as the servo will allow */
    set_clutch_servo (CLUTCH_DISENGAGED_POSITION);
    clutch_state = CLUTCH_DISENGAGING;

    /* Wait for the clutch disengaged feedback switch to be activated */
    PT_WAIT_WHILE (&clutch->pt, !CLUTCH_DISENGAGED_SWITCH);
    clutch_state = CLUTCH_DISENGAGED;
    CLUTCH_DISENGAGE = 0;
  }

  PT_END (&clutch->pt);
}
