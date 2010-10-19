#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace video;

namespace Graphics
{
	class DeferredMatTypeManager
	{
	public:
		enum E_DEFERRED_TYPES
		{
			EDT_UNLIT_COLOR,
			EDT_COUNT
		};

		friend class DeferredRenderer;

		DeferredMatTypeManager(E_VERTEX_SHADER_TYPE vsVersion = EVST_VS_3_0, E_PIXEL_SHADER_TYPE psVersion = EPST_PS_3_0);
		virtual ~DeferredMatTypeManager();
		E_MATERIAL_TYPE getType(E_DEFERRED_TYPES which) { return mts[which]; }

	protected:
		//Materials to be used only by the
		enum E_PROTECTED_TYPES
		{
			EPT_POSITION_CREATOR,
			EPT_COUNT
		};

		class BaseCallback : public IShaderConstantSetCallBack
		{
		public:
			virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData);
			virtual void OnSetMaterial(const SMaterial& mat);
		};

		class PositionCreatorCallback : public IShaderConstantSetCallBack
		{
		public:
			virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData);
		};

		BaseCallback* baseCB;
		PositionCreatorCallback* positionCB;

		E_MATERIAL_TYPE mts[EDT_COUNT];
		E_MATERIAL_TYPE protectedMTs[EPT_COUNT];

		E_MATERIAL_TYPE getProtectedType(E_PROTECTED_TYPES which) { return protectedMTs[which]; }
	};
} //end namespace Graphics
