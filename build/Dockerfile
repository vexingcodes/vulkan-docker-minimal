FROM debian:sid

# Install build depdency Debian packages.
RUN apt-get update \
 && DEBIAN_FRONTEND=noninteractive apt-get install --yes --no-install-recommends \
      build-essential \
      ca-certificates \
      cmake \
      git \
      libglfw3-dev \
      libvulkan-dev \
      ninja-build \
      python3 \
      wget \
 && rm -rf /var/lib/apt/lists/*

# Build and install shaderc from source.
RUN wget -O shaderc.tar.gz https://github.com/google/shaderc/archive/v2020.5.tar.gz \
 && tar --extract --file shaderc.tar.gz \
 && cd shaderc-* \
 && ./utils/git-sync-deps \
 && cmake -Bbuild -GNinja -DCMAKE_BUILD_TYPE=Release -DSHADERC_SKIP_TESTS=ON -DSHADERC_SKIP_EXAMPLES=ON \
 && cmake --build build --target install \
 && rm -rf /shaderc*

# TODO MOVE UP!
RUN apt-get update \
 && DEBIAN_FRONTEND=noninteractive apt-get install --yes --no-install-recommends \
      doxygen \
      gdb \
      googletest \
      graphviz \
      lcov \
      libspdlog-dev \
      libunwind-dev \
      valgrind \
 && rm -rf /var/lib/apt/lists/*

RUN cd /usr/src/googletest \
 && cmake -B build -DCMAKE_BUILD_TYPE=Release \
 && cmake --build build --target install \
 && rm -rf build

# Run everything as standard user/group dev.
RUN groupadd dev \
 && useradd --create-home --shell /bin/bash --gid dev dev
WORKDIR /home/dev
USER dev
