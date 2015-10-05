/*
 * vertx.c
 *
 *  Created on: Mar 16, 2013
 *      Author: ICH
 */

#version 300 es

layout(location = 0) in vec4 pos;
layout(location = 1) in uint charDataIn;
uniform mat4 charMat;
flat out uint charDataOut;
out vec2 texCoords;

uniform float color;

void main(void)
{
	gl_Position = charMat * vec4(pos.xy, 0.0, 1.0);
	charDataOut = charDataIn;
	texCoords = pos.zw;
}
