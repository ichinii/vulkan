
#include <iostream>
#include <optional>
#include <ranges>
#include <functional>
#include <variant>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tinygltf/tiny_gltf.h"

#include "gltf.h"

using namespace tinygltf;
using std::ranges::range;

using Position = decltype(DeferredGeometryPipeline::Vertex::pos);
using Normal = decltype(DeferredGeometryPipeline::Vertex::normal);
using Uv = decltype(DeferredGeometryPipeline::Vertex::uv);
using Color = decltype(DeferredGeometryPipeline::Vertex::color);

auto loadModel(const char* filepath) {
	Model model;
	std::string err;
	std::string warn;

	TinyGLTF loader;
	auto result = loader.LoadASCIIFromFile(&model, &err, &warn, filepath);

	if (!result)
		std::cout << "failed to load gltf file: " << filepath << std::endl;
	if (!err.empty())
		std::cout << "gltf error: " << err << std::endl;
	if (!warn.empty())
		std::cout << "gltf warning: " << warn << std::endl;

	return result ? std::optional(model) : std::nullopt;
}

using Indices = std::map<std::string, std::size_t>;

auto getAttributeAccessorIndices(auto& attributes) {
	Indices indices;

	for (auto& attribute : attributes) {
		auto name = static_cast<std::string>(attribute.first);
		auto index = attribute.second;
		std::transform(name.begin(), name.end(), name.begin(), ::tolower);
		std::cout << "attribute: " << name << std::endl;
		indices[name] = index;
	}

	return indices;
}

template <typename T>
auto getAccessorBufferIter(Model& model, Accessor& accessor) {
	auto& bufferView = model.bufferViews[accessor.bufferView];
	auto& buffer = model.buffers[bufferView.buffer];
	auto offset = accessor.byteOffset + bufferView.byteOffset;
	std::size_t stride = GetComponentSizeInBytes(accessor.componentType) * GetNumComponentsInType(accessor.type);

	using namespace std::views;
	return
		iota(0ul, accessor.count) |
		transform([=] (auto index) { return index * stride + offset; }) |
		transform([&] (auto index) { return *reinterpret_cast<T*>(&buffer.data[index]); });
}

template <typename T>
auto getAttributeBufferIter(Model& model, Indices& indices, const std::string& name) {
	if (!indices.contains(name))
		std::cout << "error: there no such attribute with name: " << name << std::endl;
	
	auto& accessor = model.accessors[indices[name]];
	return getAccessorBufferIter<T>(model, accessor);
}

template <range T0, range T1>
auto createRangeVariant(T1 t) {
	return std::variant<T0, T1>(t);
}

template <typename T>
auto getAttributeBufferIterOr(Model& model, Indices& indices, const std::string& name, T fallback) {
#define Iter getAttributeBufferIter<T>(model, indices, name)
	using namespace std::views;
	auto v = createRangeVariant<decltype(Iter)>(iota(0) | transform([=] (auto) { return fallback; }));

	if (indices.contains(name))
		v = Iter;

	return v;

#undef Iter
}

// TODO: zip the iters
template <range P, typename C, range U, range N>
auto collectVertices(P positions, C colors, U uvs, N normals) {
	auto vertices = DeferredGeometryPipeline::Vertices();

	for (auto pos : positions) {
		vertices.emplace_back();
		vertices.back().pos = pos;
	}

	auto size = vertices.size();

	std::visit([&, i=0ul] (auto colors) mutable {
		for (auto color : colors) {
			vertices[i++].color = color;
			if (i == size)
				break;
		}
	}, colors);

	{
		std::size_t i = 0;
		for (auto uv : uvs) {
			vertices[i++].uv = uv;
		}
	}

	{
		std::size_t i = 0;
		for (auto normal : normals) {
			vertices[i++].normal = normal;
		}
	}

	return vertices;
}

auto collectVerticesIndexed(DeferredGeometryPipeline::Vertices& vertices, std::vector<std::uint16_t> indices) {
	auto theRealVertices = DeferredGeometryPipeline::Vertices();
	theRealVertices.reserve(indices.size());
	for (auto i : indices) {
		theRealVertices.push_back(vertices[i]);
	}
	return theRealVertices;
}

std::optional<::Mesh> loadMeshFromGltfFile(const char* filepath) {
	if (auto modelOpt = loadModel(filepath); modelOpt) {
		auto& model = *modelOpt;
		
		std::cout << "loading gltf model" << std::endl;
		std::cout << "meshes: " << model.meshes.size() << std::endl;
		for (auto& mesh : model.meshes) {
			auto primitives = std::vector<DeferredGeometryPipeline::Vertices>();
			for (auto& primitive : mesh.primitives) {
				if (primitive.mode != TINYGLTF_MODE_TRIANGLES)
					continue;

				auto& indexAccessor = model.accessors[primitive.indices];
				auto indexes = getAccessorBufferIter<glm::uint16_t>(model, indexAccessor);
				auto ind = std::vector<std::uint16_t>();
				for (auto i : indexes)
					ind.push_back(i);

				auto indices = getAttributeAccessorIndices(primitive.attributes);

				auto positions = getAttributeBufferIter<Position>(model, indices, "position");
				auto colors = getAttributeBufferIterOr(model, indices, "color", glm::vec3(1, 1, 1));
				auto uvs = getAttributeBufferIter<Uv>(model, indices, "texcoord_0");
				auto normals = getAttributeBufferIter<Normal>(model, indices, "normal");
				auto vertices = collectVertices(positions, colors, uvs, normals);
				auto theRealVertices = collectVerticesIndexed(vertices, ind);

				auto materialSrc = model.materials[primitive.material];
				auto pbr = materialSrc.pbrMetallicRoughness;
				auto roughness = pbr.roughnessFactor;
				auto metallic = pbr.metallicFactor;
				auto material = ::Material { roughness, metallic };

				std::cout << "metallic: " << metallic << std::endl
					<< "roughness: " << roughness << std::endl;

				auto texture = model.textures[pbr.baseColorTexture.index];
				std::cout << texture.source << "   " << model.images.size() << std::endl;
				auto& imageSrc = model.images[texture.source];
				auto imageSize = glm::uvec2(imageSrc.width, imageSrc.height);
				auto image = ::Image {
					std::vector<ImageColor>(imageSrc.image.size()),
					imageSize,
				};
				std::memcpy(image.colors.data(), imageSrc.image.data(), imageSrc.image.size());
				auto sampler = model.samplers[texture.source];

				return ::Mesh {
					theRealVertices,
					material,
					image,
				};
			}
		}
	}

	return std::nullopt;
}
