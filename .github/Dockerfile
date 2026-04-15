# задаем базовый образ
FROM ubuntu:24.04

# отключаем интерактивные запросы
ENV DEBIAN_FRONTEND=noninteractive

# устанавливаем часовой пояс
ENV TZ=Etc/UTC

# установка пакетов и очистка кэша в один слой
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libfmt-dev \
    libmpfr-dev \
    libgmp-dev \
    graphviz \
    gnuplot \
    python3 \
    python3-pip \
    python3-scipy \
    python3-matplotlib \
    python3-numpy \
    wget \
    && wget https://archives.boost.io/release/1.84.0/source/boost_1_84_0.tar.gz \
    && tar -xzf boost_1_84_0.tar.gz \
    && cp -r boost_1_84_0/boost /usr/local/include/ \
    && rm -rf boost_1_84_0 boost_1_84_0.tar.gz \
    && rm -rf /var/lib/apt/lists/*

LABEL org.opencontainers.image.source="https://github.com/71frukt/dumb_math.git"
