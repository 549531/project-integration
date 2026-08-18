FROM docker.io/library/node:alpine AS builder
WORKDIR /src

# Install Yarn
RUN npm install -g corepack

# Copy the code
COPY . .

# Install dependencies
RUN yarn install

# Build
RUN yarn build

# Serve
FROM docker.io/library/nginx:stable-alpine
COPY --from=builder /src/dist /usr/share/nginx/html
