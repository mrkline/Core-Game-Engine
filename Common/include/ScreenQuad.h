#pragma once

#include <irrlicht.h>

#include <GlobalContext.h>

namespace Core
{
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

				if(vd == nullptr)
				{
					throw new Error::ArgumentNullException("The global device context has not been set yet", __FUNCTION__);
				}

				//Indices are as follows:
				/* 0---1
				// |   |
				// 3---2
				*/

				const irr::video::SColor white(255, 255, 255, 255);
				vertices[0].Pos = irr::core::vector3df(-1, 1, 0);
				vertices[0].TCoords = irr::core::vector2df(0, 0);
				vertices[0].Color = white;
				vertices[0].TCoords2.set(irr::core::vector2df(0.0f));
				vertices[1].Pos = irr::core::vector3df(1, 1, 0);
				vertices[1].TCoords = irr::core::vector2df(1, 0);
				vertices[1].Color = white;			
				vertices[1].TCoords2.set(irr::core::vector2df(1.0f));
				vertices[2].Pos = irr::core::vector3df(1, -1, 0);
				vertices[2].TCoords = irr::core::vector2df(1, 1);
				vertices[2].Color = white;
				vertices[2].TCoords2.set(irr::core::vector2df(2.0f));
				vertices[3].Pos =irr::core::vector3df(-1, -1, 0);
				vertices[3].TCoords = irr::core::vector2df(0, 1);
				vertices[3].Color = white;
				vertices[3].TCoords2.set(irr::core::vector2df(3.0f));
				indices[0] = 0;
				indices[1] = 1;
				indices[2] = 3;
				indices[3] = 1;
				indices[4] = 2;
				indices[5] = 3;

				mat.BackfaceCulling = false;
				mat.Lighting = false;
				mat.ZBuffer = irr::video::ECFN_ALWAYS;
				mat.ZWriteEnable = false;
				for(u32 c = 0; c < irr::video::MATERIAL_MAX_TEXTURES; c++)
				{
					mat.TextureLayer[c].TextureWrapU = irr::video::ETC_CLAMP;
					mat.TextureLayer[c].TextureWrapV = irr::video::ETC_CLAMP;
				}
			}

			irr::video::SMaterial& GetMaterial() { return mat; }

			//Set the texture to render with the quad
			void SetTexture(irr::video::ITexture* __restrict tex, u32 layer = 0)
			{
				mat.TextureLayer[layer].Texture = tex;
			}

			irr::video::ITexture* GetTexture(u32 layer = 0) { return mat.TextureLayer[layer].Texture; }

			void SetMaterialType(irr:: video::E_MATERIAL_TYPE mt) { mat.MaterialType = mt; }

			void Render(bool setRTToFrameBuff = true)
			{
				if(setRTToFrameBuff)
					vd->setRenderTarget(irr::video::ERT_FRAME_BUFFER);
				vd->setMaterial(mat);
				vd->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
				vd->setTransform(irr::video::ETS_VIEW, irr::core::IdentityMatrix);
				vd->setTransform(irr::video::ETS_PROJECTION, irr::core::IdentityMatrix);
				vd->drawIndexedTriangleList(vertices, 4, indices, 2);
			}

			void Render(irr::video::ITexture* rt)
			{
				vd->setRenderTarget(rt);
				vd->setMaterial(mat);
				vd->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
				vd->setTransform(irr::video::ETS_VIEW, irr::core::IdentityMatrix);
				vd->setTransform(irr::video::ETS_PROJECTION, irr::core::IdentityMatrix);
				vd->drawIndexedTriangleList(vertices, 4, indices, 2);
			}

		protected:
			//Second t coords hold the index of the coordinate
			irr::video::S3DVertex2TCoords vertices[4];
			irr::u16 indices[6];
			irr::video::SMaterial mat;

			irr::video::IVideoDriver* vd;
		};

	} //end namespace graphics
} //end namespace Core