/*
 * paddles.h - Part of an open source gearbox controller
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
#ifndef PADDLES_H_INCLUDED
#define PADDLES_H_INCLUDED

/**
 * The main thread data structure.
 */
struct _pt_paddles {
  struct pt pt;                 /**< proto thread data structure */
  unsigned char timer;          /**< Debounce timer */
  unsigned char paddlestate;    /**< Saved state of the paddles */
  unsigned char paddlestatenew; /**< Last known paddle state */
};

/**
 * Definition of the pt_paddles type.
 */
typedef struct _pt_paddles pt_paddles;

/* Export the external API */
/**
 * Initialization of the paddle controller thread.
 *
 * \param paddles The instance that need to be initialized
 */
void init_paddles(pt_paddles *paddles);

/**
 * The thread loop
 *
 * \param paddles The instance that is to be scheduled
 */
PT_THREAD(thread_paddles(pt_paddles *paddles));

#endif // PADDLES_H_INCLUDED
