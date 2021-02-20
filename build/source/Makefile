default: out/shaders/vert.spv out/shaders/frag.spv out/triangle

out:
	mkdir --parent out

out/shaders: | out
	mkdir --parent out/shaders

out/shaders/vert.spv: shader.vert | out/shaders
	glslc shader.vert -o $@

out/shaders/frag.spv: shader.frag | out/shaders
	glslc shader.frag -o $@

out/triangle: triangle.cpp | out
	g++ -std=c++20 -O2 -DNDEBUG -o $@ triangle.cpp -lglfw -lvulkan

.PHONY: clean
clean:
	rm -rf out/shaders out/triangle
