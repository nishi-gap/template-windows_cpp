#version 150 core
in vec4 position;
uniform mat4 modelview;
uniform mat4 projection;
void main()
{
    gl_Position =  modelview * position;
    gl_Position = projection * modelview * position;
    
}