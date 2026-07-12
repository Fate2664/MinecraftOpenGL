#pragma once
#include <map>
#include <array>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

enum class BlockType
{
    Empty,
    Dirt,
    Grass
};

enum class BlockFaceDirection
{
    Front,
    Back,
    Left,
    Right,
    Top,
    Bottom
};

struct BlockFaceData
{
public:
    std::array<glm::vec3, 4> vertices;
    std::array<glm::vec2, 4> uv;
};

struct BlockVertexDataRaw
{
public:
    inline static std::map<BlockFaceDirection, std::array<glm::vec3, 4>> blockVertexData = {
        {
            BlockFaceDirection::Front,
            {
                glm::vec3(-0.5f, 0.5f, 0.5f),
                glm::vec3(0.5f, 0.5f, 0.5f),
                glm::vec3(0.5f, -0.5f, 0.5f),
                glm::vec3(-0.5f, -0.5f, 0.5f)
            }
        },
        {
            BlockFaceDirection::Back,
            {
                glm::vec3(0.5f, 0.5f, -0.5f),
                glm::vec3(-0.5f, 0.5f, -0.5f),
                glm::vec3(-0.5f, -0.5f, -0.5f),
                glm::vec3(0.5f, -0.5f, -0.5f)
            }
        },
        {
            BlockFaceDirection::Left,
            {
                glm::vec3(-0.5f, 0.5f, -0.5f),
                glm::vec3(-0.5f,  0.5f, 0.5f),
                glm::vec3(-0.5f, -0.5f,  0.5f),
                glm::vec3(-0.5f, -0.5f, -0.5f)
            }
        },
        {
            BlockFaceDirection::Right,
            {
                glm::vec3(0.5f, 0.5f,  0.5f),
                glm::vec3(0.5f, 0.5f,  -0.5f),
                glm::vec3(0.5f, -0.5f, -0.5f),
                glm::vec3(0.5f, -0.5f,  0.5f)
            }
        },
        {
            BlockFaceDirection::Top,
            {
                glm::vec3(-0.5f, 0.5f, -0.5f),
                glm::vec3( 0.5f,  0.5f, -0.5f),
                glm::vec3( 0.5f, 0.5f,  0.5f),
                glm::vec3(-0.5f, 0.5f,  0.5f)
            }
        },
        {
            BlockFaceDirection::Bottom,
            {
                glm::vec3(-0.5f, -0.5f,  0.5f),
                glm::vec3( 0.5f, -0.5f, 0.5f),
                glm::vec3( 0.5f, -0.5f, -0.5f),
                glm::vec3(-0.5f, -0.5f, -0.5f),
            }
        }
    };
};
