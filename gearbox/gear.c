/*
 * gear.c - Part of an open source gearbox controller
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
#include <pt/pt.h>
#include <gearbox/gear.h>

/* The port where the sense switches are connected */
#define SENSE_PORT    P3

#define FORK_NEUTRAL  0
#define SLIDE_NEUTRAL 0

/**
 * Gear position indicator switches:
 *
 *  1   3   5              front position
 *
 *  !   !   !
 *  !   !   !
 *  -------------  <- N    center position
 *  !   !   !   !
 *  !   !   !   !
 *
 *  2   4   6   R          rear position
 *
 *  Bit 0 (0x01) = fork indicator front position
 *  Bit 1 (0x02) = fork indicator rear position
 *  Bit 2 (0x04) = fork indicator center position
 *  Bit 3 (0x08) = (Not used)
 *  Bit 4 (0x10) = slide indicator leftmost position
 *  Bit 5 (0x20) = slide indicator center position
 *  Bit 6 (0x40) = slide indicator rightmost position
 *  Bit 7 (0x80) = slide indicator "reverse" position
 *
 * This version of the gear shift controller is based on using switches that
 * sense the position of the gear servos. This is probably not a very good
 * solution, instead a rotary wheel encoder could be used to indicate the exact
 * position of the servo arm. Which would increase the resolution of the
 * controller and simplify the installation greatly.
 *
 * But for a proof of concept this will have to do.
 *
 */
#define NEUTRAL_POS   0x04
#define FORK_POS_1    0x01
#define FORK_POS_2    0x02
#define FORK_POS_3    0x04
unsigned char shift_fork_switch[] = {
  NEUTRAL_POS, FORK_POS_1, FORK_POS_2, FORK_POS_1,
  FORK_POS_2,  FORK_POS_1, FORK_POS_2, FORK_POS_2
};
#define SLIDE_POS_1   0x10
#define SLIDE_POS_2   0x20
#define SLIDE_POS_3   0x40
#define SLIDE_POS_4   0x80
unsigned char shift_slide_switch[] = {
  NEUTRAL_POS, SLIDE_POS_1, SLIDE_POS_1, SLIDE_POS_2,
  SLIDE_POS_2, SLIDE_POS_3, SLIDE_POS_3, SLIDE_POS_4
};
unsigned int shift_fork[] = {
  495, 975, 114, 975, 114, 975, 114, 975
};
unsigned int shift_slide[] = {
  452, 132, 132, 452, 452, 987, 987, 1200
};

signal_t GEAR_SHIFT;

static unsigned char servo_num;
static unsigned char servo_val;
static void set_shift_servo (unsigned char num, unsigned char val)
{
  servo_num = num;
  servo_val = val;
}

/**
 * This function evaluates the current shift positions and returns the sequential
 * representation of this gear. If there was an error where a valid selected
 * gear could not be found, this function will return -1.
 */
char pos_to_num (pt_gear *gear)
{
  unsigned char i;

  IDENTIFIER_NOT_USED(gear);

  for (i=0 ; i<MAX_GEARS ; i++) {
    if (SENSE_PORT & shift_fork_switch[i] && SENSE_PORT & shift_slide_switch[i])
      break;
  }
  if (i <= MAX_GEARS)
    return i;
  else
    return -1;
}

/**
 * Initialize the gear shift controller
 */
void init_gear(pt_gear *gear)
{
  char mgear;

  PT_INIT (&gear->pt);

  /* Evaluate the current state of the gear box */
  if ((mgear = pos_to_num(gear)) < 0) {
    /* No valid state was found so put it in neutral */
    set_shift_servo (0, shift_fork_switch[FORK_NEUTRAL]);
    set_shift_servo (1, shift_slide_switch[SLIDE_NEUTRAL]);
    gear = 0;
  }
  gear->gear_ctrl.gear = mgear;
  gear->gear_ctrl.state = READY;
}

/*
 * return the number of the highest gear
 */
unsigned char get_max_gear(void)
{
  return MAX_GEARS;
}

/*
 * The gear process
 */
PT_THREAD(thread_gear(pt_gear *gear))
{
  PT_BEGIN (&gear->pt);

  PT_WAIT_WHILE (&gear->pt, GEAR_SHIFT);

  if (gear->gear_ctrl.gear > MAX_GEARS) {
    gear->gear_ctrl.state = ERROR;
    PT_RESTART(&gear->pt);
  }
  gear->gear_ctrl.state = SHIFTING;

  /* Now start shifting the gear */
  gear->mask[0] = shift_fork_switch[gear->gear_ctrl.gear];
  /* If the gear lever is not in place we need to set it */
  if (!(SENSE_PORT & gear->mask[0]))
    set_shift_servo (0, shift_fork[gear->gear_ctrl.gear]);

  gear->mask[1] = shift_slide_switch[gear->gear_ctrl.gear];
  if (!(SENSE_PORT & gear->mask[1]))
    set_shift_servo (1, shift_slide[gear->gear_ctrl.gear]);

  /* Wait for both actuators to settle into place */
  PT_WAIT_UNTIL (&gear->pt, (SENSE_PORT & gear->mask[0]) &&
      (SENSE_PORT & gear->mask[1]));
  gear->gear_ctrl.state = READY;

  PT_END (&gear->pt);
}
