/*
 *  buttons.cpp
 *  One-shot and hold functions for buttons.
 *  Created by Franky on 29/01/09.
 *  Licensed under LGPL (free to modify and use as you wish)
 */



#include "Arduino.h"
#include "buttons.h"


Button::Button() { 
	previous = false;
	mode = OneShot;
	hold_timer = millis();
	refresh_timer = millis();
	hold_level = 1000; // 1 second
	hold_refresh = 100; // 100 ms
	pin = 0;
}
Button::Button(byte mode_v) {
	previous = false;
	mode = mode_v;
	hold_timer = millis();
	refresh_timer = millis();
	hold_level = 1000;
	hold_refresh = 100;
	pin = 0;
}
	

void Button::assign(byte pin_v) { 
	pin = pin_v; 
	pinMode(pin, INPUT);
}
void Button::turnOnPullUp() { if (pin) digitalWrite(pin, HIGH); }
void Button::turnOffPullUp() { if (pin) digitalWrite(pin, LOW); }	

// Setters
void Button::setMode(byte mode_v) { mode = mode_v; }
void Button::setTimer(unsigned int t) { if (mode > Memory) hold_level = (unsigned long)t; }
void Button::setRefresh(unsigned int r) { hold_refresh = r; }

byte Button::check(byte mode_v) {
	byte mem = mode;
	mode = mode_v;
	byte res = check();
	mode = mem;
	return res;
}
	

byte Button::check() {
	if (digitalRead(pin)) {
		switch (mode) {
			case OneShot:
				if (previous) {
					return OFF;
				}
				else {
					previous = true;
					return ON;
				}
				break;
			case Memory:
				if (previous) {
					return ON;
				}
				else {
					previous = true;
					return Pressed;
				}
				break;
			case Timer:
				if (previous) {
					// Is treshold reached?
					if (millis() > hold_timer+hold_level) {
						if (millis() > refresh_timer+hold_refresh) {
							refresh_timer = millis();
							return Hold;
						}
						else return OFF;
					}
					else {
						return ON;
					}
				}
				else {
					previous = true;
					hold_timer = millis();
					refresh_timer = millis();
					return ON;
				}
				break;
			case OneShotTimer:
				if (previous) {
					// Is treshold reached?
					if (millis() > hold_timer+hold_level) {
						if (millis() > refresh_timer+hold_refresh) {
							refresh_timer = millis();
							return Hold;
						}
						else return OFF;
					}
					else {
						return OFF;
					}
				}
				else {
					previous = true;
					hold_timer = millis();
					refresh_timer = millis();
					return ON;
				}
				break;
			case MemoryTimer:
				if (previous) {
					// Is treshold reached?
					if (millis() > hold_timer+hold_level) {
						if (millis() > refresh_timer+hold_refresh) {
							refresh_timer = millis();
							return Hold;
						}
						else return OFF;
					}
					else {
						return ON;
					}
				}
				else {
					previous = true;
					hold_timer = millis();
					refresh_timer = millis();
					return Pressed;
				}
				break;
			default:
				break;
		}
	}
	else {
		if (previous && (mode == 1 || mode == 4)) {
			previous = false;
			return Released;
		}
		previous = false;
		hold_timer = millis();
		refresh_timer = millis();
		return OFF;
	}
	return OFF;
}

