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
"layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1\n"
"out vec3 ourColor; // 向片段着色器输出一个颜色\n"
"//out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos, 1.0); \n"
"	//vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
"	ourColor = aColor; // 将ourColor设置为我们从顶点数据那里得到的输入颜色\n"
"}\0";

const char *fragmentShaderSource = "#version 400 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"//uniform vec4 ourColor; // 在OpenGL程序代码中设定这个变量\n"
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
	//构建着色器
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建顶点着色器
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	/*glShaderSource函数把要编译的着色器对象作为第一个参数。第二参数指定了传递的源码字符串数量，这里只有一个。第三个参数是顶点着色器真正的源码，第四个参数我们先设置为NULL。*/
	glCompileShader(vertexShader);//编译着色器判断是否成功
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{//不成功输出日志
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		//std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
	}
	// fragment shader 片段着色器
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
	int shaderProgram = glCreateProgram();//着色器程序
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);//链接着色器
								 // check for linking errors 检测是否失败
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
		//std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);//删除顶点着色器
	glDeleteShader(fragmentShader);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// 位置              // 颜色
		0.0f,0.9f,0.0f,1.0f, 0.0f, 0.0f,  //上三角形上顶点
		-0.1f,0.6f,0.0f,0.0f, 1.0f, 0.0f,   //上三角形左顶点
		0.1f, 0.6f, 0.0f,0.0f, 0.0f, 1.0f, //右顶点
		0.0f,0.6f,0.0f,0.0f, 0.0f, 1.0f, //中三角形上顶点
		-0.2f,0.3f,0.0f,0.0f, 1.0f, 0.0f,//中三角形左顶点
		0.2f, 0.3f, 0.0f,1.0f, 0.0f, 0.0f,//右顶点
		0.0f, 0.3f, 0.0f,0.0f, 1.0f, 0.0f,//下三角形上顶点
		-0.3f,0.0f, 0.0f,1.0f, 0.0f, 0.0f,//下三角形左顶点
		0.3f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f,//右顶点
		-0.05f,0.0f,0.0f,1.0f, 0.0f, 0.0f,
		0.05f,0.0f,0.0f,0.0f, 1.0f, 0.0f,
		-0.05f,-0.3f,0.0f,0.0f, 1.0f, 0.0f,
		0.05f,-0.3f,0.0f,0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f,   // 右上角
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f   // 左上角
	};

	unsigned int indices[] = { // 注意索引从0开始! 
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11, // 第一个三角形
		10,11, 12  // 第二个三角形
	};

	unsigned int VBO, VAO, EBO;
	/*顶点数组对象：Vertex Array Object，VAO*/
	glGenVertexArrays(1, &VAO);


	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	/*顶点缓冲对象：Vertex Buffer Object，VBO*/
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//复制顶点数组到顶点缓冲中
	/*索引缓冲对象：Element Buffer Object，EBO*/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//复制索引数组到索引缓冲
	//位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//颜色属性
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
		// 更新uniform颜色
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