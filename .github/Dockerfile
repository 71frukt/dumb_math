# задаем базовый образ
FROM ubuntu:22.04

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
    && rm -rf /var/lib/apt/lists/*