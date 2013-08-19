// Loads shaders
// allow usable in application
// After compile, link shader, its loaded onto gpu, so maybe can delete the source file buffer ?


/*
	To install and use OpenGL shaders, do the following:
		After these steps, subsequent graphics primitives will be drawn with the shaders you've
		provided rather than with OpenGL's defined fixed functionality pipeline.
		1. Create one or more (empty) shader objects by calling glCreateShader.
		2. Provide source code for these shaders by calling glShaderSource.
		3. Compile each of the shaders by calling glCompileShader.
		4. Create a program object by calling glCreateProgram.
		5. Attach all the shader objects to the program object by calling glAttachShader.
		6. Link the program object by calling glLinkProgram.
		7. Install the executable program as part of OpenGL's current state by calling glUseProgram.
		8. If the shaders use user-defined uniform variables, query the locations of these variables
			with glGetUniformLocation and then set their values with glUniform.
	After these steps, subsequent graphics primitives will be drawn with the shaders you've
	provided rather than with OpenGL's defined fixed functionality pipeline.


 */
/*
class CShaderNode
{
private:
	char *vertexShaderSource;
	char *fragmentShaderSource;
	GLuint vertexShaderHandle, fragmentShaderHandle, shaderProgramHandle;	// handles to objects
public:
	CShaderNode(const char *sourceFileVS, const char *sourceFileFS); 		// VS; vertex shader, FS; fragment shader
	~CShaderNode();
	bool loadShader(const char *sourceFileVS, const char *sourceFileFS);
	void useShader(void);
};

CShaderNode::CShaderNode(const char* sourceFileVS, const char *sourceFileFS)
{
	vertexShaderSource = NULL;

	if(loadShader(sourceFileVS, sourceFileFS))
		cout << INS << "Shader: " << sourceFileVS << "\n\tloaded successfully.";
	else
		cout << INS << "Shader: " << sourceFileFS << "\n\tfailed to load.";
}

CShaderNode::~CShaderNode()
{
	// Delete char buffer containing source file for shader

	// delete other shader data
	// Delete shaders;
	glDeleteShader(vertexShaderHandle);
	glDeleteShader(fragmentShaderHandle);

	// Delete program;
	glDeleteShader(fragmentShaderHandle);
}

void CShaderNode::useShader(void)
{
	// Make the shader apart of the current state;
	if(glIsProgram(shaderProgramHandle))
		glUseProgram(shaderProgramHandle);
}

bool CShaderNode::loadShader(const char *sourceFileVS, const char *sourceFileFS)
{
	// load shader from text file, compile, have avail for application use
	//ifstream fin.
	// failed to load VS, FS return false;

	char *vertexShaderSource, *fragmentShaderSource;
	GLint compileSuccessVS, compileSuccessFS, linkSucces;
	//fopen


	// Create two empty shader objects;
	vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);

	// Copy string containing source to shader; any previously specified src is completely replaced
	glShaderSource(vertexShaderHandle, 1, &vertexShaderSource, NULL);	// Length = NULL; string is null terminated
	glShaderSource(fragmentShaderHandle, 1, &fragmentShaderSource, NULL);

	// Compile shaders;
	glCompileShader(vertexShaderHandle);
	glCompileShader(fragmentShaderHandle);

	// Get compile status;
	glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &compileSuccessVS);
	glGetShaderiv(fragmentShaderHandle, GL_COMPILE_STATUS, &compileSuccessFS);

	// Check compile status;
	if(compileSuccessVS == GL_FALSE || compileSuccessFS == GL_FALSE)
	{
		cout << INS << "Failed to compile shader.";
		return false;
	}

	// Generate a shader program object;
	shaderProgramHandle = glCreateProgram();

	// Attach shader objects to shader program object;
	glAttachShader(shaderProgramHandle, vertexShaderHandle);
	glAttachShader(shaderProgramHandle, fragmentShaderHandle);

	// Link the program object;
	glLinkProgram(shaderProgramHandle);

	// Get link status;
	glGetProgramiv(shaderProgramHandle, GL_LINK_STATUS, &linkSuccess);

	// Check if program linked successfully;
	if(linkSuccess == GL_FALSE)
	{
		cout << INS << "Failed to link shader.";
		return false;
	}

	cout << INS << "Shader: " << sourceFileVS << ", loaded successfully.";
	cout << INS << "Shader: " << sourceFileFS << ", loaded successfully.";
	return true;
}

class CShader
{
private:
	CShaderNode *shaderNodeHead;
public:
	CShader(const char* shaderSrcVS, const char* shaderSrcFS);
	~CShader();
	bool loadVertexShader(const char *shaderSourceFile);
	bool loadFragmentShader(const char *shaderSourceFile);
};

CShader::CShader(const char* shaderSrcVS = NULL, const char* shaderSrcFS = NULL)
{
	shaderNodeHead = NULL;
}

CShader::~CShader(void)
{
	// Delete shader data
}

bool CShader::loadVertexShader(const char *shaderSourceFile)
{
	return true;
	return false;
}

bool CShader::loadFragmentShader(const char *shaderSourceFile)
{
	return true;
	return false;
}


*/
