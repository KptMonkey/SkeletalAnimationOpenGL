#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera
{
    Camera(glm::vec3 position, glm::vec3 lookAt)
        : Position(position)
        , LookAt(lookAt) {

        View = glm::lookAt(Position,
                           LookAt,
                           glm::vec3(0.0f, 1.0f, 0.0f));

        Projection = glm::perspective( 45.0f, 0.75f, 0.1f, 500.0f );
    }

    glm::vec3 Position;
    glm::vec3 LookAt;
    glm::mat4 View;
    glm::mat4 Projection;
};
