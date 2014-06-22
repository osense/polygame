precision lowp float;

varying vec3 Color;
varying float Alpha;


void main()
{
    gl_FragColor = vec4(Color, Alpha);
}

