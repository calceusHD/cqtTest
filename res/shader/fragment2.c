#version 300 es

in highp vec2 uv;

layout(location = 0) out highp vec4 fragColor;

uniform sampler2D data;

void main(void)
{
    highp float c = texture(data, uv.yx);
    fragColor = vec4(1.0, 0.0, 1.0, c);
}
