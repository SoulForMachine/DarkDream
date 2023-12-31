!!ARBfp1.0

ATTRIB		varUV = fragment.texcoord[0];
ATTRIB		varNormal = fragment.texcoord[1];
ATTRIB		varLightVec = fragment.texcoord[2];

PARAM		color = program.local[0];
PARAM		shadeParams = { 1.0, 0.6, 0.2, 1.0 };

TEMP		r0, r1, normal, lightVec, texClr, emission;

# texture 0 - diffuse
# texture 1 - emission
# texture 2 - transparency


# normalize normal
DP3			r0.x, varNormal, varNormal;
RSQ			r0.x, r0.x;
MUL			normal, varNormal, r0.x;

# normalize light vector
DP3			r0.x, varLightVec, varLightVec;
RSQ			r0.x, r0.x;
MUL			lightVec, varLightVec, r0.x;

DP3			r0.x, normal, lightVec;

ADD			r0.x, r0.x, shadeParams.x;
SGE			r1.x, r0.x, shadeParams.y;
MAD			r0.x, r1.x, shadeParams.z, r0.x;
MOV			r0.w, shadeParams.w;

# diffuse texture
TEX			texClr, varUV, texture[0], 2D;

# transparency texture
TEX			r1, varUV, texture[2], 2D;
MOV			texClr.w, r1.x;

MUL			r1, texClr, color;
MUL			r0, r1, r0.xxxw;

# emission texture
TEX			emission, varUV, texture[1], 2D;

LRP			result.color, emission.xxxx, r1, r0;

END
