#version 330 core

out vec4 FragColor;

in vec2 ourTexCoord;

uniform sampler2D ourTexture;
uniform vec2 textureReps;

void main()
{
    vec2 correctedTexCoords = vec2(ourTexCoord.x * textureReps.x, ourTexCoord.y * textureReps.y);
    FragColor = texture(ourTexture, correctedTexCoords);
}