attribute vec3 inVertexPosition;
attribute vec3 inVertexColor;

uniform mat4 WorldViewProjMat;

varying vec3 Color;

void main()
{
    Color = inVertexColor;
    gl_Position = WorldViewProjMat * vec4(inVertexPosition, 1.0);
}

