attribute vec3 inVertexPosition;
attribute vec2 inTexCoord0;

uniform mat4 WorldViewProjMat;
uniform float CamFar;
uniform vec3 Color;
uniform float Transform;

varying vec4 col;

void main()
{
    col = vec4(Color, inTexCoord0.x);

    vec4 clipPos = WorldViewProjMat * vec4(inVertexPosition, 1.0);
    clipPos.y += (1.0 - cos(clipPos.x / 3.14)) * Transform * 2.0;
    gl_Position = clipPos;
}

