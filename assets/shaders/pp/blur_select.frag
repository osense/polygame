precision mediump float;

uniform sampler2D Render;

varying vec2 TexCoord;

void main()
{
    vec4 color = texture2D(Render, TexCoord);
    gl_FragColor = color;//step(0.5, color) * color;
}
