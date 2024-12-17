#version 330

uniform sampler2D texture0;
in vec2 fragTexCoord;
out vec4 finalColor;

void main() {
    vec4 texColor = texture(texture0, fragTexCoord);

    float r = texColor.r * 0.56667 + texColor.g * 0.43333;
    float g = texColor.r * 0.55833 + texColor.g * 0.44167;
    float b = texColor.b;

    finalColor = vec4(r, g, b, texColor.a);
}
