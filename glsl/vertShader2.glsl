#version 430
layout (location = 0) in vec3 position2;
out vec4 varyingColor2;
uniform vec4 costomColor2;
uniform mat4 mv_matrix2;
uniform mat4 proj_matrix2;
void main(void)
{
	gl_Position = proj_matrix2 * mv_matrix2 * vec4(position2,1.0);
	varyingColor2 = costomColor2;
}