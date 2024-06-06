void main(void) {
	char buffer[128];
	snprintf(buffer, sizeof(buffer), S_ESCAPE({
		"axis": "%c",
		"frequency": %.05f,
		"value": %.05f
	}), axis, frequency, value);

	// 	ESP_ERROR_CHECK(n_mqtt_publish(MQTT_BASE_TOPIC "/event", buffer));
}
