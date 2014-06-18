precision lowp float;

uniform sampler2D Scene;
uniform sampler2D Depth;

//uniform float PixelSizeX;
//uniform float PixelSizeY;

varying vec2 TexCoord;

//const float weights[5] = float[5](0.00390625, 0.03125, 0.109375, 0.21875, 0.2734375);


void main()
{
    vec4 blur = vec4(0.0);
    //vec2 texcoord = vec2(gl_TexCoord[0]) + vec2(0.0, PixelSizeY/2.0);

    blur += texture2D(Scene, TexCoord + vec2(0.0, 0.020)) * 0.00390625;
    blur += texture2D(Scene, TexCoord + vec2(0.0, 0.015)) * 0.03125;
    blur += texture2D(Scene, TexCoord + vec2(0.0, 0.010)) * 0.109375;
    blur += texture2D(Scene, TexCoord + vec2(0.0, 0.005)) * 0.21875;
    blur += texture2D(Scene, TexCoord) * 0.2734375;
    blur += texture2D(Scene, TexCoord - vec2(0.0, 0.005)) * 0.21875;
    blur += texture2D(Scene, TexCoord - vec2(0.0, 0.010)) * 0.109375;
    blur += texture2D(Scene, TexCoord - vec2(0.0, 0.015)) * 0.03125;
    blur += texture2D(Scene, TexCoord - vec2(0.0, 0.020)) * 0.00390625;

    //blur *= texture2D(Depth, TexCoord).x;

    gl_FragColor = texture2D(Scene, TexCoord) + blur;
}
