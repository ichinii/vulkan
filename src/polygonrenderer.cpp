#include "polygonrenderer.h"
#include <glm/ext/scalar_constants.hpp>

PolygonRenderer::PolygonRenderer()
{

}

PolygonRenderer::~PolygonRenderer()
{

}

void PolygonRenderer::clear()
{
	m_vertices.clear();
}

DeferredGeometryPipeline::Vertices PolygonRenderer::flush()
{
	return std::move(m_vertices);
}

void PolygonRenderer::drawTriangle(
	glm::vec3 p0, glm::vec3 c0, glm::vec2 t0,
	glm::vec3 p1, glm::vec3 c1, glm::vec2 t1,
	glm::vec3 p2, glm::vec3 c2, glm::vec2 t2)
{
	auto n = glm::normalize(glm::cross(p1 - p0, p2 - p0));
	m_vertices.emplace_back(p0, c0, t0, n);
	m_vertices.emplace_back(p1, c1, t1, n);
	m_vertices.emplace_back(p2, c2, t2, n);
}

void PolygonRenderer::drawCircle(glm::vec3 p, float r, glm::vec3 c)
{
	auto divisions = 128.f;
	for (int i = 0; i < divisions; ++i) {
		auto pi2 = 2.f * glm::pi<float>();
		auto a = i / (divisions - 1.f) * pi2;
		auto a2 = (i+1) / (divisions - 1.f) * pi2;
		auto dir = glm::vec2(glm::cos(a), glm::sin(a));
		auto dir2 = glm::vec2(glm::cos(a2), glm::sin(a2));
		auto pos = glm::vec3(r * dir, 0);
		auto pos2 = glm::vec3(r * dir2, 0);
		auto n = glm::normalize(glm::cross(pos, pos2));
		m_vertices.push_back(DeferredGeometryPipeline::Vertex{p, c * .5f, {.5, .5}, n});
		m_vertices.push_back(DeferredGeometryPipeline::Vertex{p + pos, c, dir * .5f + .5f, n});
		m_vertices.push_back(DeferredGeometryPipeline::Vertex{p + pos2, c, dir2 * .5f + .5f, n});
	}
}

#include <iostream>
void PolygonRenderer::drawSphere(glm::vec3 p, float r, glm::vec3 c, bool smooth)
{
	const auto vertical_divisions = 64;
	const auto horizontal_divisions = vertical_divisions * 2;

	auto nextHorizontalIndex = [=] (glm::ivec2 i) {
		return glm::ivec2((i.x + 1) % horizontal_divisions, i.y);
	};

	auto nextVerticalIndex = [=] (glm::ivec2 i) {
		return glm::ivec2(i.x, i.y + 1);
	};

	auto normalizedIndex = [=] (glm::ivec2 i) {
		return glm::vec2(i.x, i.y) / glm::vec2(horizontal_divisions, vertical_divisions);
	};

	auto getNormals = [=] (glm::vec3 pos, glm::vec3 pos_x, glm::vec3 pos_y, glm::vec3 pos_xy) {
		if (smooth) {
			pos = glm::normalize(pos);
			pos_x = glm::normalize(pos_x);
			pos_y = glm::normalize(pos_y);
			pos_xy = glm::normalize(pos_xy);
			return std::make_tuple(pos, pos_x, pos_y, pos_xy);
		}
		else {
			auto n = glm::normalize(glm::cross(pos_x - pos, pos_y - pos));
			return std::make_tuple(n, n, n, n);
		}
	};

	auto getPosition = [=] (glm::ivec2 i) {
		auto normalized = normalizedIndex(i);
		auto radians = normalized * glm::pi<float>() * glm::vec2(2, 1);
		auto x = glm::sin(radians.y) * glm::cos(radians.x);
		auto z = glm::sin(radians.y) * glm::sin(radians.x);
		auto y = glm::cos(radians.y);
		return glm::vec3(x, y, z) * r;
	};

	for (int y = 0; y < vertical_divisions; ++y) {
		for (int x = 0; x < horizontal_divisions; ++x) {
			auto index = glm::ivec2(x, y);
			auto index_x = nextHorizontalIndex(index);
			auto index_y = nextVerticalIndex(index);
			auto index_xy = nextVerticalIndex(index_x);
			auto pos = getPosition(index);
			auto pos_x = getPosition(index_x);
			auto pos_y = getPosition(index_y);
			auto pos_xy = getPosition(index_xy);
			auto uv = normalizedIndex(index);
			auto uv_x = normalizedIndex(index_x);
			auto uv_y = normalizedIndex(index_y);
			auto uv_xy = normalizedIndex(index_xy);
			auto [normal, normal_x, normal_y, normal_xy] = getNormals(pos, pos_x, pos_y, pos_xy);

			m_vertices.emplace_back(p + pos, c, uv, normal);
			m_vertices.emplace_back(p + pos_x, c, uv_x, normal_x);
			m_vertices.emplace_back(p + pos_xy, c, uv_xy, normal_xy);

			m_vertices.emplace_back(p + pos, c, uv, normal);
			m_vertices.emplace_back(p + pos_xy, c, uv_xy, normal_xy);
			m_vertices.emplace_back(p + pos_y, c, uv_y, normal_y);
		}
	}
}
