#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <stdint.h>
#include <jaglib.h>

#define FALSE 0
#define TRUE !FALSE

extern uint64_t *listbufshadow;
extern uint64_t *listbuf;

extern op_bmp_object_scaled *scaledBmp;
extern op_bmp_object *gameoverbmp;
extern op_bmp_object *youwinbmp;
extern op_branch_object *tilesbranch;

#endif