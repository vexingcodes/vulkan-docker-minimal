#!/usr/bin/env bash
set -eux

# Work out of the directory containing this script.
cd "$(dirname "${0}")"

# Build and run the contianer, which will either open a shell (default) or forward the arguments
# passed to this as a command to the entrypoint of the container, which is "bash -c"
docker build . --tag vulkan-build
exec docker run \
  --hostname vulkan-build \
  --interactive \
  --name vulkan-build \
  --tty \
  --rm \
  --volume "$(pwd)/..:/home/dev/source" \
  --workdir /home/dev/source/build/source \
  vulkan-build "${@}"
