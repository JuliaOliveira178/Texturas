#include "Sprite.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "../src/stb_image.h"
#include <iostream>

Elemento::Elemento(const std::string& caminhoTextura, GLuint programaShader)
    : local(0.0f), tamanho(1.0f), angulo(0.0f), shader(programaShader) {
    carregarTextura(caminhoTextura);
    inicializarQuad();
}

void Elemento::definirLocal(glm::vec2 p) { local = p; }
void Elemento::definirTamanho(glm::vec2 t) { tamanho = t; }
void Elemento::definirAngulo(float a) { angulo = a; }

void Elemento::desenhar() {
    glBindTexture(GL_TEXTURE_2D, textura);
    glBindVertexArray(VAO);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(local, 0.0f));
    model = glm::rotate(model, glm::radians(angulo), glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(tamanho, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Elemento::carregarTextura(const std::string& caminho) {
    glGenTextures(1, &textura);
    glBindTexture(GL_TEXTURE_2D, textura);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int largura, altura, canais;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* dados = stbi_load(caminho.c_str(), &largura, &altura, &canais, STBI_rgb_alpha);
    if (dados) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, largura, altura, 0, GL_RGBA, GL_UNSIGNED_BYTE, dados);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Erro ao carregar textura: " << caminho << std::endl;
    }
    stbi_image_free(dados);
}

void Elemento::inicializarQuad() {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.0f
    };

    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
