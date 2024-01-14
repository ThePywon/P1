/*#pragma once

namespace P1::graphics {
	struct ShaderProperty {
		char name[16];
		int length;
		int size;
		unsigned int type;
	};

	class Material {
	public:
		int program;
		unsigned int VBO, VAO;

		std::vector<ShaderProperty> attributes{};
		std::vector<ShaderProperty> uniforms{};

		Material() {
			const char *vertexShaderSource = "#version 460 core\n"
				"layout (location = 0) in vec3 aPos;\n"
				"uniform mat4 MVP;\n"
				"void main()\n"
				"{\n"
				"   gl_Position = MVP * vec4(aPos, 1.0);\n"
				"}\0";
			const char *fragmentShaderSource = "#version 460 core\n"
				"out vec4 FragColor;\n"
				"uniform vec3 color;\n"
				"void main()\n"
				"{\n"
				"   FragColor = vec4(color.x, color.y, color.z, 1.0f);\n"
				"}\n\0";

			int vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
			glCompileShader(vertexShader);

			int success;
			char infoLog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

			if(success) {
				std::cout << "Vertex shader compilation successfull!" << std::endl;
			}
			else {
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			}

			int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
			glCompileShader(fragmentShader);


			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

			if(success) {
				std::cout << "Fragment shader compilation successfull!" << std::endl;
			}
			else {
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			}

			program = glCreateProgram();
			glAttachShader(program, vertexShader);
			glAttachShader(program, fragmentShader);
			glLinkProgram(program);

			glGetProgramiv(program, GL_LINK_STATUS, &success);

			if(success) {
				std::cout << "Shader linking sucessfull!" << std::endl;
			}
			else {
				glGetProgramInfoLog(program, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			}

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			int count;
			glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);

			std::cout << "Attributes: " << count << std::endl;

			for(int i = 0; i < count; i++) {
				ShaderProperty attribute{};
				glGetActiveAttrib(program, i, 16, &attribute.length, &attribute.size, &attribute.type, attribute.name);
				attributes.push_back(attribute);
				std::cout << attribute.name << std::endl;
			}

			glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);

			std::cout << "Uniforms: " << count << std::endl;

			for(int i = 0; i < count; i++) {
				ShaderProperty uniform{};
				glGetActiveUniform(program, i, 16, &uniform.length, &uniform.size, &uniform.type, uniform.name);
				uniforms.push_back(uniform);
				std::cout << uniform.name << std::endl;
			}
		}

		void set_mesh(float* vertices_data, int length) {
			float vertices[length];

			for(int i = 0; i < length; i++)
				vertices[i] = *(vertices_data + i);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, 0); 
			glBindVertexArray(0);
		}

		~Material() {
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			glDeleteProgram(program);
		}
	};
}
*/