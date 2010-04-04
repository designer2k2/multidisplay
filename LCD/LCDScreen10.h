/*
 * LCDScreen10.h
 *
 *  Created on: 02.04.2010
 *      Author: bofh
 */

#ifndef LCDSCREEN10_H_
#define LCDSCREEN10_H_

#include "LCDScreen.h"

class LCDScreen10: public LCDScreen {
public:
	LCDScreen10();
	virtual ~LCDScreen10();

	void customInit ();
	void customDraw ();
};

#endif /* LCDSCREEN10_H_ */
