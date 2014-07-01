attribute vec3 inVertexPosition;
attribute vec3 inVertexNormal;
//attribute vec2 inTexCoord0;

uniform mat4 WorldViewProjMat;
uniform mat4 WorldViewMat;
uniform float CamFar;
uniform vec3 NearColor, FarColor;

varying vec3 Color;
varying float Alpha;

void main()
{
    vec3 Vertex = (WorldViewMat * vec4(inVertexPosition, 1.0)).xyz;
    float depth = length(Vertex) / CamFar;//Vertex.z / CamFar;

    Color = mix(NearColor, FarColor, depth);
    //Color = mix(vec3(0.0, 1.0, 0.0), vec3(1.0, 1.0, 0.0), depth*(1.5+depth));

    Alpha = min(1.0, 5.5 - 10.0*depth);

    vec4 clipPos = WorldViewProjMat * vec4(inVertexPosition + inVertexNormal * (depth * 0.1), 1.0);
    /*vec3 dir = inVertexNormal;
    clipPos.xyz += dir * 0.05;*/
    gl_Position = clipPos;
}

