/*
 * controller.h - Part of an open source gearbox controller
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
#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#include <pt/pt-sem.h>

/**
 * List of commands that will be accepted by the controller
 */
enum command_list {
  SHIFT_UP = 0x01,
  SHIFT_DOWN,
  SHIFT_NEUTRAL,
};

/**
 * Command structure for sequencing a gear shift. Due to the simplicity of
 * protothreads we need to have the command data placed somewhere. In this case
 * we have placed it with the controller, the recipient.
 */
struct shift_cmd {
  unsigned char command;
  unsigned char gear;
};

/**
 * \struct _pt_controller
 * Instance data of the pt_controller thread
 */
struct _pt_controller {
  struct pt pt;           /**< Proto thread structure */
  struct pt_sem mutex;    /**< Mutex to protect shift data */
  struct shift_cmd cmd;   /**< Shift signal data */
  unsigned char gear;     /**< The current gear */
  char result;            /**< The result of the last rules evaluation */
};

/**
 * \hideinitializer
 */
typedef struct _pt_controller pt_controller;
extern pt_controller controller;

void init_controller(pt_controller *controller);
unsigned char
request_shift_sequence (pt_controller *controller, unsigned char command);
PT_THREAD(thread_controller(pt_controller *controller));
#endif // CONTROLLER_H_INCLUDED
