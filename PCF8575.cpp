#include "PCF8575.h"

/*!
*	@brief Class constructor
*	@param address the device's I2C address which can be 0x20 .. 0x27
*	@param wire	   the I2C Wire object instance
*/
PCF8575::PCF8575(const uint8_t deviceAddress, TwoWire *wire){
	_address   = deviceAddress;
	_wire      = wire;
	_dataIn    = 0;
	_dataOut   = PCF8575_INITIAL_VALUE;
	_pinConfig = PCF8575_INITIAL_VALUE;
}

/*!
*	@brief  Method that starts I2C and sets all pins as input pullup by default
*	@return true if successful, otherwise false
*/
bool PCF8575::begin(){
	if (!isConnected()) return false;
	writeAll(_dataOut);
	return true;
}

/*!
*	@brief  Method to read a specified pin
*	@param  pin the pin value P00 .. P17
*	@return pin state -> HIGH / LOW
*/
uint8_t PCF8575::digitalRead(const uint8_t pin){
	if (pin > 15) return 0; // Check if pin is illegal
	if ((_pinConfig & (1 << pin)) == 0)  return 0; // Check if the pin is not set as INPUT

	readAll();
	return (_dataIn & (1 << pin)) > 0;
}

/*!
*	@brief Method to write a specified pin
*	@param pin    the pin value P00 .. P17
*	@param value  the value to set the pin as -> HIGH / LOW
*/
void PCF8575::digitalWrite(const uint8_t pin, const uint8_t value){
	if (pin > 15) return; 						 // Check if pin is illegal
    if ((_pinConfig & (1 << pin)) != 0)  return; // Check if the pin is not set as OUTPUT

	if (value == LOW) _dataOut &= ~(1 << pin);
	else			  _dataOut |=  (1 << pin);
	
	writeAll(_dataOut);
}

/*!
*	@brief Method to set the state of a specified pin
*	@param pin  the pin value P00 .. P17
*	@param mode the mode to set the pin as -> INPUT / INPUT_PULLUP / OUTPUT
*/
void PCF8575::pinMode(const uint8_t pin, const uint8_t mode) {
    if ((mode == INPUT) || (mode == INPUT_PULLUP)) {
        digitalWrite(pin, HIGH);
        _pinConfig |=  (1 << pin);  // Set the corresponding bit to 1 for INPUT
    } else {
        digitalWrite(pin, LOW);
        _pinConfig &= ~(1 << pin); // Set the corresponding bit to 0 for OUTPUT
    }
}

// ---------------------------------------------------- P R I V A T E S --------------------------------------------------------------

/*!
*	@brief  Checks if the device is connected
*	@return Returns false if it fails
*/
bool PCF8575::isConnected(){
	_wire->beginTransmission(_address);
	return (_wire->endTransmission() == 0);
}

/*!
*	@brief Sends the data stored in the buffer to the I2C device
*	@param data data output buffer that stores the data that will get sent
*/
void PCF8575::writeAll(const uint16_t data){
	_dataOut = data;
	_wire->beginTransmission(_address);
	_wire->write(lowByte (_dataOut));
	_wire->write(highByte(_dataOut));
	_wire->endTransmission();
}

/*!
*	@brief  Reads the state of all pins on the device, and loads them into it's data input buffer
*	@return Returns the read data if successfull
*/
uint16_t PCF8575::readAll(){
	if (_wire->requestFrom(_address, (uint8_t)2) != 2) return _dataIn; 
	
	_dataIn =   _wire->read();		 //  low 8 bits
	_dataIn |= (_wire->read() << 8); //  high 8 bits

	return _dataIn;
}