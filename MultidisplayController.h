/*
    Copyright 2009-13 Stephan Martin, Dominik Gummel

    This file is part of Multidisplay.

    Multidisplay is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Multidisplay is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Multidisplay.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MULTIDISPLAYCONTROLLER_H_
#define MULTIDISPLAYCONTROLLER_H_

#include"MultidisplayDefines.h"
#include "util.h"

#include<Wire.h>
#include <avr/pgmspace.h>
#include"SensorData.h"
#include "LCD/LCDController.h"
#include "OledController.h"




class MultidisplayController {
public:
	MultidisplayController();
	//dont use the constructor because I want to control when the global objects get initialized
	void myconstructor();

	uint8_t SerOut;
    uint8_t DoCal;
    uint8_t DoTypK;
    uint8_t DoCheck;
    unsigned long ScreenSave;
    unsigned long time;

	void mainLoop();
	void ChangeSerOut();
	void calibrateLD();
	void HeaderPrint();



//private:
    static const unsigned int PROGMEM tempTypK[];
    static const unsigned int PROGMEM tempVDOPressure[];
    static const unsigned int PROGMEM tempVDOTemp[];
    uint8_t IOport2;
    TwoWire wire;
    unsigned long FlashETimeU;
    unsigned long FlashTimeU;

    uint16_t serialFreq;
    unsigned long serialTime;

    unsigned long drawTime;

    /**
     *  ============ pc -> arduino ==================
     *  the messages to the arduino follow the following format:
     *  0: 0=Manual, 1=Auto, 2=multidisplay command, else = ignore
     *
     *  for pid lib
     *  1-4: fixed point value (scale factor 1000) setpoint
     *  5-8: fixed point value (scale factor 1000) input
     *  9-12: fixed point value (scale factor 1000) output
     *  13-16: fixed point value (scale factor 1000) P_Param
     *  17-20: fixed point value (scale factor 1000) I_Param
     *  21-24: fixed point value (scale factor 1000) D_Param
     *
     *  multidisplay command (from pc to arduino)
     *  2: buttons: 1=a pressed, 2=a hold, 3=b pressed, 4=b hold
     *
     *  3: change serial mode: 2=enabled(string), 3=tunerpro adx request data
     *
     *  4: V1 eeprom & config: 1=save settings, 2 calibrate boost (measure and store ambient pressure)
     *  			3 readSettings, 5 set lcd brightness, 4 set v1 n75 duty cycles
     *
     *  5: V2 dev debug / testing stuff
     *
     *  6: n75 v2:
     *  	1 gearX mode(low=0 high=1) serial : request gearX high/low duty cycle map
     *  		reply: STX tag=22 gearX mode serial 16 bytes map ETX
     *  	2 gearX mode(low=0 high=1) serial : request gearX high/low setpoint map
     *  		reply: STX tag=38 gearX mode serial 32 bytes map ETX fixed point 16 bit values base 100!
     *  	3 gearX mode(low=0 high=1) serial 16 bytes map values: set gearX high/low duty cycle map [size 21]
     *  		reply: STX tag=22 gearX mode serial 16 bytes map ETX neu geschriebene map zurücksenden!
     *  	4 gearX mode(low=0 high=1) serial 32 bytes map values (fixed point base 100): set gearX high/low setpoint map [size 37]
     *  		reply: STX tag=38 gearX mode serial 32 bytes map ETX fixed point 16 bit values base 100!
     *  	5 serial load maps from eeprom
     *  		reply: STX 4 serial ETX	ACK!
     *  	6 serial save maps to eeprom
     *  		reply: STX 4 serial ETX	ACK!
     *  	7 serial load N75 params from eeprom
     *  		reply: STX 4 serial ETX	ACK!
     *  	8 serial save N75 params to eeprom
     *  		reply: STX 4 serial ETX	ACK!
     *  	9 set N75 params: 9 serial aKp aKi aKd cKp cKi cKd aAT cAT (16bit fixed uint16 base 100) flags (uint8 bit0=pid enable) bst_limit (16bit base 100)  ETX
     *  		reply: STX 4 serial ETX	ACK!
     *  	10 get N75 params: 10 serial
     *  		reply: STX tag=21 serial aKp aKi aKd cKp cKi cKd aAT cAT (16bit fixed uint16 base 100) flags (uint8 bit0=pid enable) bst_limit (16bit base 100) ETX
     * 		11 set serial frequency: 11 serial lowbyte hibyte
     * 			reply: STX 4 serial ETX	ACK!
     * 		12 reserved for get serial frequency: 12 serial
     * 			reply: unimplemented!
     * 		13: get gear ratio: 13 serial
     * 			reply: STX 17 serial gears gear*uint16(fixed_int_base1000) ETX
     * 		14: set gear ratio: 14 serial gears gear*uint16(fixed_int_base1000)
     * 			reply: STX 17 serial gears gear*uint16(fixed_int_base1000) ETX
     *
     *
     *
     *  attention, you have to send ints, not chars over the serial line!
     *
     *  ============ arduino -> pc ==================
     *  [old v1, deprecated] pid messages from arduino to pc are prefixed with "PID " as chars!
     *
     *  serial data stream:
     *  STX TAG(=also complete frame length) data ETX
     */
    union {
      byte asBytes[48];
      float asFloat[12];
      int32_t asFixedInt32[12];
    } srData;

	int read_adc(uint8_t channel);

	int read_adc_fast(uint8_t channel);
	int read_adc_fast_mega(uint8_t channel);

	void expanderWrite(byte _data);
	byte expanderRead();
	void expanderWrite2(byte _data);
	byte expanderRead2();
	void AnaConversion();
	void V1_Shiftlight();

	void V2_WarnLED();

	void serialSend();
	void serialSendAck (uint8_t serial);
#ifdef GEAR_RECOGNITION
	void serialSendGearRatioMap (uint8_t serial);
#endif
	void serialReceive();

	int GetTypKTemp(unsigned int microVolts);
	int GetVDOTemp(unsigned int ADWreading);
	int GetVDOPressure(unsigned int ADWreading);

	//TypK state machine
#define TYPK_STATE_NEXT_READ_CHANNEL 1
#define TYPK_STATE_NEXT_SELECT_CHANNEL 2
	uint8_t typK_state;
	uint8_t typK_state_cur_channel;
	unsigned long typK_state_time2read;

	void FetchTypK();
	void fetchTypK2();
	void fetchTypK3_fast();
	void selectTypKchannelForReading ( uint8_t channel );
	void readTypK ( uint8_t channel );
	void incTypKChannel () {
		typK_state_cur_channel++;
		if ( typK_state_cur_channel == NUMBER_OF_ATTACHED_TYPE_K ) {
			typK_state_cur_channel = 0;
			data.checkAndSaveMaxEgt();
		}
	}

	void CheckLimits();

	//button related functions
	unsigned long buttonTime;   //Button time

	//TODO documentation!
	void buttonCheck_V2();
	void buttonCheck_V1(int buttonState);
	void buttonAHold();
	void buttonAPressed();
	void buttonBHold();
	void buttonBPressed();

	void readSettingsFromEeprom();
	void saveSettings2Eeprom();

#if defined(MULTIDISPLAY_V2) && defined(DIGIFANT_KLINE)

	void DFKlineSerialReceive();
	void DFConvertReceivedData();

	union {
	    	byte asBytes[DF_KLINEFRAMESIZE];
	    } df_klineData[DF_KLINE_STORE_FRAME_COUNT];
	unsigned long DF_KlineSerialTime;
	//! holds the index of the last frame (255 if no frame was received)
	uint8_t df_kline_last_frame_completely_received;
	uint16_t df_kline_freq_milliseconds;
	unsigned long df_kline_millis_last_frame_received;
	uint8_t df_kline_active_frame;
	uint8_t df_kline_index;
	uint8_t df_kline_status;
	uint16_t df_kline_discarded_frames;
#endif

#if defined(MULTIDISPLAY_V2) && defined(KWP1281_KLINE)
	union {
	    	byte asBytes[KWP1281_KLINEFRAMESIZE];
	    } kwp1281_klineData[KWP1281_KLINE_STORE_FRAME_COUNT];
	uint8_t kwp1281_kline_active_frame;
	int kwp1281_kline_index;
	inline void kwp1281IncActiveFrame () { kwp1281_kline_active_frame++; if ( kwp1281_kline_active_frame == KWP1281_KLINE_STORE_FRAME_COUNT ) kwp1281_kline_active_frame=0; };
	inline void kwp1281IncIndex () { kwp1281_kline_index++; if ( kwp1281_kline_index == KWP1281_KLINEFRAMESIZE ) kwp1281_kline_index=0; };

	uint8_t kwp1281_block_counter;
	uint8_t kwp1281_state;
	uint8_t kwp1281_connect_failures;

	uint8_t kwp1281_block_state;
	uint8_t kwp1281_block_length;
	uint8_t kwp1281_block_title;

	uint8_t kwp1281_controller_data_block_counter;
	bool kwp1281_controller_data_more_than_four_blocks;

	//! set t0 0 on start; even = send , odd = receive answer
	uint8_t kwp1281_send_block_state;

#ifdef KWP1281_KLINE_DEBUG
	unsigned long kwp1281_debug_printtime;
#endif

	unsigned long kwp1281_kline_millis_last_byte_received;
	unsigned long kwp1281_kline_millis_last_byte_send;

//	inline void setKLHigh () { PORTA |=  128; PORTD |= 8;  digitalWrite (16,HIGH);  digitalWrite (2,HIGH); PORTE |= 16; digitalWrite(3,HIGH); };
//	inline void setKLLow () { PORTA &=  ~128; PORTD &= ~8;  digitalWrite (16,LOW); digitalWrite (2,LOW); PORTE &= ~16; digitalWrite(3,LOW);};
	inline void setKLHigh () { PORTA |=  128; PORTD |= 8; PORTH |= 2; PORTJ |= 2;};
	inline void setKLLow () { PORTA &=  ~128; PORTD &= ~8; PORTH &= ~2; PORTJ &= ~2;};
	inline void setKLow () { PORTD &= ~8; PORTH &= ~2; PORTJ &= ~2;};
	inline void sendFiveBaudBit (int bit);
	void kwp1281SendControllerAddress(uint8_t address);
	bool kwp1281Connect ();
	bool kwp1281Connect2 ();
	void kwp1281();
	//! returns true if block is completely received
	bool kwp1281ReceiveBlock( bool init = false);

	inline bool kwp1281Read ();
	inline void kwp1281Write (uint8_t d);



#endif

#if defined(MULTIDISPLAY_V2) && defined(BLUETOOTH_SETUP_ON_SERIAL2)
	unsigned long BTSerialTime;
#endif

#if defined(MULTIDISPLAY_V2) && defined(GEAR_RECOGNITION)

#define GEAR_STATE_NEED_RECOGNITION 0
#define GEAR_STATE_MATCHED 1
#define GEAR_STATE_NEED_RECOGNITION_WAIT_FOR_CLUTCH_IN 2
#define GEAR_COMPUTATION_INTERVALL 100
#define GEAR_IDLE_RPM 1300
#define GEAR_WAIT_MSECS_BEFORE_IDLE 500

	void gear_computation ();
	uint8_t gear_state;
	uint8_t last_gear;
	float gear_ratio[GEARS];
	uint8_t gear_clutchpressed;
	unsigned long gear_computation_time;
	unsigned long gear_wait_before_idle;
#endif

#ifdef V2DEVDEBUG
	uint8_t v2devdebugflag;
#endif

#ifdef OLED
	OledController oled;
	uint8_t oled_flags;
#endif


};


extern MultidisplayController mController;



#endif /* MULTIDISPLAYCONTROLLER_H_ */
