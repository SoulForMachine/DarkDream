
#ifndef _GLSAMPLER_STATE_H_
#define _GLSAMPLER_STATE_H_

#include "GLCommon.h"


namespace GL
{

	class Renderer;


	struct SamplerStateDesc
	{
		TexAddressMode addressU;
		TexAddressMode addressV;
		TexAddressMode addressW;
		TexFilter minFilter;
		TexFilter magFilter;
		float borderColor[4];
		float minLOD;
		float maxLOD;
		int baseLevel;
		int maxLevel;
		float lodBias;
		DepthTexMode depthTexMode; // depricated
		TexCompareMode texCmpMode;
		CompareFunc texCmpFunc;
		float maxAnisotropy;
	};


	class SamplerState
	{
	public:
		SamplerState() {}
		~SamplerState() {}

	private:
		SamplerStateDesc _state;

		friend class Renderer;
	};

}


#endif // _GLSAMPLER_STATE_H_
