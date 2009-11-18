!!ARBfp1.0

ATTRIB		varUV = fragment.texcoord[0];
ATTRIB		varAlpha = fragment.texcoord[1];

TEMP		r0;


TEX			r0, varUV, texture[0], 2D;
MUL			r0.w, r0.w, varAlpha.x;
MOV			result.color, r0;

END
