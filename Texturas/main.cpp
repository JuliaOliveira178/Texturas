#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include "../src/stb_image.h"
#include "include\Sprite.h"


const int LARGURA = 800;
const int ALTURA = 600;

const char* vertexShaderSrc = R"(
#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoord;
uniform mat4 model;
uniform mat4 projection;
out vec2 TexCoord;
void main() {
    gl_Position = projection * model * vec4(pos, 0.0, 1.0);
    TexCoord = texCoord;
})";

const char* fragmentShaderSrc = R"(
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D textura;
void main() {
    FragColor = texture(textura, TexCoord);
})";

GLuint criarShader(const char* vsSrc, const char* fsSrc) {
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vsSrc, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fsSrc, NULL);
    glCompileShader(fs);

    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return prog;
}

int main() {
    glfwInit();
    GLFWwindow* janela = glfwCreateWindow(LARGURA, ALTURA, "Cena 1", NULL, NULL);
    glfwMakeContextCurrent(janela);
    glewExperimental = GL_TRUE;
    glewInit();

    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint programa = criarShader(vertexShaderSrc, fragmentShaderSrc);
    glm::mat4 proj = glm::ortho(0.0f, float(LARGURA), 0.0f, float(ALTURA), -1.0f, 1.0f);
    glUseProgram(programa);
    glUniformMatrix4fv(glGetUniformLocation(programa, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

    Elemento fundo("imagens/background.jpg", programa);
    fundo.definirLocal(glm::vec2(400, 300));
    fundo.definirTamanho(glm::vec2(800, 600));

    Elemento heroi("imagens/player.png", programa);
    heroi.definirLocal(glm::vec2(400, 100));
    heroi.definirTamanho(glm::vec2(100, 100));

    Elemento inimigo("imagens/enemy.png", programa);
    inimigo.definirLocal(glm::vec2(500, 150));
    inimigo.definirTamanho(glm::vec2(100, 100));

    Elemento vegetacao("imagens/tree.png", programa);
    vegetacao.definirLocal(glm::vec2(250, 130));
    vegetacao.definirTamanho(glm::vec2(100, 100));

    while (!glfwWindowShouldClose(janela)) {
        glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        fundo.desenhar();
        vegetacao.desenhar();
        inimigo.desenhar();
        heroi.desenhar();

        glfwSwapBuffers(janela);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
