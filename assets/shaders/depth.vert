attribute vec3 inVertexPosition;
attribute vec3 inVertexNormal;

uniform mat4 WorldViewProjMat;
uniform mat4 WorldViewMat;
uniform float CamFar;

varying float Depth;

void main()
{
    vec3 Vertex = (WorldViewMat * vec4(inVertexPosition, 1.0)).xyz;
    Depth = Vertex.z / CamFar;

    gl_Position = WorldViewProjMat * vec4(inVertexPosition, 1.0);
}

