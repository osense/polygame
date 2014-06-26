attribute vec3 inVertexPosition;
attribute vec3 inVertexNormal;
//attribute vec2 inTexCoord0;

uniform mat4 WorldViewProjMat;
uniform mat4 WorldViewMat;
uniform float CamFar;

varying vec3 Color;
varying float Alpha;

void main()
{
    vec3 Vertex = (WorldViewMat * vec4(inVertexPosition, 1.0)).xyz;
    float depth = length(Vertex) / CamFar;//Vertex.z / CamFar;

    Color = mix(vec3(0.7, 0.14, 0.1), vec3(0.7, 0.14, 1.0), depth*(1.0+depth));

    Alpha = min(1.0, 5.5 - 10.0*depth);

    vec4 clipPos = WorldViewProjMat * vec4(inVertexPosition + inVertexNormal * depth * 0.075, 1.0);
    /*vec3 dir = inVertexNormal;
    clipPos.xyz += dir * 0.05;*/
    gl_Position = clipPos;
}

