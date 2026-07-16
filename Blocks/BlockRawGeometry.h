#pragma once
#include <map>
#include <array>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include "BlockFaceDirection.h"

struct BlockRawGeometry
{
    inline static const std::array<std::array<glm::vec3, 4>, 6> cubeFaces =
    {
        //Front
        std::array<glm::vec3, 4>{
            glm::vec3(-0.5f, 0.5f, 0.5f),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(0.5f, -0.5f, 0.5f),
            glm::vec3(-0.5f, -0.5f, 0.5f)
        },
        //Back
        std::array<glm::vec3, 4>
        {
            glm::vec3(0.5f, 0.5f, -0.5f),
            glm::vec3(-0.5f, 0.5f, -0.5f),
            glm::vec3(-0.5f, -0.5f, -0.5f),
            glm::vec3(0.5f, -0.5f, -0.5f)
        },
        //Left
        std::array<glm::vec3, 4>
        {
            glm::vec3(-0.5f, 0.5f, -0.5f),
            glm::vec3(-0.5f, 0.5f, 0.5f),
            glm::vec3(-0.5f, -0.5f, 0.5f),
            glm::vec3(-0.5f, -0.5f, -0.5f)
        },
        //Right
        std::array<glm::vec3, 4>
        {
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(0.5f, 0.5f, -0.5f),
            glm::vec3(0.5f, -0.5f, -0.5f),
            glm::vec3(0.5f, -0.5f, 0.5f)
        },
        //Top
        std::array<glm::vec3, 4>
        {
            glm::vec3(-0.5f, 0.5f, -0.5f),
            glm::vec3(0.5f, 0.5f, -0.5f),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(-0.5f, 0.5f, 0.5f)
        },
        //Bottom
        std::array<glm::vec3, 4>
        {
            glm::vec3(-0.5f, -0.5f, 0.5f),
            glm::vec3(0.5f, -0.5f, 0.5f),
            glm::vec3(0.5f, -0.5f, -0.5f),
            glm::vec3(-0.5f, -0.5f, -0.5f),
        }
    };
};
