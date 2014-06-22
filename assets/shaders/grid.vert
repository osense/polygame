attribute vec3 inVertexPosition;
attribute vec3 inVertexNormal;

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

    gl_Position = WorldViewProjMat * vec4(inVertexPosition, 1.0);
}

