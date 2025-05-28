/*  network.hh   */
#pragma once
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

/*
 * Minimal-footprint network/MQTT helper for ESP32 & PubSubClient.
 * If you pass a non-null PEM root certificate the connection is verified,
 * otherwise `setInsecure()` is used (TLS without validation—OK for testing,
 * never for production).
 */
class Network {
       public:
	Network(const char* wifi_ssid, const char* wifi_pwd,
		const char* mqtt_host,
		uint16_t mqtt_port = 8883,  // 1883 for plain
		const char* mqtt_user = nullptr,
		const char* mqtt_pass = nullptr,
		const char* mqtt_topic = "sensor/data",
		const char* ca_cert_pem = nullptr)
	    : _ssid(wifi_ssid),
	      _pwd(wifi_pwd),
	      _host(mqtt_host),
	      _port(mqtt_port),
	      _user(mqtt_user),
	      _pass(mqtt_pass),
	      _topic(mqtt_topic),
	      _ca(ca_cert_pem),
	      _mqtt(_client) {}

	/* call once from setup() */
	void begin() {
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

	/* call every loop() */
	void loop() {
		if (!_mqtt.connected()) _reconnect();
		_mqtt.loop();
	}

	/* publish one float (returns true on success) */
	bool push(float value) {
		char payload[32];
		dtostrf(value, 0, 2, payload);                   // "123.45"
		bool ok = _mqtt.publish(_topic, payload, true);  // retained
		if (!ok) Serial.println(F("[NET] MQTT publish failed"));
		return ok;
	}

       private:
	void _reconnect() {
		while (!_mqtt.connected()) {
			Serial.printf("[NET] MQTT connect %s:%u …\n", _host,
				      _port);
			bool ok =
			    _user ? _mqtt.connect("esp32-client", _user, _pass)
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

	/* data
	 * ------------------------------------------------------------------*/
	const char* _ssid;
	const char* _pwd;
	const char* _host;
	uint16_t _port;
	const char* _user;
	const char* _pass;
	const char* _topic;
	const char* _ca;

	WiFiClientSecure _client;
	PubSubClient _mqtt;
};
