FROM python:3.13-slim AS builder
WORKDIR /src

# Install UV
COPY --from=ghcr.io/astral-sh/uv:latest /uv /uvx /bin/

# Install dependencies
COPY pyproject.toml uv.lock .
RUN --mount=type=cache,target=/root/.cache/uv,id=uv \
	uv sync --no-dev --locked

# Copy the code
COPY . .

EXPOSE 8000

ENTRYPOINT ["uv", "run"]
CMD ["gunicorn", "api.asgi:application", "-b", "0.0.0.0", "-k", "uvicorn_worker.UvicornWorker"]
