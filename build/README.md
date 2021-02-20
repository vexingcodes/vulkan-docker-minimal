# build

This directory contains a Docker image capable of building Vulkan example code from
[vulcan-tutorial.com](https://vulkan-tutorial.com/). The `run.bash` script will build the image,
spawn a container from the image with the `source` directory mounted in, and pass the arguments
given to the script through to the bash shell inside the container. To build the code you would run
`./run.bash make`.

This can act as a starting point for images with additional dependencies. This is the minimal number
of packages required to build a Vulkan program with GLFW3.
