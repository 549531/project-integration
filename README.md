## Repository organization

|  Path  | Description
| ------ | -----------
| docs/  | This project's documentation, made with [Sphinx]. Automatically deployed to [GitHub Pages](https://549531.github.io/project-integration).
| glove/ | [ESP-IDF] project, built with [PlatformIO]. Deployed to the ESP32 embedded in the glove.

[ESP-IDF]: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/index.html
[PlatformIO]: https://docs.platformio.org/en/latest/core/index.html
[Sphinx]: https://www.sphinx-doc.org/

## Documentation

### Build

```
cd docs
make html
```

### Run development server

```
make livehtml
```

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
