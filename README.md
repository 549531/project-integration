## Repository organization

| Path      | Description                                                                                                                               |
| --------- | ----------------------------------------------------------------------------------------------------------------------------------------- |
| api/      | Backend for this project. Built with [Django].                                                                                            |
| docs/     | This project's documentation, made with [Sphinx]. Automatically deployed to [GitHub Pages](https://549531.github.io/project-integration). |
| glove/    | [ESP-IDF] project, built with [PlatformIO]. Deployed to the ESP32 embedded in the glove.                                                  |
| frontend/ | This project's website, available on the [web]. Built with [Vue.js] and [GSAP].                                                           |

[Django]: https://www.djangoproject.com/
[ESP-IDF]: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/index.html
[GSAP]: https://gsap.com/
[PlatformIO]: https://docs.platformio.org/en/latest/core/index.html
[Sphinx]: https://www.sphinx-doc.org/
[Vue.js]: https://vuejs.org/
[web]: https://project-integration.foo.ng/

## API

### Run development server

```
cd api
# 1. Install dependencies
uv sync
# 2. Start the server
uv run manage.py runserver
```

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

## Frontend

### Setup

```
# 1. install Yarn
npm install -g corepack
# 2. install project dependencies
yarn
```

### Build

```
yarn build
```

### Start a dev server

```
yarn dev
```
