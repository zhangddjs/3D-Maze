// Std. Includes
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Vertices.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//PrimMazeCreater
#include "Prim.h"

// Other Libs
#include <SOIL.h>

// Properties
GLuint screenWidth = 800, screenHeight = 600;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
GLuint loadTexture(GLchar* path, GLboolean alpha = false);
GLuint loadCubemap(std::vector<const GLchar*> faces);

// Camera
Camera camera(glm::vec3(1.0f, 1.0f, 1.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
	printf("选择背景(1.宇宙;2.midnight;3.钢铁之都;4.暴雪):");
	int bg;
	scanf("%d", &bg);
	PrimMaze();
	glm::vec3 pointLightPositions[] = {
		glm::vec3(1.0f, 2.0f, 1.0f),
		glm::vec3(size * 2 - 3, 2.0f, size * 2 - 1)
	};
	camera.Position.x = size * 2 - 3;
	camera.Position.z = size * 2 + 5;

	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Setup and compile our shaders
	Shader shader("shader/advanced.vs", "shader/blending.frag");
	Shader lightingShader("shader/Lightcasters.vs", "shader/Multiplelights.frag");
	Shader skyboxShader("shader/skybox.vs", "shader/skybox.frag");
	Shader lampShader("shader/lamp.vs", "shader/lamp.frag");

#pragma region "object_initialization"
	// Setup plane VAO
	GLuint planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glBindVertexArray(0);
	// Setup skyBoxVAO
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
	//Setup wall VAO
	GLuint wallVAO, wallVBO;
	glGenVertexArrays(1, &wallVAO);
	glGenBuffers(1, &wallVBO);
	glBindVertexArray(wallVAO);
	glBindBuffer(GL_ARRAY_BUFFER, wallVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices), wallVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glBindVertexArray(0);
	// Load textures
	GLuint cubeTexture = loadTexture("sucai/container2.png");
	GLuint floorTexture = loadTexture("sucai/wall5.jpg");
	GLuint transparentTexture = loadTexture("sucai/grass.png", true);
	GLuint wallTexture = loadTexture("sucai/wall6.jpg");
#pragma endregion

	//入口
	std::vector<glm::vec3> vegetation;
	vegetation.push_back(glm::vec3(size * 2 - 5, 1.0f, size * 2 + 1));
	vegetation.push_back(glm::vec3(size * 2 - 5, 1.0f, size * 2 + 3));
	vegetation.push_back(glm::vec3(size * 2 - 5, 1.0f, size * 2 + 5));
	vegetation.push_back(glm::vec3(size * 2 - 5, 1.0f, size * 2 + 7));
	vegetation.push_back(glm::vec3(size * 2 - 1, 1.0f, size * 2 + 1));
	vegetation.push_back(glm::vec3(size * 2 - 1, 1.0f, size * 2 + 3));
	vegetation.push_back(glm::vec3(size * 2 - 1, 1.0f, size * 2 + 5));
	vegetation.push_back(glm::vec3(size * 2 - 1, 1.0f, size * 2 + 7));
	vegetation.push_back(glm::vec3(size * 2 - 3, 1.0f, size * 2 + 7));

	std::vector<glm::vec3> vegetationII;
	vegetation.push_back(glm::vec3(size * 2 - 3, -1.0f, size * 2 + 1));
	vegetation.push_back(glm::vec3(size * 2 - 3, -1.0f, size * 2 + 3));
	vegetation.push_back(glm::vec3(size * 2 - 3, -1.0f, size * 2 + 5));

	//天空盒
	GLchar* bgPath;
	GLchar* bgPathRT;
	GLchar* bgPathLF;
	GLchar* bgPathUP;
	GLchar* bgPathDN;
	GLchar* bgPathBK;
	GLchar* bgPathFT;

	switch (bg)
	{
	case 1:
		bgPath = "sucai/ame_nebula/purplenebula_";
		bgPathRT = "sucai/ame_nebula/purplenebula_rt.jpg";
		bgPathLF = "sucai/ame_nebula/purplenebula_lf.jpg";
		bgPathUP = "sucai/ame_nebula/purplenebula_up.jpg";
		bgPathDN = "sucai/ame_nebula/purplenebula_dn.jpg";
		bgPathBK = "sucai/ame_nebula/purplenebula_bk.jpg";
		bgPathFT = "sucai/ame_nebula/purplenebula_ft.jpg";
		break;
	case 2:
		bgPath = "sucai/midnight/mp_midnight/midnight-silence_";
		bgPathRT = "sucai/midnight/mp_midnight/midnight-silence_rt.jpg";
		bgPathLF = "sucai/midnight/mp_midnight/midnight-silence_lf.jpg";
		bgPathUP = "sucai/midnight/mp_midnight/midnight-silence_up.jpg";
		bgPathDN = "sucai/midnight/mp_midnight/midnight-silence_dn.jpg";
		bgPathBK = "sucai/midnight/mp_midnight/midnight-silence_bk.jpg";
		bgPathFT = "sucai/midnight/mp_midnight/midnight-silence_ft.jpg";
		break;
	case 3:
		bgPath = "sucai/city/nec_city/city_";
		bgPathRT = "sucai/city/nec_city/city_rt.jpg";
		bgPathLF = "sucai/city/nec_city/city_lf.jpg";
		bgPathUP = "sucai/city/nec_city/city_up.jpg";
		bgPathDN = "sucai/city/nec_city/city_dn.jpg";
		bgPathBK = "sucai/city/nec_city/city_bk.jpg";
		bgPathFT = "sucai/city/nec_city/city_ft.jpg";
		break;
	case 4:
		bgPath = "sucai/blizzard/mp_blizzard/blizzard_";
		bgPathRT = "sucai/blizzard/mp_blizzard/blizzard_rt.jpg";
		bgPathLF = "sucai/blizzard/mp_blizzard/blizzard_lf.jpg";
		bgPathUP = "sucai/blizzard/mp_blizzard/blizzard_up.jpg";
		bgPathDN = "sucai/blizzard/mp_blizzard/blizzard_dn.jpg";
		bgPathBK = "sucai/blizzard/mp_blizzard/blizzard_bk.jpg";
		bgPathFT = "sucai/blizzard/mp_blizzard/blizzard_ft.jpg";
		break;
	default:
		break;
	}
	std::vector<const GLchar*> faces;
	faces.push_back(bgPathRT);
	faces.push_back(bgPathLF);
	faces.push_back(bgPathUP);
	faces.push_back(bgPathDN);
	faces.push_back(bgPathBK);
	faces.push_back(bgPathFT);
	GLuint cubemapTexture = loadCubemap(faces);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = (currentFrame - lastFrame) * 2;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		Do_Movement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//skyBox

		glDepthMask(GL_FALSE);// Remember to turn depth writing off
		skyboxShader.Use();
		glm::mat4 model;
		glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		//glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(skyboxShader.Program, "skybox"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);


		// Draw objects
		lightingShader.Use();
		//设置材质
		GLint matSpecularLoc = glGetUniformLocation(lightingShader.Program, "material.specular");
		GLint matShineLoc = glGetUniformLocation(lightingShader.Program, "material.shininess");
		glUniform3f(matSpecularLoc, 0.5f, 0.5f, 0.5f);
		glUniform1f(matShineLoc, 8.0f);
		//光的属性
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.35);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.44);
		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.35);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.44);
		//手电筒
		GLint lightAmbientLoc = glGetUniformLocation(lightingShader.Program, "spotLight.ambient");
		GLint lightDiffuseLoc = glGetUniformLocation(lightingShader.Program, "spotLight.diffuse");
		GLint lightSpecularLoc = glGetUniformLocation(lightingShader.Program, "spotLight.specular");
		GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "spotLight.position");
		GLint lightSpotdirLoc = glGetUniformLocation(lightingShader.Program, "spotLight.direction");
		GLint lightSpotCutOffLoc = glGetUniformLocation(lightingShader.Program, "spotLight.cutOff");
		GLint lightSpotOuterCutOffLoc = glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff");
		glUniform3f(lightPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform3f(lightSpotdirLoc, camera.Front.x, camera.Front.y, camera.Front.z);
		glUniform1f(lightSpotCutOffLoc, glm::cos(glm::radians(20.0f)));
		glUniform1f(lightSpotOuterCutOffLoc, glm::cos(glm::radians(40.0f)));
		glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
		glUniform3f(lightDiffuseLoc, 0.5f, 0.5f, 0.5f);// 让我们把这个光调暗一点，这样会看起来更自然
		glUniform3f(lightSpecularLoc, 0.5f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.35);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.44);


		//shader.Use();
		
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		// Floor
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 0);

		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		for (int i = 0; i < size / 10; i++)
		{
			for (int j = 0; j < size / 10; j++)
			{
				model = glm::mat4();
				//model = glm::scale(model, glm::vec3(size / 10, 1.0f, size / 10));
				model = glm::translate(model, glm::vec3(10.0f, 1.0f, 10.0f));
				model = glm::translate(model, glm::vec3(20.0f * i, 0.0f, 20.0f * j));
				glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, wallTexture);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, wallTexture);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 0);
		glBindVertexArray(wallVAO);
		glEnable(GL_CULL_FACE);
		for (int h = 0; h < size; h++)
		{
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(-1, 1.0f, h * 2 + 1));
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glBindTexture(GL_TEXTURE_2D, wallTexture);
			glDrawArrays(GL_TRIANGLES, 0, 30);
		};
		for (int k = 0; k <= size; k++)
		{
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(k * 2 - 1, 1.0f, -1));
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glBindTexture(GL_TEXTURE_2D, wallTexture);
			glDrawArrays(GL_TRIANGLES, 0, 30);
		};
		for (int h = 0; h < size; h++)
		{

			for (int k = 0; k < size; k++)
			{
				if (b[h][k] == 0 && c[h][k] == 0)
				{
					model = glm::mat4();
					model = glm::translate(model, glm::vec3(k * 2 + 1, 1.0f, h * 2 + 1));
					glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
					glBindTexture(GL_TEXTURE_2D, wallTexture);
					glDrawArrays(GL_TRIANGLES, 0, 30);
				}
			}
		}

		//入口wall
		for (int i = 0; i < vegetation.size(); i++)
		{
			model = glm::mat4();
			model = glm::translate(model, vegetation[i]);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 30);
		}
		//入口floor
		for (int i = 0; i < vegetationII.size(); i++)
		{
			model = glm::mat4();
			model = glm::translate(model, vegetationII[i]);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 30);
		}
		glDisable(GL_CULL_FACE);

		//lamp
		lampShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		for (int i = 0; i < 2; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(wallVAO);
			glDrawArrays(GL_TRIANGLES, 0, 30);
			glBindVertexArray(0);
		}

		//通关
		if (camera.Position.x > 0.0
			&& camera.Position.x < 2.0
			&& camera.Position.z > 0.0
			&& camera.Position.z < 2.0)
		{
			camera.ProcessKeyboard(UP, deltaTime);
		}

		// Swap the buffers
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &wallVAO);
	glDeleteBuffers(1, &wallVBO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);

	glfwTerminate();
	return 0;
}

// This function loads a texture from file. Note: texture loading functions like these are usually 
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio). 
// For learning purposes we'll just define it as a utility function.
GLuint loadTexture(GLchar* path, GLboolean alpha)
{
	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);	// Use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes value from next repeat 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}

//天空盒
GLuint loadCubemap(std::vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
			);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

#pragma region "User input"

// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

#pragma endregion
