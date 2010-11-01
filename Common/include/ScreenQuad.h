#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace video;

#include "GlobalContext.h"
#include "ErrorHandling.h"

namespace Graphics
{
	//class for rendering a texture to the screen (assumed to be an RTT)
	//Particular to the video driver given to it on initialization
	class ScreenQuad
	{
	public:
		ScreenQuad()
		{
			vd = GlobalContext::DeviceContext.GetVideoDriver();

			//Indices are as follows:
			/* 0---1
			// |   |
			// 3---2
			*/

			const SColor white(255, 255, 255, 255);
			vertices[0].Pos = vector3df(-1, 1, 0);
			vertices[0].TCoords = vector2df(0, 0);
			vertices[0].Color = white;
			vertices[0].TCoords2.set(vector2df(0.0f));
			vertices[1].Pos = vector3df(1, 1, 0);
			vertices[1].TCoords = vector2df(1, 0);
			vertices[1].Color = white;			
			vertices[1].TCoords2.set(vector2df(1.0f));
			vertices[2].Pos = vector3df(1, -1, 0);
			vertices[2].TCoords = vector2df(1, 1);
			vertices[2].Color = white;
			vertices[2].TCoords2.set(vector2df(2.0f));
			vertices[3].Pos = vector3df(-1, -1, 0);
			vertices[3].TCoords = vector2df(0, 1);
			vertices[3].Color = white;
			vertices[3].TCoords2.set(vector2df(3.0f));
			indices[0] = 0;
			indices[1] = 1;
			indices[2] = 3;
			indices[3] = 1;
			indices[4] = 2;
			indices[5] = 3;

			mat.BackfaceCulling = false;
			mat.Lighting = false;
			mat.ZBuffer = video::ECFN_ALWAYS;
			mat.ZWriteEnable = false;
			for(u32 c = 0; c < MATERIAL_MAX_TEXTURES; c++)
			{
				mat.TextureLayer[c].TextureWrapU = video::ETC_CLAMP;
				mat.TextureLayer[c].TextureWrapV = video::ETC_CLAMP;
			}
		}

		SMaterial& GetMaterial() { return mat; }

		//Set the texture to render with the quad
		void SetTexture(ITexture* __restrict tex, u32 layer = 0)
		{
			mat.TextureLayer[layer].Texture = tex;
		}

		ITexture* GetTexture(u32 layer = 0) { return mat.TextureLayer[layer].Texture; }

		void SetMaterialType(E_MATERIAL_TYPE mt) { mat.MaterialType = mt; }

		void Render(bool setRTToFrameBuff = true)
		{
			if(setRTToFrameBuff)
				vd->setRenderTarget(video::ERT_FRAME_BUFFER);
			vd->setMaterial(mat);
			vd->setTransform(ETS_WORLD, core::IdentityMatrix);
			vd->setTransform(ETS_VIEW, core::IdentityMatrix);
			vd->setTransform(ETS_PROJECTION, core::IdentityMatrix);
			vd->drawIndexedTriangleList(vertices, 4, indices, 2);
		}

		void Render(ITexture* rt)
		{
			vd->setRenderTarget(rt);
			vd->setMaterial(mat);
			vd->setTransform(ETS_WORLD, core::IdentityMatrix);
			vd->setTransform(ETS_VIEW, core::IdentityMatrix);
			vd->setTransform(ETS_PROJECTION, core::IdentityMatrix);
			vd->drawIndexedTriangleList(vertices, 4, indices, 2);
		}

	protected:
		//Second t coords hold the index of the coordinate
		S3DVertex2TCoords vertices[4];
		u16 indices[6];
		SMaterial mat;

		IVideoDriver* vd;
	};

} //end namespace graphics
