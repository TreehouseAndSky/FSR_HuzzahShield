# Helpfile for the Broker Software

## Testing (MacOS X)

### install mosquitto

```sh
$ brew install mosquitto
``` 

### install paho-mqtt Python client library

```sh
$ pip install paho-mqtt
``` 

[Official documentation](https://pypi.python.org/pypi/paho-mqtt/1.1)

### run a mosquitto broker (not background)

```sh
$ mosquitto -c /usr/local/etc/mosquitto/mosquitto.conf
``` 

## Deployment (Raspberry Pi)

