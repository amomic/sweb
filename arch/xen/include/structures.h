//----------------------------------------------------------------------
//  $Id: structures.h,v 1.1 2005/07/31 18:34:10 nightcreature Exp $
//----------------------------------------------------------------------
//
//  $Log: structures.h,v $
//
//----------------------------------------------------------------------

#ifndef _STRUCTUTES_H_
#define _STRUCTUTES_H_

#include "types.h"

typedef struct
{
   /* pushed by pusha */
   uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
   
   /* pushed separately */
   uint32 ds, es, fs, gs;
   uint32 which_int, err_code;
   
   /* pushed by exception. Exception may also push err_code.
   user_esp and user_ss are pushed only if a privilege change occurs. */
   uint32 eip, cs, eflags, user_esp, user_ss;
} regs_t __attribute__((__packed__));

typedef struct
{
  uint32 access_byte, eip;
} vector_t __attribute__((__packed__));

#endif