attribute vec3 inVertexPosition;
attribute vec3 inVertexNormal;
//attribute vec2 inTexCoord0;

uniform mat4 WorldViewProjMat;
uniform mat4 WorldViewMat;
uniform float CamFar;
uniform vec3 NearColor, FarColor;

varying vec3 Color;

void main()
{
    vec3 Vertex = (WorldViewMat * vec4(inVertexPosition, 1.0)).xyz;
    float depth = length(Vertex) / CamFar;//Vertex.z / CamFar;

    float alpha = min(1.0, 8.0 - 10.0*depth);
    Color = mix(NearColor, FarColor, depth) * alpha;

    vec4 clipPos = WorldViewProjMat * vec4(inVertexPosition + inVertexNormal * (depth * 0.08), 1.0);
    gl_Position = clipPos;
}

