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
		const char* ca_cert_pem = nullptr);

	/* call once from setup() */
	void begin();

	/* call every loop() */
	void loop();

	/* publish one float (returns true on success) */
	bool push(float value);

       private:
	void _reconnect();

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
