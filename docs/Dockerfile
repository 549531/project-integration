FROM python:3.13-slim AS builder
WORKDIR /src

# Install PlantUML
RUN \
	apt-get update &&\
	apt-get install -y --no-install-recommends \
		default-jre-headless &&\
	rm -rf /var/lib/apt/lists/*

ADD https://github.com/plantuml/plantuml/releases/latest/download/plantuml.jar /opt/plantuml.jar
COPY .docker/bin/plantuml /usr/bin/plantuml

# Setup venv
RUN --mount=type=cache,target=/src/.venv,sharing=locked,id=pint-docs-venv \
	python -m venv --symlinks .venv
ENV PATH="/src/.venv:$PATH"

# Install dependencies
COPY requirements.txt .
RUN --mount=type=cache,target=/src/.venv,sharing=locked,id=pint-docs-venv \
	pip install --no-cache-dir -r requirements.txt

# Build
COPY . .
RUN --mount=type=cache,target=/src/.venv,sharing=locked,id=pint-docs-venv \
	sphinx-build -M html . _build

# Save the result
FROM scratch
COPY --from=builder /src/_build/html .
