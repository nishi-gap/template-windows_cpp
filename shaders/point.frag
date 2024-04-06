#version 150 core
out vec4 fragment;
in vec4 vertex_color;
void main()
{
 fragment = vertex_color;
}