#version 330

uniform sampler2D texture0;
in vec2 fragTexCoord;
out vec4 finalColor;

void main() {
    vec4 texColor = texture(texture0, fragTexCoord);

    float r = texColor.r;
    float g = texColor.r * 0.95 + texColor.g * 0.05;
    float b = texColor.g * 0.43333 + texColor.b * 0.56667;

    finalColor = vec4(r, g, b, texColor.a);
}
