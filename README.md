# pms3003-dust-sensor-pc-client
AirQualityMonitor application that connects PMS3003 dust sensor to PC via UART-USB converter and prints air quality measurements

The purpose of this software is to make measurements reader on pc side to get the data from 
air quality sensor. The sensor is supposed to be connected via uart-usb converter.
Beside the software to print the data in terminal, the project contains a guide how to connect the hardware
and install needed drivers.


Needed components:
* hardware:
	PMS3003 device
	uart-usb converter: output power 5V, signals @ 3.3V

* software:
	Uart driver zagid2.3 - http://zadig.akeo.ie
	Libusb - https://github.com/libusb/libusb
	Windows 7/8/10 (TODO: Linux support)
	Visual Studio Visual 2013/2017




Hardware connection schema:

* connect PMS3003 pin1 to usb-uart 5v pin
* connect PMS3003 pin2 to usb-uart ground pin
* connect PMS3003 pin5 to usb-uart Rx pin
See the pictures of PMS3003 & usb-uart devices.




Software installation procedure:
* Install vc_redist.x86.exe from Install folder (or another version of Visual Studio Redist from Microsoft)
* Plugin usb-uart to USB port and apply the driver to usb-uart device using zadig-2.3.exe from Install folder  




AirQualityMonitor build procedure:
* Open VS2017.sln or VS2013.sln solution.
* Build All to build Libusb and AirQualityMonitor(executable PC measurement client)

Note: Currently only VS 2013 & VS 2017 are supported. 
TODO: add gcc support
