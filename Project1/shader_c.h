#ifndef SHADER_C_H
#define SHADER_C_H

#include <glad\glad.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned int ID;
void checkCompileErrors(unsigned int shader, char * type);

void Shader(const char* vertexPath, const char* fragmentPath)
{
	char * vShaderCode;//读取的字符指针
	char * fShaderCode;//读取的字符指针
	
	FILE * pVertex;
	FILE * pFragment;
	pVertex = fopen(vertexPath, "rb");
	pFragment = fopen(fragmentPath, "rb");
	if (pVertex == NULL && pFragment == NULL)
	{
		printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
		exit(-1);
	}
	long vShaderCodesize, fShaderCodesize;
	fseek(pVertex, 0L, SEEK_END);
	vShaderCodesize = ftell(pVertex);
	rewind(pVertex);
	fseek(pFragment, 0L, SEEK_END);
	fShaderCodesize = ftell(pFragment);
	rewind(pFragment);
	vShaderCode=(char*)malloc(sizeof(char)*vShaderCodesize+1);
	fShaderCode=(char*)malloc(sizeof(char)*fShaderCodesize+1);
	fread(vShaderCode, 1, vShaderCodesize,pVertex);
	fread(fShaderCode, 1, fShaderCodesize, pFragment);
	vShaderCode[vShaderCodesize] = 0;
	fShaderCode[fShaderCodesize] = 0;
	//printf("\n%s\n", vShaderCode);
	//printf("\n%s\n", fShaderCode);
	fclose(pVertex);
	fclose(pFragment);
	// 2. compile shaders
	unsigned int vertex, fragment;
//	int success;
//	char infoLog[512];
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	free(vShaderCode);
	free(fShaderCode);
}

void use()
{
	glUseProgram(ID);
}
// utility uniform functions
// ------------------------------------------------------------------------
void setBool(const char * name, bool value) 
{
	glUniform1i(glGetUniformLocation(ID, name), (int)value);
}
// ------------------------------------------------------------------------
void setInt(const char * name, int value) 
{
	glUniform1i(glGetUniformLocation(ID, name), value);
}
// ------------------------------------------------------------------------
void setFloat(const char * name, float value) 
{
	glUniform1f(glGetUniformLocation(ID, name), value);
}

void checkCompileErrors(unsigned int shader, char * type)
{//检查着色器编译是否有错误
	int success;
	char infoLog[1024];
	if (strcmp(type,"PROGRAM")!=0 )
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			printf("ERROR::SHADER_COMPILATION_ERROR of type:%s\n%s\n -- --------------------------------------------------- -- \n" , type, infoLog );
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n -- --------------------------------------------------- -- \n", type, infoLog);
			//std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

#endif