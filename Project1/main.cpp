#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <math.h>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 400 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor; // ��ɫ����������λ��ֵΪ 1\n"
"out vec3 ourColor; // ��Ƭ����ɫ�����һ����ɫ\n"
"//out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos, 1.0); \n"
"	//vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
"	ourColor = aColor; // ��ourColor����Ϊ���ǴӶ�����������õ���������ɫ\n"
"}\0";

const char *fragmentShaderSource = "#version 400 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"//uniform vec4 ourColor; // ��OpenGL����������趨�������\n"
"void main()\n"
"{\n"
"   //FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
"	 //FragColor = vertexColor;\n"
"	 FragColor = vec4(ourColor, 1.0);\n"
"}\n\0";

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}


	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	//������ɫ��
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);//����������ɫ��
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	/*glShaderSource������Ҫ�������ɫ��������Ϊ��һ���������ڶ�����ָ���˴��ݵ�Դ���ַ�������������ֻ��һ���������������Ƕ�����ɫ��������Դ�룬���ĸ���������������ΪNULL��*/
	glCompileShader(vertexShader);//������ɫ���ж��Ƿ�ɹ�
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{//���ɹ������־
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		//std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
	}
	// fragment shader Ƭ����ɫ��
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
	}
	// link shaders
	int shaderProgram = glCreateProgram();//��ɫ������
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);//������ɫ��
								 // check for linking errors ����Ƿ�ʧ��
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
		//std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);//ɾ��������ɫ��
	glDeleteShader(fragmentShader);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// λ��              // ��ɫ
		0.0f,0.9f,0.0f,1.0f, 0.0f, 0.0f,  //���������϶���
		-0.1f,0.6f,0.0f,0.0f, 1.0f, 0.0f,   //���������󶥵�
		0.1f, 0.6f, 0.0f,0.0f, 0.0f, 1.0f, //�Ҷ���
		0.0f,0.6f,0.0f,0.0f, 0.0f, 1.0f, //���������϶���
		-0.2f,0.3f,0.0f,0.0f, 1.0f, 0.0f,//���������󶥵�
		0.2f, 0.3f, 0.0f,1.0f, 0.0f, 0.0f,//�Ҷ���
		0.0f, 0.3f, 0.0f,0.0f, 1.0f, 0.0f,//���������϶���
		-0.3f,0.0f, 0.0f,1.0f, 0.0f, 0.0f,//���������󶥵�
		0.3f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f,//�Ҷ���
		-0.05f,0.0f,0.0f,1.0f, 0.0f, 0.0f,
		0.05f,0.0f,0.0f,0.0f, 1.0f, 0.0f,
		-0.05f,-0.3f,0.0f,0.0f, 1.0f, 0.0f,
		0.05f,-0.3f,0.0f,0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f,   // ���Ͻ�
		0.5f, -0.5f, 0.0f,  // ���½�
		-0.5f, -0.5f, 0.0f, // ���½�
		-0.5f, 0.5f, 0.0f   // ���Ͻ�
	};

	unsigned int indices[] = { // ע��������0��ʼ! 
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11, // ��һ��������
		10,11, 12  // �ڶ���������
	};

	unsigned int VBO, VAO, EBO;
	/*�����������Vertex Array Object��VAO*/
	glGenVertexArrays(1, &VAO);


	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	/*���㻺�����Vertex Buffer Object��VBO*/
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//���ƶ������鵽���㻺����
	/*�����������Element Buffer Object��EBO*/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//�����������鵽��������
	//λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shaderProgram);
		// ����uniform��ɫ
		/*float timevalue = glfwgettime();
		float greenvalue = sin(timevalue) / 2.0f + 0.5f;
		int vertexcolorlocation = glgetuniformlocation(shaderprogram, "ourcolor");
		gluniform4f(vertexcolorlocation, 0.0f, greenvalue, 0.0f, 1.0f);
		*/
		/*glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);*/
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);


	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}