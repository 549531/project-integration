#include "network.hh"

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
	Serial.println(F("[NET] Wi-Fi connect…"));
	WiFi.mode(WIFI_STA);
	WiFi.begin(_ssid, _pwd);
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print('.');
		delay(500);
	}
	Serial.printf("\n[NET] Wi-Fi OK – IP=%s\n",
		      WiFi.localIP().toString().c_str());

	if (_ca)
		_client.setCACert(_ca);
	else
		_client.setInsecure();  // skip cert validation

	_mqtt.setServer(_host, _port);
	_mqtt.setBufferSize(256);  // long enough for JSON etc.
	_mqtt.setSocketTimeout(10);
	_reconnect();
}

void Network::loop() {
	if (!_mqtt.connected()) _reconnect();
	_mqtt.loop();
}

bool Network::push(float value) {
	char payload[32];
	dtostrf(value, 0, 2, payload);                   // "123.45"
	bool ok = _mqtt.publish(_topic, payload, true);  // retained
	if (!ok) Serial.println(F("[NET] MQTT publish failed"));
	return ok;
}

void Network::_reconnect() {
	while (!_mqtt.connected()) {
		Serial.printf("[NET] MQTT connect %s:%u …\n", _host, _port);
		bool ok = _user ? _mqtt.connect("esp32-client", _user, _pass)
				: _mqtt.connect("esp32-client");
		if (!ok) {
			Serial.printf("[NET]   rc=%d – retry in 2 s\n",
				      _mqtt.state());
			delay(2000);
		} else {
			Serial.println(F("[NET] MQTT connected"));
		}
	}
}
