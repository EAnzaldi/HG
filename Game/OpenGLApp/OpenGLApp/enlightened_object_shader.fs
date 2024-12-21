#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 ourTexCoord;

uniform sampler2D ourTexture;

void main()
{
    vec4 texColor = texture(ourTexture, ourTexCoord);

    vec4 mixedColor = mix(texColor, vec4(ourColor, 1.0f), 1.0);

    FragColor = texture(ourTexture, ourTexCoord);
}