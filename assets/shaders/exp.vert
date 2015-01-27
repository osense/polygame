attribute vec3 inVertexPosition;
attribute vec3 inVertexNormal;

uniform mat4 WorldViewMat;
uniform mat4 ViewProjMat;
uniform mat4 WorldMat, ViewMat;
uniform float CamFar;
uniform vec3 CamPos;
uniform vec3 NearColor, FarColor;
uniform float Transform;

varying vec3 Color;

void main()
{
    vec4 vertex = (WorldViewMat * vec4(inVertexPosition, 1.0));
    float depth = length(vertex.xyz) / CamFar;

    float alpha = max(min(1.0, 8.0 - 10.0*depth), 0.0);
    Color = mix(NearColor, FarColor, depth) * alpha;

    vertex = WorldMat * vec4(inVertexPosition, 1.0);

    vertex.x = ((vertex.x - CamPos.x) / ((vertex.z - CamPos.z)*0.5 + 1.0)) + CamPos.x;

    depth = length((ViewMat * vertex).xyz) / CamFar;
    vertex.xyz += inVertexNormal * (depth * 0.08);

    gl_Position = ViewProjMat * vertex;
}
