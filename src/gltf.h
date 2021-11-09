#pragma once

#include <optional>
#include "deferredgeometrypipeline.h"
#include "texture.h"

struct Material {
	double roughness;
	double metallic;
};

struct Mesh {
	DeferredGeometryPipeline::Vertices vertices;
	Material material;
	Image albedo;
};

extern std::optional<Mesh> loadMeshFromGltfFile(const char* filepath);
