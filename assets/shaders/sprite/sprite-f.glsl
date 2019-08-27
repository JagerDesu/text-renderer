#version 100
precision mediump float;

varying vec3 v_normal;
varying vec3 v_color0;
varying vec2 v_texcoord0;

uniform sampler2D s_spriteTexture;

void main() {
	vec4 texColor = texture2D(s_spriteTexture, v_texcoord0);
	vec4 r_color0 = texColor.r * vec4(v_color0, 1);
    gl_FragColor = vec4(r_color0.xyz, texColor.a);
}