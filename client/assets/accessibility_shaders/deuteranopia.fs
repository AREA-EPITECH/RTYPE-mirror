#version 330

uniform sampler2D texture0;
in vec2 fragTexCoord;
out vec4 finalColor;

void main() {
    vec4 texColor = texture(texture0, fragTexCoord);

    float r = texColor.r * 0.8 + texColor.g * 0.2;
    float g = texColor.r * 0.2 + texColor.g * 0.8;
    float b = texColor.b;

    finalColor = vec4(r, g, b, texColor.a);
}
