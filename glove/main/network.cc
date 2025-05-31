#include "network.hh"

static char const* TAG = "network";

Network::Network(const char* wifi_ssid, const char* wifi_pwd,
		 const char* mqtt_host,
		 uint16_t mqtt_port,  // 1883 for plain
		 const char* mqtt_user, const char* mqtt_pass,
		 const char* mqtt_topic, const char* ca_cert_pem)
    : _ssid(wifi_ssid),
      _pwd(wifi_pwd),
      _host(mqtt_host),
      _port(mqtt_port),
      _user(mqtt_user),
      _pass(mqtt_pass),
      _topic(mqtt_topic),
      _ca(ca_cert_pem),
      _mqtt(_client) {}

void Network::begin() {
	ESP_LOGI(TAG, "Wi-Fi connect…");
	WiFi.mode(WIFI_STA);
	WiFi.begin(_ssid, _pwd);
	while (WiFi.status() != WL_CONNECTED) {
		ESP_LOGI(TAG, ".");
		delay(500);
	}
	ESP_LOGI(TAG, "Wi-Fi OK – IP=%s", WiFi.localIP().toString().c_str());

	// if (_ca)
	// 	_client.setCACert(_ca);
	// else
	// 	_client.setInsecure();  // skip cert validation

	_mqtt.setServer(_host, _port);
	_mqtt.setBufferSize(64);  // long enough for JSON etc.
	_mqtt.setSocketTimeout(10);
	_reconnect();
}

void Network::loop() {
	if (!_mqtt.connected()) _reconnect();
	_mqtt.loop();
}

bool Network::push(float value) {
	char payload[8];
	dtostrf(value, 0, 2, payload);                   // "123.45"
	bool ok = _mqtt.publish(_topic, payload, true);  // retained
	if (!ok) ESP_LOGI(TAG, "MQTT publish failed");
	return ok;
}

void Network::_reconnect() {
	while (!_mqtt.connected()) {
		ESP_LOGI(TAG, "MQTT connect %s:%u …", _host, _port);
		bool ok = _user ? _mqtt.connect("esp32-client", _user, _pass)
				: _mqtt.connect("esp32-client");
		if (!ok) {
			ESP_LOGI(TAG, "  rc=%d – retry in 2 s", _mqtt.state());
			delay(2000);
		} else {
			ESP_LOGI(TAG, "MQTT connected");
		}
	}
}
