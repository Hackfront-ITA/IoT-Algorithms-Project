#!/usr/bin/env python3
from json import loads
from random import randint
from re import search
from signal import signal, SIGINT
from ssl import CERT_NONE
from threading import Event
from time import sleep

from mysql.connector import connect
from paho.mqtt import client as mqtt_client

DB_HOST = 'localhost'
DB_USERNAME = 'sm_user'
DB_PASSWORD = 'password'
DB_DATABASE = 'sm_app'

MQTT_BROKER = 'localhost'
MQTT_PORT = 8883

def signal_handler(sig, frame):
	global client

	if sig == SIGINT:
		client.loop_stop()

def mqtt_on_connect(client, userdata, flags, rc):
	if rc == 0:
		print('[MQTT] Connected')
	else:
		print('[MQTT] Connection failed')

def mqtt_on_disconnect(client, userdata, rc):
	print('[MQTT] Disconnected')

def mqtt_on_messsage(client, userdata, msg):
	global db_conn

	topic = msg.topic
	payload = msg.payload.decode()
	print(f'[MQTT] {msg.topic}: {payload}')

	re_res = search('^/buildings/([0-9].*)/point/([0-9].*)/event$', topic)
	if re_res:
		building_id = re_res.group(1)
		point_id = re_res.group(2)
		payload = loads(payload)

		params = (
			building_id, payload['epoch'], point_id,
			payload['axis'], payload['frequency'], payload['value']
		)
		cursor = db_conn.cursor()
		cursor.execute(
			'INSERT INTO events (timestamp, building_id, epoch, point_id, \
			axis, frequency, value) \
			VALUES (NOW(), %s, %s, %s, %s, %s, %s)', params)
		db_conn.commit()

signal(SIGINT, signal_handler)

db_conn = connect(
	host = DB_HOST,
	user = DB_USERNAME,
	passwd = DB_PASSWORD,
	database = DB_DATABASE
)

client_id = f'sm-cloud-{randint(1000, 9999)}'
client = mqtt_client.Client(client_id)

client.on_connect = mqtt_on_connect
client.on_disconnect = mqtt_on_disconnect
client.on_message = mqtt_on_messsage

client.tls_set(cert_reqs=CERT_NONE)
client.connect(MQTT_BROKER, MQTT_PORT)

client.subscribe('/buildings/#')

client.loop_forever()
