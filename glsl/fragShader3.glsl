#version 430
out vec4 color3;
in vec4 varyingColor3;
uniform mat4 mv_matrix3;
uniform mat4 proj_matrix3;
void main(void)
{
	color3 = varyingColor3;
}