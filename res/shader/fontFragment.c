/*
 * fragment.c
 *
 *  Created on: Mar 16, 2013
 *      Author: ICH
 */

#version 300 es

uniform sampler2DArray tex;

uniform sampler2D tex2;

flat in uint charDataOut;
layout(location = 0) out highp vec4 fragColor;
in highp vec2 texCoords;
uniform highp float stepSize;

uniform highp float color;

void main(void)
{
    
    highp float nStepSize = max(0.0, min(0.5, stepSize));
	
    uint iTmp = (charDataOut >> 2); //divide by 4

    highp vec4 tmp = texture(tex, vec3(texCoords, max(0.0, min(float(0x100 - 1), floor(float(iTmp) + 0.5)))));

    highp float fTmp = 0.0;
    switch ( charDataOut % 4U )
    {
            case 0U:
                    fTmp = tmp.r;
                    break;
            case 1U:
                    fTmp = tmp.g;
                    break;
            case 2U:
                    fTmp = tmp.b;
                    break;
            case 3U:
                    fTmp = tmp.a;
                    break;
    }
	
    //float tmp2 = texture(tex2, texCoords).r;

    fTmp = smoothstep(0.5 - nStepSize, 0.5 + nStepSize, fTmp);


    fragColor = vec4(1.0, 1.0, 1.0, fTmp);
}
