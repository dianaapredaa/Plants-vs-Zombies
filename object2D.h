#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSqr(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float width, float hight, glm::vec3 color, bool fill = false);
    Mesh* Enemy(const std::string& name, glm::vec3 center, float length, glm::vec3 outerColor, glm::vec3 innerColor, bool fill = false);
    Mesh* CreateRhombus(const std::string& name, glm::vec3 center, float size, glm::vec3 color, bool fill);
    Mesh* CreateStar(const std::string& name, glm::vec3 center, float size, glm::vec3 color, bool fill);
}
