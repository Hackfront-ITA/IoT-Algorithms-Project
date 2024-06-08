#include <stdint.h>

#include <esp_log.h>
#include <mqtt_client.h>

#include "config.h"
#include "utils.h"

bool c_mqtt_connected = false;

extern const uint8_t c_mqtt_cert[]   asm("_binary_mqtt_cert_pem_start");

static const char *TAG = "MQTT";

static esp_mqtt_client_config_t c_mqtt_cfg = {
	.broker = {
		.address.uri = MQTT_BROKER_URL,
		.verification = {
			.certificate = (const char *)(c_mqtt_cert),
			.skip_cert_common_name_check = true,
			// .common_name = "MQTT Server",
		}
	}
};

static esp_mqtt_client_handle_t c_client;

static void c_mqtt_on_connect(void *handler_args,
		esp_event_base_t base, int32_t event_id, void *event_data);
static void c_mqtt_on_disconnect(void *handler_args,
		esp_event_base_t base, int32_t event_id, void *event_data);
static void c_mqtt_on_error(void *handler_args,
		esp_event_base_t base, int32_t event_id, void *event_data);
static void c_mqtt_on_data(void *handler_args,
		esp_event_base_t base, int32_t event_id, void *event_data);

esp_err_t c_mqtt_start(void) {
	c_client = esp_mqtt_client_init(&c_mqtt_cfg);
	if (c_client == NULL) {
		return ESP_FAIL;
	}

	esp_mqtt_client_register_event(c_client,
		MQTT_EVENT_CONNECTED,    c_mqtt_on_connect,     NULL);
	esp_mqtt_client_register_event(c_client,
		MQTT_EVENT_DISCONNECTED, c_mqtt_on_disconnect,  NULL);
	esp_mqtt_client_register_event(c_client,
		MQTT_EVENT_ERROR,        c_mqtt_on_error,       NULL);
	esp_mqtt_client_register_event(c_client,
		MQTT_EVENT_DATA,         c_mqtt_on_data,        NULL);

	esp_mqtt_client_start(c_client);

	return ESP_OK;
}

esp_err_t c_mqtt_stop(void) {
	return ESP_OK;
}

esp_err_t c_mqtt_publish(const char *topic, const char *data) {
	int msg_id;

	msg_id = esp_mqtt_client_enqueue(c_client, topic, data, 0, 0, 0, true);

	if (msg_id < 0) {
		ESP_LOGE(TAG, "Error publishing message, rc=%d", msg_id);
		return ESP_FAIL;
	}

	return ESP_OK;
}


static void c_mqtt_on_connect(void *handler_args,
		esp_event_base_t base, int32_t event_id, void *event_data)
{
	ESP_LOGI(TAG, "Connected");

	c_mqtt_connected = true;

	c_mqtt_publish("/buildings/" S_ESCAPE(BUILDING_ID) "/controller", "BOOTED");
	ESP_LOGI(TAG, "Sent alive message");
};

static void c_mqtt_on_disconnect(void *handler_args,
		esp_event_base_t base, int32_t event_id, void *event_data)
{
	ESP_LOGI(TAG, "Disconnected");

	c_mqtt_connected = false;
};

static void c_mqtt_on_error(void *handler_args,
		esp_event_base_t base, int32_t event_id, void *event_data)
{
	ESP_LOGI(TAG, "Generic error");

	// if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
	//   ESP_LOGE(TAG, "Last error reported from esp-tls: 0x%x",
	//       event->error_handle->esp_tls_last_esp_err);
	//
	//   ESP_LOGE(TAG, "Last error reported from tls stack: 0x%x",
	//       event->error_handle->esp_tls_stack_err);
	//
	//   ESP_LOGE(TAG, "Last error captured as transport's socket errno: 0x%x",
	//       event->error_handle->esp_transport_sock_errno);
	//
	//   ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
	// }
};

static void c_mqtt_on_data(void *handler_args,
		esp_event_base_t base, int32_t event_id, void *event_data)
{
  esp_mqtt_event_handle_t event = event_data;
  esp_mqtt_client_handle_t client = event->client;

  ESP_LOGI(TAG, "MQTT_EVENT_DATA");
  printf("  TOPIC=%.*s\r\n", event->topic_len, event->topic);
  printf("  DATA=%.*s\r\n", event->data_len, event->data);
};
