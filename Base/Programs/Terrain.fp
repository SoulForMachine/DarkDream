!!ARBfp1.0

ATTRIB		varUV = fragment.texcoord[0];
ATTRIB		varNormal = fragment.texcoord[1];
ATTRIB		varLightVec = fragment.texcoord[2];

PARAM		color = program.local[0];
PARAM		shadeParams = { 0.5, 0.4, 0.2, 1.0 };
PARAM		grayscale = { 0.3, 0.59, 0.11, 0.0 }; 

TEMP		r0, normal, lightVec, diff, ramp;


# normalize normal
DP3			r0.x, varNormal, varNormal;
RSQ			r0.x, r0.x;
MUL			normal, varNormal, r0.x;

# normalize light vector
DP3			r0.x, varLightVec, varLightVec;
RSQ			r0.x, r0.x;
MUL			lightVec, varLightVec, r0.x;
#MOV			lightVec, { 0.0, 0.0, -1.0, 0.0 };

DP3			r0.x, normal, lightVec;
ADD_SAT		r0.x, r0.x, 0.8;

MOV			r0.y, 0.0;
TEX			ramp, r0, texture[1], 2D;
#MUL			ramp.y, ramp.y, varNormal.w;
#MUL			ramp.z, ramp.z, varNormal.w;

TEX			diff, varUV, texture[0], 2D;
DP3			diff, diff, grayscale;
DP4			r0, ramp, diff;
MOV			r0.w, shadeParams.w;

MUL			result.color, r0, color;

END
