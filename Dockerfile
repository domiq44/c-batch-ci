# ---- STAGE 0 : Base ----
FROM alpine:3.20 AS base
RUN apk add --no-cache build-base cmocka-dev clang-extra-tools

WORKDIR /app
COPY Makefile .
COPY include ./include
COPY src ./src
COPY tests ./tests

# ---- STAGE 1 : Lint ----
FROM base AS lint
RUN clang-tidy src/*.c --warnings-as-errors=* || true

# ---- STAGE 2 : Tests ----
FROM base AS test
RUN make test

# ---- STAGE 3 : Build ----
FROM base AS builder
RUN make CFLAGS="-O2 -Wall -Wextra -Werror -static"

# ---- STAGE 4 : Runtime ----
FROM scratch
USER 1000
WORKDIR /app
COPY --from=builder /app/c-batch-ci /app/c-batch-ci
ENTRYPOINT ["/app/c-batch-ci"]
