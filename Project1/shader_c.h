#ifndef SHADER_C_H
#define SHADER_C_H

/*原版C++:https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader.h
此版本修改于原版为了适应无法用C++的必要*/

#include <glad\glad.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cglm\cglm.h>
//使用CGLM替代GLM库为了适应无法用C++的必要

unsigned int ID;
void checkCompileErrors(unsigned int shader, char * type);

void Shader(const char* vertexPath, const char* fragmentPath)
{//着色器函数
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
 //utility uniform functions
 //------------------------------------------------------------------------
void setbool(const char * name, bool value) 
{
	glUniform1i(glGetUniformLocation(ID, name), (int)value);
}
 //------------------------------------------------------------------------
void setInt(const char * name, int value) 
{
	glUniform1i(glGetUniformLocation(ID, name), value);
}
// ------------------------------------------------------------------------
void setFloat(const char * name, float value) 
{
	glUniform1f(glGetUniformLocation(ID, name), value);
}
// ------------------------------------------------------------------------
//clgm库没有因此注释掉
//void setVec2(const char * name,vec2 &value) 
//{
//	glUniform2fv(glGetUniformLocation(ID, name), 1, &value[0]);
//}
void setVec2(const char * name, float x, float y)
{
	glUniform2f(glGetUniformLocation(ID, name), x, y);
}
// ------------------------------------------------------------------------
void setVec3(const char * name, vec3 value) 
{
	glUniform3fv(glGetUniformLocation(ID, name), 1, &value[0]);
}
void setVec3_2(const char * name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(ID, name), x, y, z);
}
// ------------------------------------------------------------------------
void setVec4(const char * name, vec4 value) 
{
	glUniform4fv(glGetUniformLocation(ID, name), 1, &value[0]);
}
void setVec4_2(const char * name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ID, name), x, y, z, w);
}
// ------------------------------------------------------------------------
//clgm库没有因此注释掉
//void setMat2(const char * name, mat2 &mat) 
//{
//	glUniformMatrix2fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]);
//}
// ------------------------------------------------------------------------
void setMat3(const char * name, mat3 mat)
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, mat[0]);
}
// ------------------------------------------------------------------------
void setMat4(const char * name, mat4 mat)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, mat[0]);
}
// ------------------------------------------------------------------------
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
		}
	}
}

#endif