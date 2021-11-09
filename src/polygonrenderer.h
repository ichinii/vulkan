#pragma once

#include "deferredgeometrypipeline.h"

class PolygonRenderer {
public:
	PolygonRenderer();
	~PolygonRenderer();

	void clear();
	DeferredGeometryPipeline::Vertices flush();

	void drawTriangle(
		glm::vec3 p0, glm::vec3 c0, glm::vec2 t0,
		glm::vec3 p1, glm::vec3 c1, glm::vec2 t1,
		glm::vec3 p2, glm::vec3 c2, glm::vec2 t2);

	void drawCircle(glm::vec3 p, float r, glm::vec3 c);

	void drawSphere(glm::vec3 p, float r, glm::vec3 c, bool smooth);

	void drawMesh(const DeferredGeometryPipeline::Vertices& vertices);

private:
	DeferredGeometryPipeline::Vertices m_vertices;
};
