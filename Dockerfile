FROM ubuntu:22.04

# Install dependencies
RUN apt-get update -y && \
    apt-get install -y cmake g++ libpq-dev libcjson-dev && \
    apt-get clean

# Set the working directory
WORKDIR /app
COPY CMakeLists.txt /app
COPY src /app/src

RUN cmake . && make

ARG DATABASE_NAME=postgres
ARG DATABASE_USER=postgres
ARG DATABASE_PASSWORD
ARG DATABASE_HOST
ARG DATABASE_PORT=5432

ENV DATABASE_NAME=${DATABASE_NAME}
ENV DATABASE_USER=${DATABASE_USER}
ENV DATABASE_PASSWORD=${DATABASE_PASSWORD}
ENV DATABASE_HOST=${DATABASE_HOST}
ENV DATABASE_PORT=${DATABASE_PORT}
# DATABASE_NAME=postgres DATABASE_USER=postgres DATABASE_PASSWORD=senha DATABASE_HOST=localhost DATABASE_PORT=5432  ./rinhabackend.out
CMD ["./rinhabackend.out"]
