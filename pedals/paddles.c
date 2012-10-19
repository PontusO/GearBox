/*
 * paddles.c - Part of an open source gearbox controller
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

#include "system/system.h"
#include "pedals/paddles.h"
#include "gearbox/controller.h"

/* Paddle button inputs
 * Inputs are active low.
 * If other input pins are used the PADDLE_MASK must match the selected input
 * pair. For instance if bit 0 and 2 is used the mask must be changed to 0x05.
 */
#define PADDLE_UP             0x01
#define PADDLE_DOWN           0x02
#define PADDLE_NEUTRAL        0x00

#define UP_PADDLE             P2_0
#define DOWN_PADDLE           P2_1
#define PADDLE_PORT           P2
#define PADDLE_MASK           0x03
#define PADDLES_INACTIVE      0

/* A 50 msec debounce timeout */
#define DEBOUNCE_TIMEOUT      50

/* Dummy timer functions */
unsigned char timer_alloc(void)
{
  return 0;
}
void set_timer(unsigned char timer, unsigned int timeout)
{
  IDENTIFIER_NOT_USED(timer);
  IDENTIFIER_NOT_USED(timeout);
}
unsigned int get_timer(unsigned char timer)
{
  return timer * 10;
}
void init_paddles(pt_paddles *paddles)
{
  PT_INIT (&paddles->pt);
  PADDLE_PORT = 0;
}

/* The paddle loop
 *
 * The paddles can perform one out of three different actions at any given time.
 * The up paddle can be pressed to give the SHIFT_UP command, the down paddle
 * can be pressed to give the SHIFT_DOWN command and both paddles can be pressed
 * at the same time to give the SHIFT_NEUTRAL command. No other possibilities
 * are available.
 */
PT_THREAD(thread_paddles(pt_paddles *paddles))
{
  PT_BEGIN (&paddles->pt);

  paddles->timer = timer_alloc();

  while (1) {
restart:
    /* Wait for an event to happen on the paddle inputs */
    PT_WAIT_UNTIL (&paddles->pt,
                   (paddles->paddlestate = PADDLE_PORT & PADDLE_MASK) != PADDLES_INACTIVE);

    /*
     * First we need to do some light debouncing. High quality buttons must be
     *     used so that this algorithm can be a short as possible.
     */
    set_timer(paddles->timer, DEBOUNCE_TIMEOUT);
    PT_WAIT_WHILE(&paddles->pt, get_timer(paddles->timer) != 0 &&
                  (paddles->paddlestatenew = PADDLE_PORT & PADDLE_MASK) == paddles->paddlestate);

    /* 'kay, lets check why we got here */
    if (get_timer(paddles->timer) != 0) {
      /* The paddle state has changed, could it be a shift neutral event */
      if (!paddles->paddlestatenew) {
        /* It is, Wait for the specified time to lock in the neutral command */
        paddles->paddlestate = paddles->paddlestatenew;
        set_timer(paddles->timer, DEBOUNCE_TIMEOUT);
        PT_WAIT_WHILE(&paddles->pt, get_timer(paddles->timer) != 0 &&
                      (paddles->paddlestatenew = PADDLE_PORT & PADDLE_MASK) == paddles->paddlestate);

        /* Make sure we have waited the hole timeout to confirm that the paddles
         * have not changed state */
        if (get_timer(paddles->timer) != 0)
          goto restart;

        /* Shift to neutral detected */
        request_shift_sequence(&controller, SHIFT_NEUTRAL);
      } else {
        /* Not valid, go and start over */
        goto restart;
      }
    } else {
      /* Right, since we ended up here, the paddle event is valid */
      switch (paddles->paddlestatenew ^ PADDLE_MASK) {
      case PADDLE_UP:
        request_shift_sequence(&controller, SHIFT_UP);
        break;

      case PADDLE_DOWN:
        request_shift_sequence(&controller, SHIFT_DOWN);
        break;

      case PADDLE_NEUTRAL:
        request_shift_sequence(&controller, SHIFT_NEUTRAL);
        break;

      default:
        /* Create and entry in the error log */
        break;
      }
    }
  } /* While (1) */

  PT_END (&paddles->pt);
}
