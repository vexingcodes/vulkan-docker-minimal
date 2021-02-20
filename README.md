# docker-vulkan-minimal

Minimal environment to run the Vulkan triangle
[example](https://vulkan-tutorial.com/Drawing_a_triangle/Swap_chain_recreation) with software
rendering in a Docker container requiring no privileges, running processes only as a non-root user,
and outputting via a VNC session accessible over an HTML5 web interface. The top-level `run.bash`
script will build everything and run it.

Why? Mostly just looking for the bare minimum required to build and run Vulkan programs in an
isolated environment with no elevated privileges, no hardware guarantees and the only software
dependency on the host being Docker. Within the docker container, we only rely on official Debian
packages and otherwise build from GitHub hosted sources. This repo is probably not particularly
useful in itself.

## Details

Two Docker images are used. One to build the resources, the other to run the resources. A combined
image could be made, but this works and makes it obvious what are build dependences versus run
dependencies.

The `vulkan-build` container defined in the `build` directory is used to, oddly enough, build the
resources. It's a simple GNU makefile that compiles the shaders and the C++ code. The output of the
compilation goes in `build/source/out`. Running the build is accomplished with `./build/run.bash`.

The `vulkan-run` container defined in the `run` directory is used to run the generated code in a
virtual desktop served on a browser-accessible VNC session that should be hosted at the URL printed
by the `run/run.bash` script.

## License

All code in this repository, including the code that comes from vulkan-tutorial.com is under the
[CC0 1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/) license. See the
Overv/VulkanTutorial [README.md](https://github.com/Overv/VulkanTutorial/README.md#license) for more
licensing information on the third-party code.
