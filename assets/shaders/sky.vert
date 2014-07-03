attribute vec3 inVertexPosition;
attribute vec2 inTexCoord0;

uniform mat4 WorldViewProjMat;

varying vec2 TexCoord;


void main()
{
    TexCoord = inTexCoord0;

    gl_Position = WorldViewProjMat * vec4(inVertexPosition, 1.0);
}

