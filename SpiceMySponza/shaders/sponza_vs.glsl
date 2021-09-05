#version 330

// TODO: add uniforms to take in the matrix

//uniform float time_seconds;
uniform mat4 combinedMatrix;
uniform mat4 model_xform;



// TODO: add in variables for each of the streamed attributes

in vec3 vertex_position;

in vec3 vertex_normal;

in vec2 vertex_texcoord;


// TODO: specify out variables to be varied to the FS

out vec2 texCoord;
out vec3 fragmentPosition;
out vec3 fragmentNormal;



void main(void)
{
	 texCoord = vertex_texcoord;
     fragmentNormal = (model_xform * vec4(vertex_normal,0.0)).xyz;
     fragmentPosition =(model_xform * vec4(vertex_position,1.0)).xyz;
	 gl_Position = combinedMatrix * model_xform *  vec4(vertex_position, 1.0);
}
