#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

class Elemento {
public:
    Elemento(const std::string& caminhoTextura, GLuint programaShader);
    void definirLocal(glm::vec2 p);
    void definirTamanho(glm::vec2 t);
    void definirAngulo(float a);
    void desenhar();

private:
    GLuint textura;
    GLuint VAO;
    GLuint shader;
    glm::vec2 local;
    glm::vec2 tamanho;
    float angulo;

    void carregarTextura(const std::string& caminho);
    void inicializarQuad();
};
