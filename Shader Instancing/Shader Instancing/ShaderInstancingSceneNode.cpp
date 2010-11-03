#include "ShaderInstancingSceneNode.h"
#include <climits>

using namespace io;

namespace Graphics
{
	ShaderInstancingSceneNode::ShaderInstancingSceneNode(IMesh* mesh, scene::ISceneManager* sm,
		ISceneNode* parent,  s32 id)
		:ISceneNode(parent, sm, id), numInstances(0), duplicatedMesh(NULL)
	{
		vd = sm->getVideoDriver();
		timesBufferDuplicated.setAllocStrategy(core::ALLOC_STRATEGY_SAFE);

		//TEMP
		mat.Lighting = false;
		mat.TextureLayer[0].AnisotropicFilter = 16;
		mat.setTexture(0, vd->getTexture("cubeTexture.png"));
		mat.BackfaceCulling = false;

		IReadFile* fin = sm->getFileSystem()->createAndOpenFile("InstancingTest.fx");
		mat.MaterialType = static_cast<E_MATERIAL_TYPE>(
			vd->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(fin, "InstancingVS",
			video::EVST_VS_2_0, fin, "InstancingPS", video::EPST_PS_2_0, this));
		fin->drop();

		//Duplicate the mesh buffers and record how many times we duplicated each
		duplicateMesh(mesh);

		setAutomaticCulling(scene::EAC_OFF);
	}


	ShaderInstancingSceneNode::~ShaderInstancingSceneNode()
	{
		ISceneNodeList::Iterator it = instances.begin();
		for(; it != instances.end(); ++it)
			(*it)->drop();

		duplicatedMesh->drop();
	}

	void ShaderInstancingSceneNode::OnAnimate(u32 timeMs)
	{
		if(IsVisible)
		{
			for (ISceneNodeList::Iterator it = instances.begin(); it != instances.end(); ++it)
				(*it)->OnAnimate(timeMs);

			ISceneNode::OnAnimate(timeMs);
		}
	}

	void ShaderInstancingSceneNode::render()
	{
		//TODO: Multiple material support?
		vd->setMaterial(mat);

		u32 mbc = duplicatedMesh->getMeshBufferCount();
		u32 timesDuplicated;
		ISceneNode* instance;
		
		ISceneNodeList::Iterator it;

		//Draw the instances once for each mesh buffer
		for(u32 m = 0; m < mbc; ++m)
		{
			it = instances.begin();
			timesDuplicated = timesBufferDuplicated[m];
			//Draw all instances
			while(it != instances.end())
			{
				//Copy [timesDuplicated] (<= 60) instances into the transform
				//matrix array and do a render
				for(u32 i = 0; i < timesDuplicated; ++i)
				{
					//Even if we've set all the instances, we still need to
					//make the rest of the instances invisible
					if(it == instances.end())
					{
						//Set scale to 0 (ad hoc invisibility)
						worldTransforms[i][0] =
							worldTransforms[i][5] =
							worldTransforms[i][10] = 0.0f;
						continue;
					}
					instance = *it;
					if(instance->isVisible())
					{
						memcpy(worldTransforms[i], instance->getAbsoluteTransformation().pointer(),
							16 * sizeof(f32)); //16 floats in a 4x4 transform matrix
					}
					++it;
				}
				vd->drawMeshBuffer(duplicatedMesh->getMeshBuffer(m));
			}
		}
	}

	ISceneNode* ShaderInstancingSceneNode::addInstance(const matrix4& transform, s32 id)
	{
		return addInstance(transform.getTranslation(), transform.getRotationDegrees(),
			transform.getScale(), id);
	}

	ISceneNode* ShaderInstancingSceneNode::addInstance(const vector3df& position,
		const vector3df& rotation, const vector3df& scale, s32 id)
	{
		//TODO: Get bbox from mesh

		ISceneNode* toAdd = new InstanceSceneNode(*this, aabbox3df(), SceneManager, id);
		toAdd->setPosition(position);
		toAdd->setRotation(rotation);
		toAdd->setScale(scale);
		instances.push_back(toAdd);	
		++numInstances;
		return toAdd;
	}

	void ShaderInstancingSceneNode::removeInstance(ISceneNode *instance)
	{
		for(ISceneNodeList::Iterator it = instances.begin(); it != instances.end(); ++it)
		{
			if((*it) == instance)
			{
				(*it)->drop();
				instances.erase(it);
				--numInstances; 
				return;
			}
		}
	}

	void ShaderInstancingSceneNode::setMesh(IMesh* mesh)
	{
		//TODO: throw a fit if we get NULL

		duplicateMesh(mesh);
	}

	void ShaderInstancingSceneNode::OnSetConstants(IMaterialRendererServices* services, s32 userData)
	{
		matrix4 viewProjection = vd->getTransform(video::ETS_PROJECTION);
		viewProjection *= vd->getTransform(video::ETS_VIEW);
		services->setVertexShaderConstant("viewProjection", viewProjection.pointer(), 16);
		services->setVertexShaderConstant("worldTransforms", reinterpret_cast<f32*>(worldTransforms),
			kInstanceGroupSize * 16); //16 floats in a 4 x 4 matrix
	}

	void ShaderInstancingSceneNode::duplicateMesh(IMesh* mesh)
	{
		if(duplicatedMesh != NULL)
			duplicatedMesh->drop();

		//Duplicate the mesh buffers and record how many times we duplicated each
		duplicatedMesh = SceneManager->getMeshManipulator()->createMeshWith2TCoords(mesh);

		u32 mbCount = duplicatedMesh->getMeshBufferCount();
		timesBufferDuplicated.set_used(mbCount);

		for(u32 c = 0; c < duplicatedMesh->getMeshBufferCount(); ++c)
		{
			SMeshBufferLightMap* mb
				= reinterpret_cast<SMeshBufferLightMap*>(duplicatedMesh->getMeshBuffer(c));
			mb->setHardwareMappingHint(scene::EHM_STATIC);


			//Grab a copy of the vertices and indices we're going to use
			array<S3DVertex2TCoords> vertices = mb->Vertices;

			array<u16> indices = mb->Indices;

			//Determine the number of times we can duplicate the mesh buffer.
			//The SM2 max is 60, but it might be lower depending on the size of the buffer
			//to begin with.
			u32 vc = mb->getVertexCount();
			timesBufferDuplicated[c] = _UI16_MAX / vc;
			timesBufferDuplicated[c] = timesBufferDuplicated[c] > kInstanceGroupSize ?
				kInstanceGroupSize : timesBufferDuplicated[c];

			//Clear the mesh buffer out
			mb->Vertices.clear();
			mb->Indices.clear();

			//Duplicate it up
			u32 i;
			for(i = 0; i < timesBufferDuplicated[c]; ++i)
			{
				//Append the mesh buffer
				mb->append(vertices.pointer(), vertices.size(),
					indices.pointer(), indices.size());
			}

			//Set indices
			for(i = 0; i < mb->getVertexCount(); ++i)
			{
				mb->Vertices[i].TCoords2 = vector2df(static_cast<f32>(i / vc));
			}
		
			mb->setDirty();
		}
	}
} //end namespace Graphics