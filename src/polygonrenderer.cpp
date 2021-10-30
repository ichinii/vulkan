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
	glm::vec2 p0, glm::vec3 c0, glm::vec2 t0,
	glm::vec2 p1, glm::vec3 c1, glm::vec2 t1,
	glm::vec2 p2, glm::vec3 c2, glm::vec2 t2)
{
	m_vertices.emplace_back(p0, c0, t0);
	m_vertices.emplace_back(p1, c1, t1);
	m_vertices.emplace_back(p2, c2, t2);
}

void PolygonRenderer::drawCircle(glm::vec2 p, float r, glm::vec3 c)
{
	auto divisions = 32.f;
	for (int i = 0; i < divisions; ++i) {
		auto pi2 = 2.f * glm::pi<float>();
		auto a = i / (divisions - 1.f) * pi2;
		auto a2 = (i+1) / (divisions - 1.f) * pi2;
		auto dir = glm::vec2(glm::cos(a), glm::sin(a));
		auto dir2 = glm::vec2(glm::cos(a2), glm::sin(a2));
		auto pos = r * dir;
		auto pos2 = r * dir2;
		m_vertices.push_back(DeferredGeometryPipeline::Vertex{p, c * .5f, {.5, .5}});
		m_vertices.push_back(DeferredGeometryPipeline::Vertex{p + pos, c, dir * .5f + .5f});
		m_vertices.push_back(DeferredGeometryPipeline::Vertex{p + pos2, c, dir2 * .5f + .5f});
	}
}
