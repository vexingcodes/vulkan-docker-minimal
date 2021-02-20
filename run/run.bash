#!/usr/bin/env bash
set -eux

# Work out of the directory containing this script.
cd "$(dirname "${0}")"

# Build and run the container (killing any existing one), and wait for it to start up.
docker build . --tag vulkan-run
docker rm --force vulkan-run
docker run  \
  --detach \
  --hostname vulkan-run \
  --name vulkan-run \
  --volume "$(pwd)/../build/source/out:/home/dev/vulkan:ro" \
  vulkan-run

# Get the IP address of the container from within the container. This allows us to not have to open
# a port on the host for the VNC server, since we are trying to keep the minimum possible number of
# dependencies and privileges required.
IP_ADDR="$(docker exec vulkan-run bash -c "ip addr show dev eth0 | grep 'inet ' | awk '{print \$2}' | cut -d/ -f1")"
VNC_URL="http://${IP_ADDR}:8080"

# Output the URL, and if we can, open the browser window.
set +x
echo
echo "VNC URL: ${VNC_URL}"
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  if command -v xdg-open &> /dev/null; then
    echo "Opening browser to VNC URL in 3 seconds..."
    sleep 3 # Yuck, just give some time for things to start up.
    xdg-open "${VNC_URL}"
  fi
fi
