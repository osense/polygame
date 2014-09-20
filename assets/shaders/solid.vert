attribute vec3 inVertexPosition;
attribute vec3 inVertexNormal;
//attribute vec3 inVertexColor;

uniform mat4 WorldViewProjMat;
uniform mat4 WorldViewMat;
uniform float CamFar;
uniform vec3 Color;
uniform float ThicknessBias;
uniform float Transform;

varying vec3 col;

void main()
{
    vec3 Vertex = (WorldViewMat * vec4(inVertexPosition, 1.0)).xyz;
    float depth = length(Vertex) / CamFar;

    float alpha = min(1.0, 8.0 - 10.0*depth);
    col = Color/* * inVertexColor*/ * alpha;

    vec4 clipPos = WorldViewProjMat * vec4(inVertexPosition + inVertexNormal * (depth * (0.08 + ThicknessBias)), 1.0);
    clipPos.y += (1.0 - cos(clipPos.x / 3.14)) * Transform * 2.0;
    clipPos.y += (1.0 - cos(clipPos.z / 6.28)) * Transform * 4.0;
    gl_Position = clipPos;
}

