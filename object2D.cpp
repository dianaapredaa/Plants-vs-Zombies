#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSqr(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float width,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices;

    if (!fill) {
        indices = { 0, 1, 2, 3, 0 };
        rectangle->SetDrawMode(GL_LINE_STRIP);
    }
    else {
        // For filled rectangle
        indices = { 0, 1, 2, 0, 2, 3 };
        rectangle->SetDrawMode(GL_TRIANGLES);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* object2D::Enemy(
    const std::string& name,
    glm::vec3 center,
    float size,
    glm::vec3 outerColor,
    glm::vec3 innerColor,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // add center of the hexagon
    vertices.emplace_back(VertexFormat(center, innerColor));

    // create the inner hexagon
    for (int i = 0; i < 6; ++i) {
        float angleDeg = 60.0f * i;
        float angleRad = glm::radians(angleDeg);
        float x = center.x + size * cos(angleRad) / 1.5;
        float y = center.y + size * sin(angleRad) / 1.5;

        vertices.emplace_back(VertexFormat(glm::vec3(x, y, center.z), innerColor));
    }


    // Calculate vertices of the hexagon based on the size
    for (int i = 0; i < 6; ++i) {
    float angleDeg = 60.0f * i;
    float angleRad = glm::radians(angleDeg);
    float x = center.x + size * cos(angleRad);
    float y = center.y + size * sin(angleRad);

    vertices.emplace_back(VertexFormat(glm::vec3(x, y, center.z), outerColor));
    }

    // Calculate vertices of the hexagon based on the size / 2
    for (int i = 0; i < 6; ++i) {
        float angleDeg = 60.0f * i;
        float angleRad = glm::radians(angleDeg);
        float x = center.x + size * cos(angleRad);
        float y = center.y + size * sin(angleRad);

        vertices.emplace_back(VertexFormat(glm::vec3(x, y, center.z), innerColor));
    }


    indices = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 1, 0, 7, 8, 0, 8, 9, 0, 9, 10, 0 , 10, 11, 0, 11, 12, 0, 12, 7};

    Mesh* hexagon = new Mesh(name);

    if (!fill) {
    indices.push_back(0); // Close the loop for the wireframe hexagon
    hexagon->SetDrawMode(GL_LINE_STRIP);
    }
    else {
    // Filled hexagon with triangles

    hexagon->SetDrawMode(GL_TRIANGLES);
    }

    hexagon->InitFromData(vertices, indices);

    return hexagon;
}


Mesh* object2D::CreateRhombus(
	const std::string& name,
	glm::vec3 center,
	float size,
	glm::vec3 color,
	bool fill)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;

	// Calculate vertices of the rhombus based on the size
	// Top vertex - 0
    vertices.emplace_back(VertexFormat(glm::vec3(center.x, center.y + size, center.z), color));
    // Left vertex - 1
    vertices.emplace_back(VertexFormat(glm::vec3(center.x - size / 2, center.y, center.z), color));
    // Right vertex - 2
    vertices.emplace_back(VertexFormat(glm::vec3(center.x + size / 2, center.y, center.z), color));

    // Bottom vertex - 3
    vertices.emplace_back(VertexFormat(glm::vec3(center.x, center.y - size, center.z), color));

    // Square Vertexes
     // Top right - 4
    vertices.emplace_back(VertexFormat(glm::vec3(center.x + size, center.y + size / 4, center.z), color));
    
    // Top left - 5
    vertices.emplace_back(VertexFormat(glm::vec3(center.x + size / 3, center.y + size / 4, center.z), color));

    // Bottom left - 6
    vertices.emplace_back(VertexFormat(glm::vec3(center.x + size / 3, center.y - size / 4, center.z), color));
    
    // Bottom right - 7
    vertices.emplace_back(VertexFormat(glm::vec3(center.x + size, center.y - size / 4, center.z), color));

    indices = { 2, 0, 1,
                2, 1, 3,
                4, 5, 6,
                4, 6, 7, };

	Mesh* rhombus = new Mesh(name);

	if (!fill) {
		rhombus->SetDrawMode(GL_LINE_STRIP);
	}
	else {
		// Filled rhombus with triangles
		rhombus->SetDrawMode(GL_TRIANGLES);
	}

	rhombus->InitFromData(vertices, indices);
	return rhombus;
}

Mesh* object2D::CreateStar(
    const std::string& name,
    glm::vec3 center,
    float size,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // inner radius of the star
    double innerRadius = size / 2;

    // outer radius of the star
    double outerRadius = size;

    // Calculate vertices for the star peaks
    for (int i = 0; i < 5; i++) {
		double angleDeg = 72.0 * i;
		double angleRad = glm::radians(angleDeg);
		double x = (double)center.x + outerRadius * cos(angleRad);
		double y = (double)center.y + outerRadius * sin(angleRad);

		vertices.emplace_back(VertexFormat(glm::vec3(x, y, center.z), color));
	}

    // Calculate vertices for the star in-between peaks
    for (int i = 0; i < 5; i++) {
        double angleDeg = 72.0 * i + 36.0;
        double angleRad = glm::radians(angleDeg);
        double x = (double)center.x + innerRadius * cos(angleRad);
        double y = (double)center.y + innerRadius * sin(angleRad);

		vertices.emplace_back(VertexFormat(glm::vec3(x, y, center.z), color));
	}

    indices = {
        0, 2, 8,
        0, 6, 3,
        4, 1, 7
    };

    Mesh* star = new Mesh(name);

    if (!fill) {
        star->SetDrawMode(GL_LINE_STRIP);
    }
    else {
        // Filled rhombus with triangles
        star->SetDrawMode(GL_TRIANGLES);
    }

    star->InitFromData(vertices, indices);
    return star;
}