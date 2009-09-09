!!ARBfp1.0

ATTRIB		varNormal = fragment.texcoord[0];

PARAM		color = program.local[0];
PARAM		lightVec = { 0.5773, 0.5773, 0.5773, 0.0 };

TEMP		r0;


DP3			r0.x, varNormal, varNormal;
RSQ			r0.x, r0.x;
MUL			r0, r0.x, varNormal;

DP3_SAT		r0, r0, lightVec;
MOV			r0.w, 1.0;
MUL			result.color, r0, color;

END
