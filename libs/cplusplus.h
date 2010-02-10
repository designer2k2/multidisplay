/*
 * cplusplus.hxx
 *
 *  Created on: 01.02.2010
 *      Author: bofh
 */

#ifndef CPLUSPLUS_HXX_
#define CPLUSPLUS_HXX_

//taken from the avr-c++ micro howto http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&t=59453

#include <stdlib.h>

void * operator new(size_t size);
void operator delete(void * ptr);

__extension__ typedef int __guard __attribute__((mode (__DI__)));

extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release (__guard *);
extern "C" void __cxa_guard_abort (__guard *);

//needed fpr pure virtual functions
extern "C" void __cxa_pure_virtual(void);

#endif /* CPLUSPLUS_HXX_ */
