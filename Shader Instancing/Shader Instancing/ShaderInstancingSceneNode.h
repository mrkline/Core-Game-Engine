#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace scene;
using namespace core;
using namespace video;

namespace Graphics
{
	class ShaderInstancingSceneNode : public ISceneNode, public IShaderConstantSetCallBack
	{
	public:
		ShaderInstancingSceneNode(IMesh* mesh, scene::ISceneManager* sm,
			ISceneNode* parent,  s32 id = -1);

		virtual ~ShaderInstancingSceneNode();

		virtual void OnRegisterSceneNode()
		{
			if (IsVisible)
				SceneManager->registerNodeForRendering(this);

			ISceneNode::OnRegisterSceneNode();
		}

		virtual void OnAnimate(u32 timeMs);

		virtual void render();

		virtual const aabbox3d<f32>& getBoundingBox() const { return bbox; }

		virtual u32 getMaterialCount() const { return duplicatedMesh->getMeshBufferCount(); }
		virtual SMaterial& getMaterial(u32 i)
		{ 
			return i >= getMaterialCount() ?
			ISceneNode::getMaterial(i) : duplicatedMesh->getMeshBuffer(i)->getMaterial();
		}

		virtual ISceneNode* addInstance(const matrix4& transform, s32 id = -1);
		virtual ISceneNode* addInstance(const vector3df& position = vector3df(0.f, 0.f, 0.f),
			const vector3df& rotation = vector3df(0.f, 0.f, 0.f),
			const vector3df& scale = vector3df(1.f, 1.f, 1.f), s32 id = -1);

		//removes an instance based on an index
		virtual void removeInstance(ISceneNode* instance);

		virtual const list<ISceneNode*>& getInstances() { return instances; }

		virtual u32 getNumInstances() { return numInstances; }
		
		//Be aware that this does not grab the mesh, but makes a copy of it that can 
		//be instanced
		virtual void setMesh(IMesh* mesh);

		virtual IMesh* getDuplicatedMesh() { return duplicatedMesh; }

		virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData);

	protected:
		//These scene nodes behave exactly like normal ones with the exception that
		//they don't render and aren't added to the scene graph.  They are kept
		//in a special list by the shader instancing scene node and used for transformations
		//
		class InstanceSceneNode : public ISceneNode
		{
		public:
			InstanceSceneNode(ShaderInstancingSceneNode& parentNode,
				const aabbox3df& meshBbox,
				ISceneManager* sm, s32 id = -1)
				: ISceneNode(NULL, sm, id), parent(parentNode), bbox(meshBbox) {}
			ESCENE_NODE_TYPE getType() const { return ESNT_UNKNOWN; }
			u32 getMaterialCount() const { return parent.getMaterialCount(); }
			SMaterial& getMaterial(u32 i) { return parent.getMaterial(i); }
			void OnRegisterSceneNode() {}
			void render() {}
			const aabbox3df& getBoundingBox() const { return bbox; }
		private:
			ShaderInstancingSceneNode& parent;
			const aabbox3df& bbox;
		};
		
		static const u32 kInstanceGroupSize = 60;

		IVideoDriver* vd;

		aabbox3d<f32> bbox;
		SMaterial mat;

		list<ISceneNode*> instances;
		//The number of instances is cached instead of being recounted each time
		u32 numInstances;

		IMesh* mesh;

		IMesh* duplicatedMesh;
		//The number of duplications each mesh buffer was duplicated
		array<u32> timesBufferDuplicated;
		
		f32 worldTransforms[kInstanceGroupSize][16];

		void duplicateMesh(IMesh* mesh);
	};
} //end name4space Graphics