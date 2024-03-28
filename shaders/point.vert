#version 150 core
in vec4 position;
uniform vec2 size;
uniform float scale;
uniform vec2 location;
void main()
{
    gl_Position = vec4(2.0 * scale / size, 1.0, 1.0) * position + vec4(location, 0.0, 0.0);
}