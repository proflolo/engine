#version 430 core
layout(location = 0) out vec4 FragColor;
layout(location = 0) in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)
void main()
{
    FragColor = vertexColor;
}