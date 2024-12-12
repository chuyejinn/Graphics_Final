#include "src/shader.h"
#include "src/objects.h"
#include "src/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// 콜백 함수
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);  // 화면 크기 설정
}

// 물체 렌더링 전처리
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// 셰이더 코드
const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    uniform mat4 projection;

    void main() {
        gl_Position = projection * vec4(aPos, 1.0);
    }
)";

const char* fragmentShaderSourceGray = R"(
    #version 330 core
    out vec4 FragColor;

    void main() {
        FragColor = vec4(0.2, 0.2, 0.2, 1.0); // 회색 (도로, 가로등 기둥)
    }
)";

const char* fragmentShaderSourceBrown = R"(
    #version 330 core
    out vec4 FragColor;

    void main() {
        FragColor = vec4(0.5, 0.25, 0.1, 1.0); // 갈색 (가로수 줄기)
    }
)";

const char* fragmentShaderSourceGreen = R"(
    #version 330 core
    out vec4 FragColor;

    void main() {
        FragColor = vec4(0.0, 0.8, 0.0, 1.0); // 초록색 (가로수 잎)
    }
)";

const char* fragmentShaderSourceYellow = R"(
    #version 330 core
    out vec4 FragColor;

    void main() {
        FragColor = vec4(1.0, 1.0, 0.0, 1.0); // 노란색 (가로등 등불)
    }
)";

const char* fragmentShaderSourceWhite = R"(
    #version 330 core
    out vec4 FragColor;

    void main() {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0); // 흰색 (구름)
    }
)";

int main() {
    // GLFW 초기화
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Everyday Scene", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLAD 초기화
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 배경색 설정
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f); // 하늘색 배경

    // 셰이더 프로그램 생성
    unsigned int grayShader = createShader(vertexShaderSource, fragmentShaderSourceGray);
    unsigned int brownShader = createShader(vertexShaderSource, fragmentShaderSourceBrown);
    unsigned int greenShader = createShader(vertexShaderSource, fragmentShaderSourceGreen);
    unsigned int yellowShader = createShader(vertexShaderSource, fragmentShaderSourceYellow);
    unsigned int whiteShader = createShader(vertexShaderSource, fragmentShaderSourceWhite);

    // 투영 행렬 설정
    glm::mat4 projection = glm::ortho(-2.0f, 2.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    unsigned int projectionLoc;

    // 메인 루프
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        // 도로 그리기
        glUseProgram(grayShader);
        projectionLoc = glGetUniformLocation(grayShader, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        drawRoad();

        // 나무와 가로등 간격 설정
        float treePositions[] = { -1.6f, -0.8f, 0.0f, 0.8f };  // 나무 위치
        float lampPositions[] = { -1.2f, -0.4f, 0.4f, 1.2f };  // 가로등 위치

        // 나무 배치
        for (float x : treePositions) {
            // 줄기
            glUseProgram(brownShader);
            projectionLoc = glGetUniformLocation(brownShader, "projection");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
            drawTreeTrunk(x);

            // 잎
            glUseProgram(greenShader);
            projectionLoc = glGetUniformLocation(greenShader, "projection");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
            drawTreeLeaves(x);
        }

        // 가로등 배치
        for (float x : lampPositions) {
            // 기둥
            glUseProgram(grayShader);
            projectionLoc = glGetUniformLocation(grayShader, "projection");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
            drawLampPost(x);

            // 등불
            glUseProgram(yellowShader);
            projectionLoc = glGetUniformLocation(yellowShader, "projection");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
            drawLampLight(x);
        }

        // 구름 배치
        glUseProgram(whiteShader);  // 구름 색상 (흰색)
        projectionLoc = glGetUniformLocation(whiteShader, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        float cloudPositions[][2] = {
            { -1.0f, 0.5f },
            { 0.5f, 0.6f }
        };

        for (const auto& pos : cloudPositions) {
            drawCloud(pos[0], pos[1]);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 자원 해제
    glDeleteProgram(grayShader);
    glDeleteProgram(brownShader);
    glDeleteProgram(greenShader);
    glDeleteProgram(yellowShader);
    glDeleteProgram(whiteShader);

    glfwTerminate();
    return 0;
}