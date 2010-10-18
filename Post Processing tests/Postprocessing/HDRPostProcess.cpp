#include "HDRPostProcess.h"
#include "LuminanceGenerator.h"
#include "DSBloomGenerator.h"
#include "TextureAdder.h"
#include "GlobalContext.h"
#include "ResourceManager.h"
#include "Paths.h"

namespace Graphics
{
	HDRPostProcess::HDRPostProcess(const dimension2d<u32>& sourceSize, bool useHalfBuffers)
		: exposure(1.0f), lastLuminanceAdjustment(0), minLuminance(1.0f), maxLuminance(1e20f),
		lumIncreaseRate(1.0f), lumDecreaseRate(2.0f), outputLuminance(-1.0f)
	{
		if(useHalfBuffers)
		{
			lg = new LuminanceGenerator(video::ECF_R16F);
			bg = new DSBloomGenerator(sourceSize, video::ECF_G16R16F);
		}
		else
		{
			lg = new LuminanceGenerator(video::ECF_G32R32F);
			bg = new DSBloomGenerator(sourceSize, video::ECF_A32B32G32R32F);
		}

		ta = new TextureAdder(sourceSize);

		//quad.SetTexture(lg->GetOutput(), 1);

		IReadFile* fh = Resources::ResourceManager::OpenResource(Paths::PostProcesses::HDR);
		if(fh == NULL)
			throw new Exception("HDR shader file couldn't be opened", __FUNCTION__);

		mt = (E_MATERIAL_TYPE)GlobalContext::DeviceContext.GetVideoDriver()->getGPUProgrammingServices()->
			addHighLevelShaderMaterialFromFiles(nullptr,
			nullptr, video::EVST_VS_1_1,
			fh, "PSHDR", video::EPST_PS_2_0,
			this);
		if(mt < 0)
		{
			fh->drop();
			throw new Exception("HDR shader couldn't be loaded", __FUNCTION__);
		}
		
		quad.SetMaterialType(mt);

		fh->drop();
	}

	HDRPostProcess::~HDRPostProcess()
	{
		delete lg;
		delete bg;
		delete ta;
	}

	void HDRPostProcess::Render(ITexture* __restrict source, ITexture* __restrict output)
	{
		bg->Render(source);
		ta->Render(source, bg->GetOutput());
		lg->Render(ta->GetOutput());

		currLuminance = *static_cast<f32*>(lg->GetOutput()->lock(true));
		lg->GetOutput()->unlock();

		//Ramp luminance according to set values
		//If this is the first luminance, map it directly
		if(outputLuminance < 0.0f)
		{
			outputLuminance = currLuminance;
		}
		else
		{
			//Amount of seconds elapsed since the last check
			f32 dt = (static_cast<f32>(lastLuminanceAdjustment)
				- static_cast<f32>(GlobalContext::DeviceContext.GetTimer()->getTime())) / 1000.0f;

			if( currLuminance < outputLuminance + lumDecreaseRate * dt)
			{
				outputLuminance += lumDecreaseRate * dt;
			}
			else if(currLuminance > outputLuminance - lumIncreaseRate * dt)
			{
				outputLuminance -= lumIncreaseRate * dt;
			}
			else
			{
				outputLuminance = currLuminance;
			}
		}

		outputLuminance = clamp(outputLuminance, minLuminance, maxLuminance);

		lastLuminanceAdjustment = GlobalContext::DeviceContext.GetTimer()->getTime();
		quad.SetTexture((ta->GetOutput()));
		quad.Render(output);
	}

	void HDRPostProcess::OnSetConstants(IMaterialRendererServices* services, s32 userData)
	{
		services->setPixelShaderConstant("exposure", &exposure, 1);
		services->setPixelShaderConstant("avgLuminance", &outputLuminance, 1);
	}
}