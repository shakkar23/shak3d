#pragma once
#include "context.hpp"

#include <SDL2/SDL.h>

void init();
// shader type = GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
GLuint compileShader(const char* source, GLuint shaderType);
GLuint compileProgram(const char* vtxFile, const char* fragFile);

#ifndef SHADER_COMPILER_IMPL
#include <iostream>
#include <fstream>
#include <string>

GLFuncTable glFuncTable;

#define GL_CHECK(call) do { glFuncTable.call; assert(glFuncTable.GetError() == GL_NO_ERROR); } while (0)
#define GL_NO_CHECK(call) (glFuncTable.call)
//---------------------------------------------------------------------------
//  Fill in the glFuncTable function table on Win32
//  This lets us avoid having to link with opengl32.lib
//---------------------------------------------------------------------------
void GLFuncTable::initialize() {
#define X(retVal, name, args) { name = (retVal (GL_APIENTRY *)args) SDL_GL_GetProcAddress("gl" #name); }
#include "funcs.hpp"
#undef X
}

void init() {
	glFuncTable.initialize();
}

// shader type = GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
GLuint compileShader(const char* source, GLuint shaderType) {
	// prints the source and the shader its compiling
	// std::cout << "Compilando shader:" << std::endl << source << std::endl
	
	// Create ID for shader
	GLuint result = GL_NO_CHECK(CreateShader(shaderType));
	// Define shader text
	GL_NO_CHECK(ShaderSource(result, 1, &source, NULL));
	// Compile shader
	GL_NO_CHECK(CompileShader(result));

	//Check vertex shader for errors
	GLint shaderCompiled = GL_FALSE;
	GL_NO_CHECK(GetShaderiv(result, GL_COMPILE_STATUS, &shaderCompiled));
	if (shaderCompiled != GL_TRUE) {
		std::cout << "Error en la compilacion: " << result << "!" << std::endl;
		GLint logLength;
		GL_NO_CHECK(GetShaderiv(result, GL_INFO_LOG_LENGTH, &logLength));
		if (logLength > 0)
		{
			GLchar* log = (GLchar*)malloc(logLength);
			GL_NO_CHECK(GetShaderInfoLog(result, logLength, &logLength, log));
			std::cout << "Shader compile log:" << log << std::endl;
			free(log);
		}
		GL_NO_CHECK(DeleteShader(result));
		result = 0;
	}
	else {
		// outputs the ID and notifies that the shader was compiled correctly
		//std::cout << "Shader compilado correctamente. Id = " << result << std::endl;
	}
	return result;
}

GLuint compileProgram(const char* vtxFile, const char* fragFile) {
	GLuint programId = 0;
	GLuint vtxShaderId, fragShaderId;

	programId = GL_NO_CHECK(CreateProgram());

	std::ifstream f(vtxFile);
	std::string source((std::istreambuf_iterator<char>(f)),
		std::istreambuf_iterator<char>());
	vtxShaderId = compileShader(source.c_str(), GL_VERTEX_SHADER);

	f = std::ifstream(fragFile);
	source = std::string((std::istreambuf_iterator<char>(f)),
		std::istreambuf_iterator<char>());
	fragShaderId = compileShader(source.c_str(), GL_FRAGMENT_SHADER);

	if (vtxShaderId && fragShaderId) {
		// Associate shader with program
		GL_NO_CHECK(AttachShader(programId, vtxShaderId));
		GL_NO_CHECK(AttachShader(programId, fragShaderId));
		GL_NO_CHECK(LinkProgram(programId));
		GL_NO_CHECK(ValidateProgram(programId));

		// Check the status of the compile/link
		GLint logLen;
		GL_NO_CHECK(GetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLen));
		if (logLen > 0) {
			char* log = (char*)malloc(logLen * sizeof(char));
			// Show any errors as appropriate
			GL_NO_CHECK(GetProgramInfoLog(programId, logLen, &logLen, log));
			std::cout << "Prog Info Log: " << std::endl << log << std::endl;
			free(log);
		}

		// detach and delete shaders
		GL_NO_CHECK(DetachShader(programId, vtxShaderId));
		GL_NO_CHECK(DetachShader(programId, fragShaderId));
	}
	if (vtxShaderId) {
		GL_NO_CHECK(DeleteShader(vtxShaderId));
	}
	if (fragShaderId) {
		GL_NO_CHECK(DeleteShader(fragShaderId));
	}
	return programId;
}



#endif // SHADER_COMPILER_IMPL