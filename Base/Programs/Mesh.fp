!!ARBfp1.0

INPUT		varNormal = fragment.texcoord[0];

PARAM		lightVec = { 0.0, 0.0, 1.0, 0.0 };
PARAM		const = { 0.0, 0.0, 0.0, 1.0 };

TEMP		r0;


MOV			r0, const;
DP3			r0.x, varNormal, varNormal;
RSQ			r0.x, r0.x;
MUL			r0.x, varNormal.z, r0.x;

MOV			result.color, r0.xxxw;

END
