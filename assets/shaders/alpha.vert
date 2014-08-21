attribute vec3 inVertexPosition;
attribute vec3 inVertexNormal;
attribute vec3 inVertexColor;

uniform mat4 WorldViewProjMat;
uniform mat4 WorldViewMat;
uniform float CamFar;
uniform float Transform;
uniform float Alpha;

varying vec4 Color;

void main()
{
    vec3 Vertex = (WorldViewMat * vec4(inVertexPosition, 1.0)).xyz;
    float depth = length(Vertex) / CamFar;

    Color.rgb = inVertexColor;
    Color.a = Alpha;

    vec4 clipPos = WorldViewProjMat * vec4(inVertexPosition + inVertexNormal * (depth * 0.08), 1.0);
    clipPos.y += (1.0 - cos(clipPos.x / 3.14)) * Transform * 2.0;
    gl_Position = clipPos;
}

