#include "MyView.hpp"
#include <sponza/sponza.hpp>
#include <tygra/FileHelper.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>



//#include <cassert>

MyView::MyView()
{
}

MyView::~MyView() {
}

void MyView::setScene(const sponza::Context * scene)
{
    scene_ = scene;
	
}

void MyView::windowViewWillStart(tygra::Window * window)
{
	
    assert(scene_ != nullptr);

	GLint compile_status = GL_FALSE;

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertex_shader_string
		= tygra::createStringFromFile("resource:///sponza_vs.glsl");
	const char * vertex_shader_code = vertex_shader_string.c_str();
	glShaderSource(vertex_shader, 1,
		(const GLchar **)&vertex_shader_code, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE)
	{
		const int string_length = 1024;
		GLchar log[string_length] = "";
		glGetShaderInfoLog(vertex_shader, string_length, NULL, log);
		std::cerr << log << std::endl;
	}

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragment_shader_string
		= tygra::createStringFromFile("resource:///sponza_fs.glsl");
	const char * fragment_shader_code = fragment_shader_string.c_str();
	glShaderSource(fragment_shader, 1,
		(const GLchar **)&fragment_shader_code, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE)
	{
		const int string_length = 1024;
		GLchar log[string_length] = "";
		glGetShaderInfoLog(fragment_shader, string_length, NULL, log);
		std::cerr << log << std::endl;
	}

	// Create shader program & shader in variables
	shader_program_ = glCreateProgram();
	glAttachShader(shader_program_, vertex_shader);

	// TODO: glBindAttribLocation for all shader streamed IN variables e.g.
	//glBindAttribLocation(shader_program_, kVertexPosition, "vertex_position");
	
	GLuint VertexPosition = 0; // check here
	GLuint fragmentPosition = 0;
	GLuint vertexNormalID = 1;
	GLuint texCoordID = 2;
	

	glm::vec3 vertexNormal;

	
	glBindAttribLocation(shader_program_, VertexPosition, "vertex_position");
	glBindAttribLocation(shader_program_, vertexNormalID, "vertex_normal");
	glBindAttribLocation(shader_program_, texCoordID, "vertex_texcoord");
	glDeleteShader(vertex_shader);
	glAttachShader(shader_program_, fragment_shader);
	glBindFragDataLocation(shader_program_, fragmentPosition, "fragmentPosition");
	glDeleteShader(fragment_shader);
	glLinkProgram(shader_program_);


	GLint link_status = GL_FALSE;
	glGetProgramiv(shader_program_, GL_LINK_STATUS, &link_status);
	if (link_status != GL_TRUE)
	{
		const int string_length = 1024;
		GLchar log[string_length] = "";
		glGetProgramInfoLog(shader_program_, string_length, NULL, log);
		std::cerr << log << std::endl;
	}

	/*
		The framework provides a builder class that allows access to all the mesh data	
	*/





	

	sponza::GeometryBuilder builder;
	const auto& source_meshes = builder.getAllMeshes();

	// We can loop through each mesh in the scene
	for each (const sponza::Mesh& source in source_meshes)
	{
		// Each mesh has an id that you will need to remember for later use
		// obained by calling source.getId()

		// To access the actual mesh raw data we can get the array e.g. // need to complete
		Mesh myMesh;
		myMesh.meshId = source.getId();


		 

		const auto& positions = source.getPositionArray();
		const auto& normals = source.getNormalArray();
		const auto& elements = source.getElementArray();
		const auto& textureCoords = source.getTextureCoordinateArray();

		GLuint lightVAO;

		glGenBuffers(1, &myMesh.positionsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, myMesh.positionsVBO);
		glBufferData(GL_ARRAY_BUFFER,
			positions.size() * sizeof(glm::vec3), // size of data in bytes
			positions.data(), // pointer to the data
			GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, kNullId);

		glGenBuffers(1, &myMesh.elementVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myMesh.elementVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			elements.size() * sizeof(unsigned int),
			elements.data(),
			GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, kNullId);
		myMesh.numElements = elements.size();

		glGenBuffers(1, &myMesh.normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, myMesh.normalVBO);
		glBufferData(GL_ARRAY_BUFFER,
			normals.size() * sizeof(glm::vec3), // size of data in bytes
			normals.data(), // pointer to the data
			GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, kNullId);

		glGenBuffers(1, &myMesh.texcoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, myMesh.texcoordVBO);
		glBufferData(GL_ARRAY_BUFFER,
			textureCoords.size() * sizeof(glm::vec2), // size of data in bytes
			textureCoords.data(), // pointer to the data
			GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, kNullId);
	

		glGenVertexArrays(1, &myMesh.VAO);
		glBindVertexArray(myMesh.VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myMesh.elementVBO);
		glBindBuffer(GL_ARRAY_BUFFER, myMesh.normalVBO);
		glEnableVertexAttribArray(vertexNormalID);
		glVertexAttribPointer(vertexNormalID, 3, GL_FLOAT, GL_FALSE,
			sizeof(glm::vec3), TGL_BUFFER_OFFSET(0));

		glBindBuffer(GL_ARRAY_BUFFER, myMesh.normalVBO);
		glEnableVertexAttribArray(vertexNormalID);
		glVertexAttribPointer(vertexNormalID, 3, GL_FLOAT, GL_FALSE,
			sizeof(glm::vec3), TGL_BUFFER_OFFSET(0));
		glBindBuffer(GL_ARRAY_BUFFER, kNullId);

		glBindBuffer(GL_ARRAY_BUFFER, myMesh.positionsVBO);
		glEnableVertexAttribArray(VertexPosition);
		glVertexAttribPointer(VertexPosition, 3, GL_FLOAT, GL_FALSE,
			sizeof(glm::vec3), TGL_BUFFER_OFFSET(0));
		glBindBuffer(GL_ARRAY_BUFFER, kNullId);
		

		glBindBuffer(GL_ARRAY_BUFFER, myMesh.texcoordVBO);
		glEnableVertexAttribArray(texCoordID);
		glVertexAttribPointer(texCoordID, 2, GL_FLOAT, GL_FALSE,
			sizeof(glm::vec2), TGL_BUFFER_OFFSET(0));
		glBindBuffer(GL_ARRAY_BUFFER, kNullId);
		glBindVertexArray(kNullId);


		m_meshVector.push_back(myMesh);




		// TODO: you also need to get the normals, elements and texture coordinates in a similar way

		// TODO:
		// Create VBOs for position, normals, elements and texture coordinates

		// TODO
		// Create a VAO to wrap all the VBOs
		
		// TODO: store in a mesh structure and add to a container for later use
	}

	const auto& all_materials = scene_->getAllMaterials();
	for (const auto&mat : all_materials)
	{

		if (Textures.find(mat.getDiffuseTexture()) == Textures.end())
		{
			textureFileNames.push_back(mat.getDiffuseTexture());
			loadTexture(mat.getDiffuseTexture());
		}
		if (Textures.find(mat.getSpecularTexture()) == Textures.end())
		{
			textureFileNames.push_back(mat.getSpecularTexture());
			loadTexture(mat.getSpecularTexture());
		}

	

	}

/*	for (const auto&fileName : textureFileNames)
	{

		loadTexture(fileName);
		

	}*/
	// put texture code here :)
}

void MyView::windowViewDidReset(tygra::Window * window,
                                int width,
                                int height)
{
    glViewport(0, 0, width, height);

	
}

void MyView::windowViewDidStop(tygra::Window * window)
{
}

void MyView::windowViewRender(tygra::Window * window)
{
	assert(scene_ != nullptr);

	// Configure pipeline settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Clear buffers from previous frame
	glClearColor(0.f, 0.f, 0.25f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader_program_);
	 
	// Compute viewport
	GLint viewport_size[4];
	glGetIntegerv(GL_VIEWPORT, viewport_size);
	const float aspect_ratio = viewport_size[2] / (float)viewport_size[3];

	// Note: the code above is supplied for you and already works

	// TODO: Compute projection matrix
	// You can get the far plane distance, the near plane distance and the fov from
	// scene_->GetCamera().
	glm::mat4 projection_xform = glm::perspective(glm::radians(scene_->getCamera().getVerticalFieldOfViewInDegrees()), aspect_ratio, scene_->getCamera().getNearPlaneDistance(), scene_->getCamera().getFarPlaneDistance());

	// TODO: Compute view matrix
	// You can get the camera position, look at and world up from the scene e.g.
	const auto& camera_pos = (const glm::vec3&)scene_->getCamera().getPosition();
	const auto& camera_at_pos = (const glm::vec3&)scene_->getCamera().getPosition() + (const glm::vec3&)scene_->getCamera().getDirection();
	const auto& world_up = (const glm::vec3&)scene_->getUpDirection();

	// Compute camera view matrix and combine with projection matrix
	glm::mat4 view_xform = glm::lookAt(camera_pos, camera_at_pos, world_up);
	

	glm::mat4 combinedMatrix = projection_xform * view_xform;
	glm::vec3 view = (const glm::vec3&)scene_->getCamera().getPosition();

	GLuint objectColour = glGetUniformLocation(shader_program_, "objectColour");
	GLuint LightColour = glGetUniformLocation(shader_program_, "lightColour");
	GLuint lightPosition = glGetUniformLocation(shader_program_, "lightPosition");
	GLuint viewPosition = glGetUniformLocation(shader_program_, "viewPosition");
	GLuint ambientScene = glGetUniformLocation(shader_program_, "ambientScene");
	glm::vec3 ambientLightIntensity = (const glm::vec3&)scene_->getAmbientLightIntensity();
	
	glUseProgram(shader_program_);
	glUniform3f(objectColour, 1.0f, 0.5f, 0.31f);
	glUniform3f(LightColour, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosition, 0,20,0);
	glUniform3f(viewPosition, view.x, view.y, view.z);
	glUniform3fv(ambientScene, 1, glm::value_ptr(ambientLightIntensity));
	


	


	GLuint combinedMatrixID = glGetUniformLocation(shader_program_,"combinedMatrix");

	glUniformMatrix4fv(combinedMatrixID, 1, false, glm::value_ptr(combinedMatrix));

	// TODO: create combined view * projection matrix and pass to shader as a uniform

	// TODO: Get light data from scene via scene_->getAllLights()
	// then plug the values into the shader - you may want to leave this until you have a basic scene showing

	// TODO: Render each mesh
	// Loop through your mesh container e.g.
	for (const auto& mesh : m_meshVector)
	{
		
		
		

		// Each mesh can be repeated in the scene so we need to ask the scene for all instances of the mesh
		// and render each instance with its own model matrix
		// To get the instances we need to use the meshId we stored earlier e.g.
		const auto& instances = scene_->getInstancesByMeshId(mesh.meshId);
		
		// then loop through all instances
			// for each instance you can call getTransformationMatrix 
			// this then needs passing to the shader as a uniform
		for (const auto& instance : instances)
		{

			glm::mat4 model_xform = (const glm::mat4x3&)scene_->getInstanceById(instance).getTransformationMatrix();
			GLuint model_xform_id = glGetUniformLocation(shader_program_, "model_xform");
			glUniformMatrix4fv(model_xform_id, 1, false, glm::value_ptr(model_xform));




			const auto& material = scene_->getMaterialById(scene_->getInstanceById(instance).getMaterialId());

			glm::vec3 materialDiffuse = (const glm::vec3&)material.getDiffuseColour();

			GLuint materialDiffuseID = glGetUniformLocation(shader_program_, "materialObject.diffuseColour");

			glUniform3fv(materialDiffuseID, 1, glm::value_ptr(materialDiffuse));

			glm::vec3 materialSpecular = (const glm::vec3&)material.getSpecularColour();

			GLuint materialSpecularID = glGetUniformLocation(shader_program_, "materialObject.SpecularColour");

			glUniform3fv(materialSpecularID, 1, glm::value_ptr(materialSpecular));

			glm::float32 specularIntensity = (const glm::float32&)material.getShininess();

			GLuint materialSpecularIntensityID = glGetUniformLocation(shader_program_, "materialObject.specularIntensity");

			glUniform1f(materialSpecularIntensityID, specularIntensity);

			bool isShiny = material.isShiny();

			GLuint isShinyID = glGetUniformLocation(shader_program_, "material.isShiny");

			glUniform1i(isShinyID, isShiny);

			glm::vec3 ambientColour = (const glm::vec3&) material.getAmbientColour();

			GLuint ambientColourID = glGetUniformLocation(shader_program_, "materialObject.ambientColour");

			glUniform3fv(ambientColourID, 1, glm::value_ptr(ambientColour));
			
			if (material.getDiffuseTexture() != "")
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, Textures[material.getDiffuseTexture()]);
				GLuint diffuseTexureID = glGetUniformLocation(shader_program_, "materialObject.diffuseTexture");
				glUniform1i(diffuseTexureID, 0);
				bool hasDiffuseTexture = true;
				GLuint hasDiffuseID = glGetUniformLocation(shader_program_, "materialObject.hasDiffuseTexture");
				glUniform1i(hasDiffuseID, hasDiffuseTexture);
			}

			else
			{
				bool hasDiffuseTexture = false;
				GLuint hasDiffuseID = glGetUniformLocation(shader_program_, "materialObject.hasDiffuseTexture");
				glUniform1i(hasDiffuseID, hasDiffuseTexture);
			}

			if (material.getSpecularTexture() != "")
			{
				
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, Textures[material.getSpecularTexture()]);
				GLuint diffuseSpecularID = glGetUniformLocation(shader_program_, "materialObject.specularTexture");
				glUniform1i(diffuseSpecularID,1);
				bool hasSpecularTexture = true;
				GLuint hasSpecularId = glGetUniformLocation(shader_program_, "materialObject.hasSpecularTexture");
				glUniform1i(hasSpecularId, hasSpecularTexture);
			}
			else
			{
				bool hasSpecularTexture = false;
				GLuint hasSpecularId = glGetUniformLocation(shader_program_, "materialObject.hasSpecularTexture");
				glUniform1i(hasSpecularId, hasSpecularTexture);
			}



			
			glBindVertexArray(mesh.VAO);

			
			
			glDrawElements(GL_TRIANGLES, mesh.numElements, GL_UNSIGNED_INT, 0);
		}
		// Materials - leave this until you get the main scene showing
		// Each instance of the mesh has its own material accessed like so:
		// Get material for this instance
		//const auto& material_id = scene_->getInstanceById(instances.at(i)).getMaterialId();
		//const auto& material = scene_->getMaterialById(material_id);
		// You can then get the material colours from material.XX - you need to pass these to the shader

		// Finally you render the mesh e.g.
					
	}
}

void MyView::loadTexture(std::string fileName)
{
	
	

	tygra::Image texture_image = tygra::createImageFromPngFile("resource:///" + fileName);
	

	if (texture_image.doesContainData()) 
	{
		
		glGenTextures(1, &Textures[fileName]);
		glBindTexture(GL_TEXTURE_2D, Textures[fileName]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		GLenum pixel_formats[] = { 0, GL_RED, GL_RG, GL_RGB, GL_RGBA };
		glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGBA,
			texture_image.width(),
			texture_image.height(),
			0,
			pixel_formats[texture_image.componentsPerPixel()],
			texture_image.bytesPerComponent() == 1
			? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT,
			texture_image.pixelData());
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, kNullId);
		
	
	}

}


