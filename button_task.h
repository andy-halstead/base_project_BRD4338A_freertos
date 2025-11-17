/*
 * button_task.h
 *
 *  Created on: 16 Nov 2025
 *      Author: anhalste
 */

#ifndef BUTTON_TASK_H_
#define BUTTON_TASK_H_

#define BUTTON_EVENT_ALL                  ( BUTTON_EVENT_BUTTON0_PRESSED \
                                          | BUTTON_EVENT_BUTTON1_PRESSED )

                                          
#define BUTTON_EVENT_BUTTON0_PRESSED     0x1
#define BUTTON_EVENT_BUTTON1_PRESSED     0x2


void button_task_init(void);

#endif /* BUTTON_TASK_H_ */
