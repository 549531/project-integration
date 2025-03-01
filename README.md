## Repository organization

|  Path  | Description
| ------ | -----------
| glove/ | [ESP-IDF] project, built with [PlatformIO]. Deployed to the ESP32 embedded in the glove.

[ESP-IDF]: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/index.html
[PlatformIO]: https://docs.platformio.org/en/latest/core/index.html

## Glove

### Compile

```
cd glove
pio run
```

### Upload to the ESP32

```
cd glove
pio run -t upload
```

### Monitor the ESP32

```
cd glove
pio run -t monitor
```

### Upload and Monitor in one go

```
cd glove
pio run -t upload -t monitor
```
