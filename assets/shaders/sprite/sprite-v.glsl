#version 100
precision highp float;

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec3 a_color0;
attribute vec2 a_texcoord0;

uniform mat4 u_mvp;

varying vec3 v_normal;
varying vec3 v_color0;
varying vec2 v_texcoord0;

void main() {
    vec4 position = u_mvp * vec4(a_position, 1.0);
    gl_Position = position;
    v_normal = a_normal;
    v_color0 = a_color0;
    v_texcoord0 = a_texcoord0;
}