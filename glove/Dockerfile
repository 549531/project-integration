FROM python:slim AS build
WORKDIR /src

# Install PlatformIO Core
RUN pip install --no-cache-dir platformio

# Install ESP-IDF
COPY platformio.ini .
RUN --mount=type=cache,target=/root/.platformio,id=platformio \
	pio pkg install

# Build the code
COPY . .
RUN --mount=type=cache,target=/root/.platformio,id=platformio \
    --mount=type=cache,target=/src/.pio,id=pint-glove-local \
	pio run

# Copy built files out of the cache
# If this isn't done, COPY --from=… won't work
RUN --mount=type=cache,target=/src/.pio,id=pint-glove-local \
	mkdir /dist && cp -a .pio/build/*/*.elf /dist

# Save the result
FROM scratch
COPY --from=build /dist /
