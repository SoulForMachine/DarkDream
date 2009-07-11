!!ARBfp1.0

ATTRIB		texCoord = fragment.texcoord[0];
ATTRIB		color = fragment.color;

TEMP		r0;


TEX			r0, texCoord, texture[0], 2D;
MUL			result.color, r0.xxxx, color;

END
