# Build swiftshader in its own build stage to keep the final image size down.
FROM debian:sid as swiftshader-build

# Install swiftshader build dependencies.
RUN apt-get update \
 && DEBIAN_FRONTEND=noninteractive apt-get install --yes --no-install-recommends \
      build-essential \
      ca-certificates \
      cmake \
      git \
      libx11-dev \
      libx11-xcb-dev \
      libxext-dev \
      python3 \
      zlib1g-dev \
 && rm -rf /var/lib/apt/lists/*

# Build swiftshader. Unfortunately, there are no tagged releases as of writing this Dockerfile. The
# commit used is the latest commit as of 02/19/2021.
RUN git clone https://swiftshader.googlesource.com/SwiftShader \
 && cd SwiftShader \
 && git checkout d85a7a27b27269741a779a72f33436b83d98687c \
 && cmake -Bbuild \
      -DSWIFTSHADER_BUILD_EGL=OFF \
      -DSWIFTSHADER_BUILD_GLESv2=OFF \
      -DSWIFTSHADER_BUILD_GLES_CM=OFF \
      -DSWIFTSHADER_BUILD_PVR=OFF \
      -DSWIFTSHADER_BUILD_TESTS=OFF \
 && cmake --build build

# Fix up the path in the ICD file so it can find the shared object in the normal place. Do it here
# to avoid making another layer for no reason in the final image.
RUN sed -i 's|./libvk_swiftshader.so|libvk_swiftshader.so|' /SwiftShader/build/Linux/vk_swiftshader_icd.json

# Create an image that runs novnc as a non-privileged user when the container starts. This is the
# bare minimum, there's not even a window manager. Windows launched by applications will show up,
# but they have no controls and you can't move/resize them.
FROM debian:sid as vnc

# Install dependencies to run novnc on an X Virtual Framebuffer (xvfb).
RUN apt-get update \
 && DEBIAN_FRONTEND=noninteractive apt-get install --yes --no-install-recommends \
      novnc \
      supervisor \
      x11vnc \
      xvfb \
 && rm -rf /var/lib/apt/lists/*

# Force vnc_lite.html to be used for novnc, to avoid having the directory listing page.
# Additionally, turn off the control bar. Just run fullscreen with no distractions.
RUN ln -s /usr/share/novnc/vnc_lite.html /usr/share/novnc/index.html \
 && sed -i 's/display:flex/display:none/' /usr/share/novnc/app/styles/lite.css

# Configure supervisord to start when the container starts.
COPY supervisord.conf /etc/supervisor/
ENTRYPOINT [ "supervisord", "-c", "/etc/supervisor/supervisord.conf" ]

# Launch the VNC service when the container starts.
COPY vnc.conf /etc/supervisor/conf.d/

# Set a default display and resolution.
ENV DISPLAY=:0.0 \
    DISPLAY_SETTINGS=1024x768x24

# Add a window manager.
FROM vnc as xfce

# Install xfce.
RUN apt-get update \
 && DEBIAN_FRONTEND=noninteractive apt-get install --yes --no-install-recommends \
      dbus-x11 \
      xfce4 \
      xfce4-terminal \
 && rm -rf /var/lib/apt/lists/*

COPY xfce.conf /etc/supervisor/conf.d/

# Create an image that can run the Vulkan program compiled by this repository.
FROM xfce as vulkan-run

# Put the SwiftShader files in place so we can run Vulkan programs without a GPU.
COPY --from=swiftshader-build /SwiftShader/build/Linux/libvk_swiftshader.so /usr/lib/x86_64-linux-gnu/
COPY --from=swiftshader-build /SwiftShader/build/Linux/vk_swiftshader_icd.json /etc/vulkan/icd.d/

# Install other packages required to run. We require iproute2 so we don't even have to open a port
# on the host to make the VNC session accessible.
RUN apt-get update \
 && DEBIAN_FRONTEND=noninteractive apt-get install --yes --no-install-recommends \
      iproute2 \
      libglfw3 \
      libspdlog1 \
      libunwind8 \
 && rm -rf /var/lib/apt/lists/*

COPY triangle.conf /etc/supervisor/conf.d/

# Run everything as standard user/group dev.
RUN groupadd dev \
 && useradd --create-home --shell /bin/bash --gid dev dev
WORKDIR /home/dev
USER dev
RUN mkdir supervisor
