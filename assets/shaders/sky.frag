precision lowp float;

uniform sampler2D Tex;

varying vec2 TexCoord;

void main()
{
    gl_FragColor = vec4(vec3(TexCoord.y-0.5), 1.0);
}

