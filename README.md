# pms3003-dust-sensor-pc-client
PMS3003 dust sensor connected to PC via UART-USB converter to print air quality measurements

The purpose of this software is to make measurements reader on pc side to get the data from 
air quality sensor. The sensor is supposed to be connected via uart-usb converter.
Beside the application to print the data in terminal, it contains the guide how to connect the hardware
and install needed drivers.

Needed components:
* hardware:
	PMS3003 device
	uart-usb converter: output power 5V, signals @ 3.3V
* software:
	uart driver zagid2.3
	libusb 
	windows 7/8/10 (TODO: Linux support)


Hardware connection schema:

* connect PMS3003 pin1 to usb-uart 5v pin
* connect PMS3003 pin2 to usb-uart ground pin
* connect PMS3003 pin5 to usb-uart Rx pin
See the pictures of PMS3003 & usb-uart devices.