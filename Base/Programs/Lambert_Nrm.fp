!!ARBfp1.0

ATTRIB		varUV = fragment.texcoord[0];
ATTRIB		varLightVec = fragment.texcoord[1];

PARAM		color = program.local[0];
PARAM		lightVec = { 0.0, 1.0, 0.0, 0.0 };		#{ 0.5773, 0.5773, 0.5773, 0.0 };
PARAM		scaleTwo = { 2.0, 2.0, 2.0, 0.0 };
PARAM		subOne = { -1.0, -1.0, -1.0, 0.0 };

TEMP		r0, normal;


# get normal from normal map
TEX			normal, varUV, texture[3], 2D;
MAD			normal, normal, scaleTwo, subOne;

# normalize normal
DP3			r0.x, normal, normal;
RSQ			r0.x, r0.x;
MUL			normal, normal, r0.x;

DP3_SAT		r0, normal, lightVec;
MOV			r0.w, 1.0;
MUL			result.color, r0, color;

END
