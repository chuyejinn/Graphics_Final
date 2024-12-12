#include "objects.h"
#include "glad.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// 도로 그리기
void drawRoad() {
    float roadVertices[] = {
        -2.0f, -1.0f, 0.0f,  // 왼쪽 아래
         2.0f, -1.0f, 0.0f,  // 오른쪽 아래
         2.0f, -0.7f, 0.0f,  // 오른쪽 위
        -2.0f, -0.7f, 0.0f   // 왼쪽 위
    };

    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(roadVertices), roadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

// 가로수 줄기
void drawTreeTrunk(float x) {
    float trunkVertices[] = {
        x - 0.05f, -0.7f, 0.0f,  // 왼쪽 아래
        x + 0.05f, -0.7f, 0.0f,  // 오른쪽 아래
        x + 0.05f, -0.5f, 0.0f,  // 오른쪽 위
        x - 0.05f, -0.5f, 0.0f   // 왼쪽 위
    };

    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trunkVertices), trunkVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

// 가로수 잎
void drawTreeLeaves(float x) {
    const int segments = 30;  // 다각형 세그먼트 수
    const float radii[][2] = {
        { 0.15f, 0.1f },  // 중앙 타원
        { 0.12f, 0.08f }, // 왼쪽 작은 타원
        { 0.12f, 0.08f }, // 오른쪽 작은 타원
        { 0.1f, 0.07f }   // 위쪽 작은 타원
    };

    const glm::vec2 offsets[] = {
        { 0.0f, 0.0f },    // 중앙 타원
        { -0.12f, 0.05f }, // 왼쪽
        { 0.12f, 0.05f },  // 오른쪽
        { 0.0f, 0.12f }    // 위쪽
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);

    for (int i = 0; i < 4; ++i) {  // 4개의 타원으로 잎 구성
        float vertices[(segments + 1) * 3];
        unsigned int indices[segments * 3];

        for (int j = 0; j < segments; ++j) {
            float angle = 2.0f * M_PI * j / segments;
            vertices[j * 3 + 0] = x + offsets[i].x + radii[i][0] * cos(angle);  // x 좌표
            vertices[j * 3 + 1] = -0.45f + offsets[i].y + radii[i][1] * sin(angle);  // y 좌표
            vertices[j * 3 + 2] = 0.0f;                                         // z 좌표

            indices[j * 3 + 0] = j;
            indices[j * 3 + 1] = (j + 1) % segments;
            indices[j * 3 + 2] = segments;  // 중심점
        }

        // 중심점 추가
        vertices[segments * 3 + 0] = x + offsets[i].x;
        vertices[segments * 3 + 1] = -0.45f + offsets[i].y;
        vertices[segments * 3 + 2] = 0.0f;

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glDrawElements(GL_TRIANGLES, segments * 3, GL_UNSIGNED_INT, 0);

        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    glDeleteVertexArrays(1, &VAO);
}

// 가로등 등불
void drawLampLight(float x) {
    const int segments = 30;  // 다각형 세그먼트 수
    const float radiusX = 0.1f;  // 가로 반지름
    const float radiusY = 0.05f;  // 세로 반지름
    const float centerY = -0.4f; // 중심 높이

    float vertices[(segments + 1) * 3];
    unsigned int indices[segments * 3];

    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        vertices[i * 3 + 0] = x + radiusX * cos(angle);  // x 좌표
        vertices[i * 3 + 1] = centerY + radiusY * sin(angle);  // y 좌표
        vertices[i * 3 + 2] = 0.0f;  // z 좌표

        indices[i * 3 + 0] = i;
        indices[i * 3 + 1] = (i + 1) % segments;
        indices[i * 3 + 2] = segments;  // 중심점
    }

    // 중심점 추가
    vertices[segments * 3 + 0] = x;
    vertices[segments * 3 + 1] = centerY;
    vertices[segments * 3 + 2] = 0.0f;

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawElements(GL_TRIANGLES, segments * 3, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void drawLampPost(float x) {
    float lampPostVertices[] = {
        x - 0.02f, -0.7f, 0.0f,  // 왼쪽 아래
        x + 0.02f, -0.7f, 0.0f,  // 오른쪽 아래
        x + 0.02f, -0.4f, 0.0f,  // 오른쪽 위
        x - 0.02f, -0.4f, 0.0f   // 왼쪽 위
    };

    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lampPostVertices), lampPostVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void drawCloud(float x, float y) {
    const int segments = 30;  // 다각형 세그먼트 수
    const float radii[][2] = {
        { 0.2f, 0.15f },  // 중앙 타원
        { 0.15f, 0.1f },  // 왼쪽 작은 타원
        { 0.18f, 0.12f }, // 오른쪽 작은 타원
        { 0.12f, 0.08f }  // 위쪽 작은 타원
    };

    const glm::vec2 offsets[] = {
        { 0.0f, 0.0f },      // 중앙 타원
        { -0.22f, -0.05f },  // 왼쪽 아래
        { 0.22f, -0.05f },   // 오른쪽 아래
        { 0.0f, 0.1f }       // 위쪽
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);

    for (int i = 0; i < 4; ++i) {  // 4개의 타원으로 구름 구성
        float vertices[(segments + 1) * 3];
        unsigned int indices[segments * 3];

        for (int j = 0; j < segments; ++j) {
            float angle = 2.0f * M_PI * j / segments;
            vertices[j * 3 + 0] = x + offsets[i].x + radii[i][0] * cos(angle);  // x 좌표
            vertices[j * 3 + 1] = y + offsets[i].y + radii[i][1] * sin(angle);  // y 좌표
            vertices[j * 3 + 2] = 0.0f;                                        // z 좌표

            indices[j * 3 + 0] = j;
            indices[j * 3 + 1] = (j + 1) % segments;
            indices[j * 3 + 2] = segments;  // 중심점
        }

        // 중심점 추가
        vertices[segments * 3 + 0] = x + offsets[i].x;
        vertices[segments * 3 + 1] = y + offsets[i].y;
        vertices[segments * 3 + 2] = 0.0f;

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glDrawElements(GL_TRIANGLES, segments * 3, GL_UNSIGNED_INT, 0);

        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    glDeleteVertexArrays(1, &VAO);
}