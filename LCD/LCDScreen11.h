/*
 * LCDScreen11.h
 *
 *  Created on: 06.11.2011
 *      Author: bofh
 */

#ifndef LCDSCREEN11_H_
#define LCDSCREEN11_H_

#include "LCDScreen.h"

class LCDScreen11 : public LCDScreen {
public:
	LCDScreen11();
	virtual ~LCDScreen11();


	void customInit ();
	void customDraw ();
};

#endif /* LCDSCREEN11_H_ */
