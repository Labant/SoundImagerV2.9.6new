#version 430
out vec4 color;
in vec4 varyingColor2;
uniform mat4 mv_matrix2;
uniform mat4 proj_matrix2;
void main(void)
{
	color = varyingColor2;
}