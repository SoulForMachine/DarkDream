!!ARBfp1.0

ATTRIB		varNormal = fragment.texcoord[0];

PARAM		const = { 0.0, 2.0, 0.0, 1.0 };

TEMP		r0, r1;


MOV			r0, const;
DP3			r0.x, varNormal, varNormal;
RSQ			r0.x, r0.x;
MUL			r0.x, -varNormal.z, r0.x;
ADD			r0.x, r0.x, 1.0;
SGE			r1.x, r0.x, 0.6;
MAD			r0.x, r1.x, 0.2, r0.x;
#POW			r0.x, r0.x, const.y;
#MAD			r0.x, r0.x, 0.5, 0.5;
#MUL			r0.x, r0.x, 2.0;
#ADD			r0.x, r0.x, 0.1;


MOV			result.color, r0.xxxw;

END
