# FSR_HuzzahShield

The goal of the project is to analyze a persons sitting posture over time & give immediate feedback if the user has been sitting too long, or has adapted a bad sitting position.

To do this we connect a 4x4 Force Sensitive Resistor matrix to an ESP8266 in order to read out and and wirelessly transmit the resistance values over WiFi with the MQTT protocol.


## Sensor Hardware
Contains the eagle files of the project.

## Sensor Software
Firmware to read out the matrix. The ESP8266 is programmed using Arduino for fast prototyping & access to convenient libraries (eg the PubSubClient MQTT library). And because getting the Espressif Toolchain set up has proved to be a real pain. The data is transferred over MQTT to a central broker (I'm using an old Raspberry Pi) where data processing takes place. Which brings us to

## Broker Software
Contains the data processing code on the central hub. Work in progress.
To set up a Raspberry Pi as a MQTT broker, [this](https://www.youtube.com/watch?v=AsDHEDbyLfg) is a great video.




This README was created using [Dillinger](www.dillinger.io).
