#version 430
layout (location = 0) in vec2 position3;
out vec4 varyingColor3;
uniform vec4 costomColor3;
uniform mat4 mv_matrix3;
uniform mat4 proj_matrix3;
void main(void)
{
	gl_Position = proj_matrix3 * mv_matrix3 * vec4(position3,0.0,1.0);
	varyingColor3 = costomColor3;
}