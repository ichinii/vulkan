#pragma once

#include "polygonpipeline.h"

class PolygonRenderer {
public:
	PolygonRenderer();
	~PolygonRenderer();

	void clear();
	PolygonPipeline::Vertices flush();

	void drawTriangle(
		glm::vec2 p0, glm::vec3 c0, glm::vec2 t0,
		glm::vec2 p1, glm::vec3 c1, glm::vec2 t1,
		glm::vec2 p2, glm::vec3 c2, glm::vec2 t2);
	void drawCircle(glm::vec2 p, float r, glm::vec3 c);

private:
	PolygonPipeline::Vertices m_vertices;
};
