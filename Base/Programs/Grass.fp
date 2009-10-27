!!ARBfp1.0

ATTRIB		varUV = fragment.texcoord[0];

PARAM		threshold = { 0.2, 0.0, 0.0, 0.0 };

TEMP		r0, r1;

TEX			r0, varUV, texture[0], 2D;
SUB			r1, r0.wwww, threshold;
KIL			r1;
MOV			result.color, r0;

END
