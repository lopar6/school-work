#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "cylinder.h"
#include "camera.h"
#include "Sphere.h"

using namespace std; // Standard namespace

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// settings
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// camera
Camera gCamera(glm::vec3(0.0f, 1.5f, 20.0f));
float gLastX = WINDOW_WIDTH / 2.0f;
float gLastY = WINDOW_HEIGHT / 2.0f;
bool gFirstMouse = true;

glm::vec2 gUVScale(1.0f, 1.0f);
glm::vec2 grassUVScale(5.0f, 5.0f);
GLint gTexWrapMode = GL_REPEAT;

// Shader programs
GLuint defaultProgramId;
GLuint groundProgramId;

// light color
glm::vec3 gObjectColor(1.0f, 1.0f, 1.0f);
glm::vec3 glightColor(0.9f, 0.9f, 0.9f);

// Light position and scale
glm::vec3 glightPosition(-10.0f, 50.0f, 10.0f);
glm::vec3 glightScale(1.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float lastFlip;

bool shouldUseProjection = true;

const GLchar* defaultVertexShader = GLSL(440,

	layout(location = 0) in vec3 position; // VAP position 0 for vertex position data
	layout(location = 1) in vec2 textureCoordinate;
	layout(location = 2) in vec3 normal; 

	out vec3 vertexNormal; // For outgoing normals to fragment shader
	out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader
	out vec2 vertexTextureCoordinate;

	//Uniform / Global variables for the  transform matrices
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates

		vertexFragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

		vertexNormal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties
		vertexTextureCoordinate = textureCoordinate;
	}
);


const GLchar* defaultFragmentShader = GLSL(440,

	in vec3 vertexNormal; // For incoming normals
	in vec3 vertexFragmentPos; // For incoming fragment position
	in vec2 vertexTextureCoordinate;

	out vec4 fragmentColor; // For outgoing  color to the GPU

	// Uniform / Global variables for object color, light color, light position, and camera/view position
	uniform vec3 objectColor;
	uniform vec3 lightColor;
	uniform vec3 lightPos;
	uniform vec3 lightScale;
	uniform vec3 viewPosition;
	uniform vec2 uvScale;
	//uniform sampler2D sceneTexture;
	uniform sampler2D texture0;
	uniform sampler2D texture1;
	uniform sampler2D texture2;
	uniform sampler2D texture3;
	uniform int textureIndex;

	void main()
	{
		/*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

		//Calculate Ambient lighting*/
		float ambientStrength = 0.4f; // Set ambient or global lighting strength
		vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0); // Generate ambient light color

		//Calculate Diffuse lighting*/
		vec3 norm = normalize(vertexNormal);
		vec3 lightDirection = normalize(lightPos - vertexFragmentPos);
		float fillImpact = max(dot(norm, lightDirection), 0.0);
		vec3 diffuse = (lightColor * fillImpact * lightScale);
		//vec4 textureColor = texture(sceneTexture, vertexTextureCoordinate * uvScale);

		// Texture holds the color to be used for all three components
		vec4 textureColor;
		switch (textureIndex) {
		case 0:
			textureColor = texture(texture0, vertexTextureCoordinate * uvScale);
			break;
		case 1:
			textureColor = texture(texture1, vertexTextureCoordinate * uvScale);
			break;
		case 2:
			textureColor = texture(texture2, vertexTextureCoordinate * uvScale);
			break;		
		case 3:
			textureColor = texture(texture3, vertexTextureCoordinate * uvScale);
			break;
		}

		//Calculate Specular lighting*/
		float specularIntensity = 0.0f; // Set specular light strength
		float highlightSize = 16.0f; // Set specular highlight size
		vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
		vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
		//Calculate specular component
		float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
		vec3 specular = specularIntensity * specularComponent * lightColor;

		// Calculate phong result
		vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;

		fragmentColor = vec4(phong, 1.0); // Send lighting results to GPU
	}
);

const GLchar* groundFragmentShader = GLSL(440,

	in vec3 vertexNormal;
	in vec3 vertexFragmentPos;
	in vec2 vertexTextureCoordinate;

	out vec4 fragmentColor;

	uniform vec3 objectColor;
	uniform vec3 lightColor;
	uniform vec3 lightPos;
	uniform vec3 lightScale;
	uniform vec3 viewPosition;
	uniform vec2 uvScale;
	uniform sampler2D groundTexture;
	uniform sampler2D pathTexture;
	uniform int textureIndex;

	void main()
	{

		float ambientStrength = 0.1f; 
		vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);

		vec3 norm = normalize(vertexNormal);
		vec3 lightDirection = normalize(lightPos - vertexFragmentPos);
		float fillImpact = max(dot(norm, lightDirection), 0.0);
		vec3 diffuse = (lightColor * fillImpact * lightScale);
		vec4 textureColor;
		if (vertexFragmentPos[0] < 1.5 && vertexFragmentPos[0] > -1.0) {
			textureColor = texture(pathTexture, vertexTextureCoordinate * uvScale);
		}
		else {
			textureColor = texture(groundTexture, vertexTextureCoordinate * uvScale);
		}
		
		vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
		vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
		float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), 2.0f);
		vec3 specular = 1.0f * specularComponent * lightColor;

		vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;

		fragmentColor = vec4(phong, 1.0); // Send lighting results to GPU
	}
);

bool createShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{
	// Compilation and linkage error reporting
	int success = 0;
	char infoLog[512];

	// Create a Shader program object.
	programId = glCreateProgram();

	// Create the vertex and fragment shader objects
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	// Retrive the shader source
	glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
	glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

	// Compile the vertex shader, and print compilation errors (if any)
	glCompileShader(vertexShaderId); // compile the vertex shader
	// check for shader compile errors
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

		return false;
	}

	glCompileShader(fragmentShaderId); // compile the fragment shader
	// check for shader compile errors
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

		return false;
	}

	// Attached compiled shaders to the shader program
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	glLinkProgram(programId);   // links the shader program
	// check for linking errors
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

		return false;
	}

	glUseProgram(programId);    // Uses the shader program

	return true;
}

void destroyShaderProgram(GLuint programId)
{
	glDeleteProgram(programId);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (gFirstMouse)
	{
		gLastX = xpos;
		gLastY = ypos;
		gFirstMouse = false;
	}

	float xoffset = xpos - gLastX;
	float yoffset = gLastY - ypos; // reversed since y-coordinates go from bottom to top

	gLastX = xpos;
	gLastY = ypos;

	gCamera.ProcessMouseMovement(xoffset, yoffset);

}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	gCamera.ProcessMouseScroll(yoffset);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		gCamera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		gCamera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		gCamera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		gCamera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		gCamera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		gCamera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		if (lastFlip > 0.5f) {
			shouldUseProjection = !shouldUseProjection;
			lastFlip = 0.0f;
		}
	}
}

bool initAndConfigure(GLFWwindow* window) {

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shader program
	// ------------------------------------
	if (!createShaderProgram(defaultVertexShader, defaultFragmentShader, defaultProgramId))
		return EXIT_FAILURE;
	if (!createShaderProgram(defaultVertexShader, groundFragmentShader, groundProgramId))
		return EXIT_FAILURE;
	
	return true;
}

// Create Vertex Attribute Pointers
// --------------------------------
void createVertexAttrPointers() {
	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(1, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(1);

}

// Create Textures
// --------------------------------
void createTexture(unsigned int& textureId, const char* filename) {
	int width, height, nrChannels;

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

glm::mat4 getProjection(){
	if (shouldUseProjection) {
		return glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
	}
	else {
		return glm::ortho(
			-0.2f * gCamera.Zoom,
			0.2f * gCamera.Zoom,
			-0.2f * gCamera.Zoom,
			0.2f * gCamera.Zoom,
			0.2f,
			100.0f
		);
	}
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Mountain Trail", NULL, NULL);
	if (!initAndConfigure(window)) {
		return -1;
	}

	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;
	const GLuint floatsPerLine = floatsPerVertex + floatsPerNormal + floatsPerUV;
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);
	unsigned int cubeVBO, cubeVAO, planeVBO, planeVAO, pyramidVBO, pyramidVAO;


	// body shapes -------------------------------
	glm::mat4 model;
	Sphere head(1, 30, 30);
	static_meshes_3D::Cylinder cylinder(.5, 20, 2, true, true, true);
	static_meshes_3D::Cylinder trunk(.1, 20, 8, true, true, true);


	// in process of modularizing!!!


	// plane -----------------------------
	float planeVerts[] = {
		//Positions            //normals           //Texture Coordinates
		-1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
		 1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		 1.0f,  1.0f,  1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		-1.0f,  1.0f,  1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
	};

	// cube -----------------------------
	float cubeVerts[] = {
		//Positions         //normals            //Texture Coordinates
	   -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

	   -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
	   -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,

	   -0.5f,  0.5f,  0.5f,	 -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,	 -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,	 -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,	 -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,	 -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,	 -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

	   -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
	   -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
	   -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,

	   -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f
	};

	GLfloat pyramidVerts[] = {
		//Positions         //normals            //Texture Coordinates
		 0.0f,  1.0f,  0.0f,   0.0f,  1.0f, 1.0f,   1.0f, 1.0f,
		-1.0f,  0.0f,  1.0f,   0.0f,  1.0f, 1.0f,   1.0f, 0.0f,
		 1.0f,  0.0f,  1.0f,   0.0f,  1.0f, 1.0f,   0.0f, 0.0f,

		 0.0f,  1.0f,  0.0f,   1.0f,  1.0f,  0.0f,   1.0f, 1.0f,
		 1.0f,  0.0f, -1.0f,   1.0f,  1.0f,  0.0f,   1.0f, 0.0f,
		 1.0f,  0.0f,  1.0f,   1.0f,  1.0f,  0.0f,   0.0f, 0.0f,

		 0.0f,  1.0f,  0.0f,   0.0f,  1.0f,  1.0f,   1.0f, 1.0f,
		 1.0f,  0.0f, -1.0f,   0.0f,  1.0f,  1.0f,   1.0f, 0.0f,
		-1.0f,  0.0f, -1.0f,   0.0f,  1.0f,  1.0f,   0.0f, 0.0f,

		 0.0f,  1.0f,  0.0f,  -1.0f,  1.0f,  0.0f,   1.0f, 1.0f,
		-1.0f,  0.0f, -1.0f,  -1.0f,  1.0f,  0.0f,   1.0f, 0.0f,
		-1.0f,  0.0f,  1.0f,  -1.0f,  1.0f,  0.0f,   0.0f, 0.0f,

		// bottom
		-1.0f,  0.0f,  1.0f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
		 1.0f,  0.0f,  1.0f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
		 1.0f,  0.0f, -1.0f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
		 1.0f,  0.0f, -1.0f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
		-1.0f,  0.0f, -1.0f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
		-1.0f,  0.0f,  1.0f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
	};

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVerts), planeVerts, GL_STATIC_DRAW);
	createVertexAttrPointers();

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
	createVertexAttrPointers();

	glGenVertexArrays(1, &pyramidVAO);
	glGenBuffers(1, &pyramidVBO);
	glBindVertexArray(pyramidVAO);
	glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVerts), pyramidVerts, GL_STATIC_DRAW);
	createVertexAttrPointers();

	// --------------------------------------------------------------------------
	// textures
	// --------------------------------------------------------------------------
	unsigned int grassTextureId, bodyTextureId, headTextureId, rockTextureId, dirtTextureId, pineTextureId;

	createTexture(grassTextureId, "images/grass2.jpg");
	createTexture(bodyTextureId, "images/human.jpg");
	createTexture(headTextureId, "images/hair.jpg");
	createTexture(rockTextureId, "images/rock.jpg");
	createTexture(dirtTextureId, "images/dirt.jpg");
	createTexture(pineTextureId, "images/pine.jpg");

	glUseProgram(defaultProgramId);
	glUniform1i(glGetUniformLocation(grassTextureId, "texture0"), 0);
	glUniform1i(glGetUniformLocation(bodyTextureId, "texture1"), 1);
	glUniform1i(glGetUniformLocation(headTextureId, "texture2"), 2);
	glUniform1i(glGetUniformLocation(rockTextureId, "texture3"), 3);
	glUniform1i(glGetUniformLocation(pineTextureId, "texture4"), 4);

	glUseProgram(groundProgramId);
	glUniform1i(glGetUniformLocation(bodyTextureId, "groundTexture"), 0);
	glUniform1i(glGetUniformLocation(rockTextureId, "pathTexture"), 1);


	// randomly rotate on each axis for each tree branch (not actually random)
	float rotateX = rand() % 10 - 5;
	float rotateY = rand() % 10 - 5;
	float rotateZ = rand() % 10 - 5;

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		lastFlip += deltaTime; // projection flip debounce

		// input
		processInput(window);

		// render
		glClearColor(0.4f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = getProjection();
		glm::mat4 view = gCamera.GetViewMatrix();
		const glm::vec3 cameraPosition = gCamera.Position;

		// ground
		// -------------------------------------------------------------------------------
		glUseProgram(groundProgramId);

		GLint groundModelLoc = glGetUniformLocation(groundProgramId, "model");
		GLint groundViewLoc = glGetUniformLocation(groundProgramId, "view");
		GLint groundProjLoc = glGetUniformLocation(groundProgramId, "projection");
		GLint groundObjectColorLoc = glGetUniformLocation(groundProgramId, "objectColor");
		GLint groundlightColorLoc = glGetUniformLocation(groundProgramId, "lightColor");
		GLint groundlightPositionLoc = glGetUniformLocation(groundProgramId, "lightPos");
		GLint groundlightScaleLoc = glGetUniformLocation(groundProgramId, "lightScale");
		GLint groundViewPositionLoc = glGetUniformLocation(groundProgramId, "viewPosition");

		glUniform3f(groundObjectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
		glUniform3f(groundlightColorLoc, glightColor.r, glightColor.g, glightColor.b);
		glUniform3f(groundlightPositionLoc, glightPosition.x, glightPosition.y, glightPosition.z);
		glUniform3f(groundlightScaleLoc, glightScale.r, glightScale.b, glightScale.g);
		glUniform3f(groundViewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);
		glUniformMatrix4fv(groundViewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(groundProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

		GLint groundUVScaleLoc = glGetUniformLocation(groundProgramId, "uvScale");

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, grassTextureId);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, dirtTextureId);
		glBindVertexArray(planeVAO);
		glUniform2fv(groundUVScaleLoc, 1, glm::value_ptr(grassUVScale));
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -5.0f));
		model = glm::scale(model, glm::vec3(20.0f, 0.000001f, 20.0f));
		glUniformMatrix4fv(groundModelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// person
		// -------------------------------------------------------------------------------
		// activate shader
		glUseProgram(defaultProgramId);

		GLint textureIndexLoc = glGetUniformLocation(defaultProgramId, "textureIndex");
		GLint modelLoc = glGetUniformLocation(defaultProgramId, "model");
		GLint viewLoc = glGetUniformLocation(defaultProgramId, "view");
		GLint projLoc = glGetUniformLocation(defaultProgramId, "projection");
		GLint objectColorLoc = glGetUniformLocation(defaultProgramId, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(defaultProgramId, "lightColor");
		GLint lightPositionLoc = glGetUniformLocation(defaultProgramId, "lightPos");
		GLint lightScaleLoc = glGetUniformLocation(defaultProgramId, "lightScale");
		GLint viewPositionLoc = glGetUniformLocation(defaultProgramId, "viewPosition");

		glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
		glUniform3f(lightColorLoc, glightColor.r, glightColor.g, glightColor.b);
		glUniform3f(lightPositionLoc, glightPosition.x, glightPosition.y, glightPosition.z);
		glUniform3f(lightScaleLoc, glightScale.r, glightScale.b, glightScale.g);

		glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		GLint UVScaleLoc = glGetUniformLocation(defaultProgramId, "uvScale");

		// body
		// -------------------------------------------------------------------------------
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(textureIndexLoc, 1);
		glBindTexture(GL_TEXTURE_2D, bodyTextureId);
		glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.2f, -5.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.8f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(defaultProgramId, "model"), 1, GL_FALSE, glm::value_ptr(model));
		cylinder.render();

		// head
		// -------------------------------------------------------------------------------
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, headTextureId);
		model = glm::mat4(1.0f); 
		model = glm::translate(model, glm::vec3(0.0f, 0.85f, -5.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(textureIndexLoc, 2);
		head.Draw();

		// rightBoulder
		// -------------------------------------------------------------------------------
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rockTextureId);
		glUniform1i(textureIndexLoc, 2);
		glBindVertexArray(cubeVAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.5f, 0.0f, 12.0f));
		model = glm::rotate(model, 0.1f, glm::vec3(1.0, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(6.0f, 2.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// leftBoulder
		// -------------------------------------------------------------------------------
		// top pyramid
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rockTextureId);
		glUniform1i(textureIndexLoc, 3);
		glBindVertexArray(pyramidVAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.5f, 0.1f, 12.0f));
		model = glm::rotate(model, 0.1f, glm::vec3(-1.0, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 24);

		// bottom cube
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rockTextureId);
		glUniform1i(textureIndexLoc, 3);
		glBindVertexArray(cubeVAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.4f, -1.0f, 10.0f));
		model = glm::rotate(model, 0.1f, glm::vec3(-1.0, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(4.0f, 2.0f, 8.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// trees
		// -------------------------------------------------------------------------------
		// close left trees
		float leftOffset = 1.0f;
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 10; ++j) {			
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(-14.0f, 3.0f, 12.0f));
				glBindTexture(GL_TEXTURE_2D, rockTextureId);
				model = glm::translate(model, glm::vec3(j * 0.8f * leftOffset, 0.0f,(i * 3.0f - (j * 2.5f))));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				trunk.render();

				model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
				model = glm::scale(model, glm::vec3(1.0f, 8.0f, 1.0f));
				glBindTexture(GL_TEXTURE_2D, pineTextureId);
				glBindVertexArray(pyramidVAO);
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 24);

			}
			leftOffset += 0.2f;
		}

		// trees in the back
		// too many triangles, or this would be great for other trees
		float offset = 1.0f;
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 15; ++j) {			
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(-2.0f, 3.0f, -22.0f));
				glBindTexture(GL_TEXTURE_2D, rockTextureId);
				model = glm::translate(model, glm::vec3(j * 1.0f * offset , 0.0f,(i * 3.0f + (j / 2.0f))));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				trunk.render();

				model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
				model = glm::scale(model, glm::vec3(1.0f, 8.0f, 1.0f));
				glBindTexture(GL_TEXTURE_2D, pineTextureId);
				glBindVertexArray(pyramidVAO);
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 24);
			}
			offset += 0.2f;
		}


		// close right
		for (int i = 0; i < 2; ++i) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(2.0f + (i * 2), 3.5f, 9.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));

			glBindTexture(GL_TEXTURE_2D, rockTextureId);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			trunk.render();

			model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));

			for (int j = 0; j < 17; j++) {
				glBindTexture(GL_TEXTURE_2D, pineTextureId);
				glBindVertexArray(planeVAO);
				model = glm::translate(model, glm::vec3(0.0f, (j * 0.05f), 0.0f));
				model = glm::scale(model, glm::vec3(1.0f - (0.01f * j), 1.0f, 1.0f - (0.01f * j)));

				if (j % 2 == 0) {
					model = glm::rotate(model, 0.3f, glm::vec3(rotateX, rotateY, rotateZ));
				}
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 6);

				// reset the rotation
				if (j % 2 == 0) {
					model = glm::rotate(model, 0.3f, glm::vec3(-rotateX, -rotateY, -rotateZ));
				}
			}
		}

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.8f, 6.5f, 14.0f));
		model = glm::scale(model, glm::vec3(4.0f, 2.0f, 4.0f));
		glBindTexture(GL_TEXTURE_2D, rockTextureId);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		trunk.render();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.5f));
		for (int i = 0; i < 8; i++) {
			glBindTexture(GL_TEXTURE_2D, pineTextureId);
			glBindVertexArray(planeVAO);
			model = glm::translate(model, glm::vec3(0.0f, (i * 0.08f), 0.0f));
			if (i % 2 == 0) {
				model = glm::rotate(model, 0.3f, glm::vec3(rotateX, rotateY, rotateZ));
			}
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 6);

			// reset the rotation
			if (i % 2 == 0) {
				model = glm::rotate(model, 0.3f, glm::vec3(-rotateX, -rotateY, -rotateZ));
			}
		}

		// skybox
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, rockTextureId);
		//glUniform1i(textureIndexLoc, 3);
		//glBindVertexArray(cubeVAO);
		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

