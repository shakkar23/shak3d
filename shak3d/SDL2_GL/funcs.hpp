X(void, AttachShader, (GLuint, GLuint))
X(void, Clear, (GLbitfield))
X(void, ClearColor, (GLclampf, GLclampf, GLclampf, GLclampf))
X(void, CompileShader, (GLuint))
X(GLuint, CreateProgram, (void))
X(GLuint, CreateShader, (GLenum))
X(void, CullFace, (GLenum))
X(void, DepthMask, (GLboolean))
X(void, DetachShader, (GLuint, GLuint))
X(void, DeleteProgram, (GLuint))
X(void, DeleteShader, (GLuint))
X(void, Disable, (GLenum))
X(void, DrawElements, (GLenum, GLsizei, GLenum, const GLvoid*))
X(void, Enable, (GLenum))
X(void, EnableVertexAttribArray, (GLuint))
X(void, FrontFace, (GLenum))
X(const GLubyte*, GetString, (GLenum))
X(GLenum, GetError, (void))
X(void, GetProgramiv, (GLuint, GLenum, GLint*))
X(void, GetShaderInfoLog, (GLuint, GLsizei, GLsizei*, char*))
X(void, GetShaderiv, (GLuint, GLenum, GLint*))
X(GLint, GetUniformLocation, (GLuint, const char*))
X(void, LinkProgram, (GLuint))
X(void, ShaderSource, (GLuint, GLsizei, const GLchar* const*, const GLint*))
X(void, Uniform3fv, (GLint, GLsizei, const GLfloat*))
X(void, UniformMatrix4fv, (GLint, GLsizei, GLboolean, const GLfloat*))
X(void, UseProgram, (GLuint))
X(void, ValidateProgram, (GLuint))
X(void, VertexAttribPointer, (GLuint, GLint, GLenum, GLboolean, GLsizei, const void*))
X(void, Viewport, (GLint, GLint, GLsizei, GLsizei))
X(GLint, GetAttribLocation, (GLuint, const GLchar*))
X(void, GenBuffers, (GLsizei, GLuint*))
X(void, DeleteBuffers, (GLsizei, GLuint*))
X(void, BindBuffer, (GLenum, GLuint))
X(void, BufferData, (GLenum, GLsizeiptr, const GLvoid*, GLenum))
X(void, GetProgramInfoLog, (GLuint, GLsizei, GLsizei*, char*))
// glBegin
X(void, Begin, (GLenum))
// glEnd
X(void, End, ())
// glVertex2f
X(void, Vertex2f, (GLfloat, GLfloat))
// glColor3f
X(void, Color3f, (GLfloat, GLfloat, GLfloat))
// glOrtho
X(void, Ortho, (GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble))
// glLoadIdentity
X(void, LoadIdentity, ())
// glTranslatef
X(void, Translatef, (GLfloat, GLfloat, GLfloat))
// glRotatef
X(void, Rotatef, (GLfloat, GLfloat, GLfloat, GLfloat))
// TexCoord2f
X(void, TexCoord2f, (GLfloat, GLfloat))
// GetIntegerv
X(void, GetIntegerv, (GLenum, GLint*))
// Uniform4f
X(void, Uniform4f, (GLint, GLfloat, GLfloat, GLfloat, GLfloat))
// GenVertexArrays
X(void, GenVertexArrays, (GLsizei, GLuint*))
// BindVertexArray
X(void, BindVertexArray, (GLuint))
// DrawArrays
X(void, DrawArrays, (GLenum, GLint, GLsizei))
// GenFramebuffers
X(void, GenFramebuffers, (GLsizei, GLuint*))
// BindFramebuffer
X(void, BindFramebuffer, (GLenum, GLuint))
// FramebufferTexture2D
X(void, FramebufferTexture2D, (GLenum, GLenum, GLenum, GLuint, GLint))
// CheckFramebufferStatus
X(GLenum, CheckFramebufferStatus, (GLenum))
// DeleteVertexArrays
X(void, DeleteVertexArrays, (GLsizei, const GLuint*))
// Uniform1i
X(void, Uniform1i, (GLint, GLint))
// Uniform1f
X(void, Uniform1f, (GLint, GLfloat))
// Uniform2f
X(void, Uniform2f, (GLint, GLfloat, GLfloat))
// Uniform3f
X(void, Uniform3f, (GLint, GLfloat, GLfloat, GLfloat))
// Uniform1fv
X(void, Uniform1fv, (GLint, GLsizei, const GLfloat*))
// Uniform2fv
X(void, Uniform2fv, (GLint, GLsizei, const GLfloat*))
// Uniform4fv
X(void, Uniform4fv, (GLint, GLsizei, const GLfloat*))
// UniformMatrix3fv
X(void, UniformMatrix3fv, (GLint, GLsizei, GLboolean, const GLfloat*))
// GenTextures
X(void, GenTextures, (GLsizei, GLuint*))
// BindTexture
X(void, BindTexture, (GLenum, GLuint))
// TexImage2D
X(void, TexImage2D, (GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid*))

//PolygonMode
X(void, PolygonMode, (GLenum, GLenum))
//ClearBufferfv
X(void, ClearBufferfv, (GLenum, GLint, const GLfloat*))