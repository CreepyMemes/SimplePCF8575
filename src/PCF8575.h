#pragma once

#include <Arduino.h>
#include <Wire.h>

// Pin name mapping
#define P00  0
#define P01  1
#define P02  2
#define P03  3
#define P04  4
#define P05  5
#define P06  6
#define P07  7
#define P10  8
#define P11  9
#define P12  10
#define P13  11
#define P14  12
#define P15  13
#define P16  14
#define P17  15

// PCF8575 register addresses
#define PCF8575_INITIAL_VALUE 0xFFFF
#define PCF8575_OK 			  0x00
#define PCF8575_PIN_ERROR 	  0x81
#define PCF8575_I2C_ERROR 	  0x82

/*!
	@brief  Class driver for a 16 pin port expander using the PCF8575 chip.
*/
class PCF8575{
	
	public:
		PCF8575(const uint8_t address = 0x20, TwoWire *wire = &Wire);

		bool     begin();

		uint8_t  digitalRead(uint8_t pin);	
		void 	 digitalWrite(const uint8_t pin, const uint8_t value);
		void 	 pinMode(const uint8_t pin, const uint8_t mode);

	private:
		bool 	 isConnected(); // Check if the device is connected

		void 	 writeAll(const uint16_t data); // Writes all pins
		uint16_t readAll();				         // Reads all pins

		uint8_t  _address;	 // The I2C device's address
		uint16_t _dataIn;	 // data input buffer
		uint16_t _dataOut;	 // Data output buffer
		uint16_t _pinConfig; // Pin mode configuration mask  (default assumes all pins as INPUT PULLUP)

		TwoWire *_wire;
};
