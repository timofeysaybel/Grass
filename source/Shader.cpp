#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>

#include "../include/Shader.hpp"

using namespace std;

void checkCompileErrors(unsigned int shader, std::string type)
{
    GLint result = GL_FALSE;
    char *infoLog = new char[1024];

    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if (!result)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "Ошибка загрузки шейдера: " << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &result);
        if (!result)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "Ошибка загрузки шейдера: " << infoLog << std::endl;
        }
    }
}

GLuint loadShaders(const char *vertex_file_path, const char *fragment_file_path, const char *geometry_file_path)
{

    unsigned int vertex, fragment, geometry;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    if (geometry_file_path != nullptr)
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        
    std::string vertex_shader_code;
    std::ifstream vertex_shader_stream(vertex_file_path, std::ios::in);
    if (vertex_shader_stream.is_open())
    {
        std::stringstream sstr;
        sstr << vertex_shader_stream.rdbuf();
        vertex_shader_code = sstr.str();
        vertex_shader_stream.close();
    }
    else
    {
        std::cout << "Не удалось открыть " << vertex_file_path << ". Возможно вы не в той директории" << std::endl;
        getchar();
        return 0;
    }

    std::string fragment_shader_code;
    std::ifstream fragment_shader_stream(fragment_file_path, std::ios::in);
    if (fragment_shader_stream.is_open())
    {
        std::stringstream sstr;
        sstr << fragment_shader_stream.rdbuf();
        fragment_shader_code = sstr.str();
        fragment_shader_stream.close();
    }

    std::string geometry_shader_code;
    std::ifstream geometry_shader_stream(geometry_file_path, std::ios::in);
    if (geometry_file_path != nullptr && geometry_shader_stream.is_open())
    {
        std::stringstream sstr;
        sstr << geometry_shader_stream.rdbuf();
        geometry_shader_code = sstr.str();
        geometry_shader_stream.close();
    }

    char const *vertex_source_pointer = vertex_shader_code.c_str();
    glShaderSource(vertex, 1, &vertex_source_pointer, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    char const *fragment_source_pointer = fragment_shader_code.c_str();
    glShaderSource(fragment, 1, &fragment_source_pointer, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    char const *geometry_source_pointer;
    if (geometry_file_path != nullptr)
    {
        geometry_source_pointer = geometry_shader_code.c_str();
        glShaderSource(geometry, 1, &geometry_source_pointer, NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
    }
    GLuint program_ID = glCreateProgram();
    glAttachShader(program_ID, vertex);
    if (geometry_file_path != nullptr)
        glAttachShader(program_ID, geometry);
    glAttachShader(program_ID, fragment);
    glLinkProgram(program_ID);
    checkCompileErrors(program_ID, "PROGRAM");
    glDeleteShader(vertex);
    if (geometry_file_path != nullptr)
        glDeleteShader(geometry);
    glDeleteShader(fragment);
    return program_ID;
}
