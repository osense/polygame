attribute vec3 inVertexPosition;
attribute vec3 inVertexNormal;
attribute vec3 inVertexColor;

uniform mat4 WorldViewProjMat;
uniform mat4 WorldViewMat;
uniform float CamFar;

varying vec3 Color;

void main()
{
    vec3 Vertex = (WorldViewMat * vec4(inVertexPosition, 1.0)).xyz;
    float depth = length(Vertex) / CamFar;

    Color = inVertexColor;

    gl_Position = WorldViewProjMat * vec4(inVertexPosition + inVertexNormal * (depth * 0.04 + 0.04), 1.0);
}

