#include "Renderer.h"
#include "Camera.h"

Renderer::Renderer() 
{

}

// On exit must clean up any OpenGL resources e.g. the program, the buffers
Renderer::~Renderer()
{
	// TODO: clean up any memory used including OpenGL objects via glDelete* calls
	glDeleteProgram(m_program);
	glDeleteBuffers(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

// Use IMGUI for a simple on screen GUI
void Renderer::DefineGUI()
{
	// Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	{
		ImGui::Begin("RTG");                    // Create a window called "RTG" and append into it.

		ImGui::Text("Visibility.");             // Display some text (you can use a format strings too)	

		ImGui::Checkbox("Wireframe", &m_wireframe);	// A checkbox linked to a member variable

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
		ImGui::End();
	}
}

// Load, compile and link the shaders and create a program object to host them
bool Renderer::CreateProgram()
{
	// Create a new program (returns a unqiue id)
	m_program = glCreateProgram();

	glViewport(0, 0, 1280, 720);

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/vertex_shader.glsl") };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/fragment_shader.glsl") };
	if (vertex_shader == 0 || fragment_shader == 0)
		return false;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(m_program, vertex_shader);

	// The attibute 0 maps to the input stream "vertex_position" in the vertex shader
	// Not needed if you use (location=0) in the vertex shader itself
	//glBindAttribLocation(m_program, 0, "vertex_position");

	// Attach the fragment shader (copies it)
	glAttachShader(m_program, fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(m_program))
		return false;

	return !Helpers::CheckForGLError();
}

// Load / create geometry into OpenGL buffers	
bool Renderer::InitialiseGeometry()
{
	// Load and compile shaders into m_program
	if (!CreateProgram())
		return false;

	// Helpers has an object for loading 3D geometry, supports most types
	
	/* Load in the jeep
	Helpers::ModelLoader loader;
	if (!loader.LoadFromFile("Data\\Models\\Jeep\\jeep.obj"))
		return false;

	 Now we can loop through all the mesh in the loaded model:
	for (const Helpers::Mesh& mesh : loader.GetMeshVector())
	{
		// We can extract from the mesh via:
		//mesh.vertices  - a vector of glm::vec3 (3 floats) giving the position of each vertex
		//mesh.elements - a vector of unsigned ints defining which vertices make up each triangle

		// TODO: create VBO for the vertices and a EBO for the elements
		// TODO: create a VBA to wrap everything and specify locations in the shaders

		std::vector<GLfloat> vertices = { -1.0f, -1.0f, 0.0f,  1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f }; //replace these to the mesh vertices
		GLuint gPositionsVBO; //Vertex Buffer Object to hold vertex positions 
		glGenBuffers(1, &gPositionsVBO); //create 1 buffer id and put the resulting identifier in VBO obj
		glBindBuffer(GL_ARRAY_BUFFER, gPositionsVBO);//bind the buffer to the context at the GL_ARRAY_BUFFER binding point
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);//binding to 0 == clear binding

	}*/
	std::vector<GLfloat> vertices = {
		// Front face
		-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  // Bottom-left
		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Bottom-right
		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,// Top-right
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,// Top-left

		// Back face
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,// Bottom-left
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,// Bottom-right
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, // Top-right
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f // Top-left
	};
	std::vector<GLuint> indices = {
		// Front face
		0, 1, 2,
		2, 3, 0,

		// Back face
		6, 5, 4, //4, 5, 6,	they where flipped. Should be always anticlockwise fromto the camera perspective
		4, 7, 6, //6, 7, 4,	they where flipped. Should be always anticlockwise fromto the camera perspective

		// Left face
		0, 3, 7,
		7, 4, 0,

		// Right face
		1, 5, 6,
		6, 2, 1,

		// Top face
		3, 2, 6,
		6, 7, 3,

		// Bottom face
		5, 1, 0, //0, 1, 5, they where flipped. Should be always anticlockwise fromto the camera perspective
		0, 4, 5  //5, 4, 0	they where flipped. Should be always anticlockwise fromto the camera perspective
	};
	m_numElements = indices.size();

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	// Good idea to check for an error now:	
	Helpers::CheckForGLError();

	
	return true;
}


// Render the scene. Passed the delta time since last called.
void Renderer::Render(const Helpers::Camera& camera, float deltaTime)
{			

	// Configure pipeline settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Wireframe mode controlled by ImGui
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Clear buffers from previous frame
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glUseProgram(m_program);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);


	glm::mat4 mvp = projection * view * model;

	GLuint mvp_location = glGetUniformLocation(m_program, "uMVP");
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));



	/*float timeValue = glfwGetTime();
	float red_value = (cos(timeValue) / 2.0f) + 0.5f;
	float green_value = (sin(timeValue) / 2.0f) + 0.5f;
	float blue_value = (tan(timeValue) / 2.0f) + 0.5f;
	GLuint custom_colour_location = glGetUniformLocation(m_program, "custom_colour");
	glUniform4f(custom_colour_location, red_value, green_value, blue_value, 1.0f);*/






	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_numElements, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	// TODO: Compute viewport and projection matrix

	// TODO: Compute camera view matrix and combine with projection matrix for passing to shader

	// TODO: Send the combined matrix to the shader in a uniform

	// TODO: render each mesh. Send the correct model matrix to the shader in a uniform

	// Always a good idea, when debugging at least, to check for GL errors each frame
	Helpers::CheckForGLError();
}

