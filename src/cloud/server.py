#!/usr/bin/env python3
from random import randint
from ssl import CERT_NONE
from time import sleep
from signal import signal, SIGINT
from threading import Event

from paho.mqtt import client as mqtt_client

BUILDING_ID = 123

MQTT_BROKER = 'localhost'
MQTT_PORT = 8883
BASE_TOPIC = f"/buildings/{BUILDING_ID}"

exit = Event()
mqtt_connected = False

def signal_handler(sig, frame):
	global running

	if sig == SIGINT:
		exit.set()

def mqtt_on_connect(client, userdata, flags, rc):
	global mqtt_connected

	if rc == 0:
		mqtt_connected = True
		print('[MQTT] Connected')
	else:
		mqtt_connected = False
		print('[MQTT] Connection failed')

def mqtt_on_disconnect(client, userdata, rc):
	mqtt_connected = False
	print('[MQTT] Disconnected')

def mqtt_on_messsage(client, userdata, msg):
	print(f'[MQTT] {msg.topic}: {msg.payload.decode()}')


signal(SIGINT, signal_handler)

client_id = f'sm-edge-{randint(1000, 9999)}'
client = mqtt_client.Client(client_id)

client.on_connect = mqtt_on_connect
client.on_disconnect = mqtt_on_disconnect
client.on_message = mqtt_on_messsage

client.tls_set(cert_reqs=CERT_NONE)
client.connect(MQTT_BROKER, MQTT_PORT)

client.loop_start()

client.subscribe(BASE_TOPIC + '/sensors/#')
client.subscribe(BASE_TOPIC + '/status/#')

while not exit.is_set():
	if mqtt_connected:
		client.publish(BASE_TOPIC + '/timer', 'ping', qos=2)

	exit.wait(3)

client.loop_stop()
