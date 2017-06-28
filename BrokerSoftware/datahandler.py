#!/usr/bin/env python

import paho.mqtt.client as mqtt
import json
import numpy
from pprint import pprint

# Global variables
pmatrix = numpy.zeros(shape=(4,4))

# The callback for when the client successfully connects to the broker
def on_connect(client, userdata, rc):
    ''' We subscribe on_connect() so that if we lose the connection
        and reconnect, subscriptions will be renewed. '''

    client.subscribe("dev/test")

# The callback for when a PUBLISH message is received from the broker
def on_message(client, userdata, msg):

    topic = msg.topic

    try:
        data = json.loads(str(msg.payload))
        row0 = data["row0"]
        row1 = data["row1"]
        row2 = data["row2"]
        row3 = data["row3"]
        
        pmatrix[0] = row0
        pmatrix[1] = row1
        pmatrix[2] = row2
        pmatrix[3] = row3
        
        pprint("Pressure Matrix = ")
        pprint(pmatrix)
        
    except:
        print "Cannot decode data on topic {0}".format(topic)

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("localhost", 1883, 60)

# Blocking call which processes all network traffic and dispatches
# callbacks (see on_*() above). It also handles reconnecting.

client.loop_forever()