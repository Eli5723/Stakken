#version 330 core
out vec4 fragColor;

uniform vec2 iResolution;
uniform float iTime;

#define fragCoord gl_FragCoord

void main() {

	fragColor = vec4(0,0.0,0.0,1.0 );
}