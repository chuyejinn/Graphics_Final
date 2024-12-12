#ifndef SHADER_H
#define SHADER_H

unsigned int compileShader(unsigned int type, const char* source);
unsigned int createShader(const char* vertexShader, const char* fragmentShader);

#endif