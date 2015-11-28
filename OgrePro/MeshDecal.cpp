/******************************************************************
** �ļ���:	DecalSystem.cpp
** ��  Ȩ:	(C)  
** ������:	FreeKnight(������)
** ��  ��:	2008-1-10 9:53:29
** ��  ��:
******************************************************************/
#include "stdafx.h"
#include "MeshDecal.h"
#include "MeshDecalClipper.h"
#include "ETTerrain.h"
#include "ETTerrainImpl.h"
#include "ETTile.h"
namespace Ogre
{
	MeshDecal::MeshDecal(SceneManager *mgr , const String &name)
		: SimpleDecal(mgr , name)
		, m_decal(0)
	{
	}

	MeshDecal::~MeshDecal()
	{
		OGRE_DELETE(m_decal);
	}

	// �����Ըı��ˣ���Ҫ�ع�������ʱ��
	void MeshDecal::updateDecal()
	{
		// ��ʾ�����ڵ�
		m_node->setVisible(m_visible);
		// �����ӾͲ�������
		if(!m_visible)
		{
			return;
		}
		// ������������
		if(m_decal == 0)
		{
			m_decal = new DecalObject;
			// �����ڵ���֮�����֮ǰ
			m_decal->setRenderQueueGroup(RENDER_QUEUE_DECAL);
			m_sceneMgr->getRootSceneNode()->attachObject(m_decal);
		}

		if(m_dirty & (DIRTY_POSITION
			| DIRTY_SIZE
			| DIRTY_DEPTH
			| DIRTY_MATERIAL_NAME
			))
		{
			// ��������������
			MeshDecalClipper clipper(m_position
				, Vector3::UNIT_Y
				, Vector3::UNIT_X
				, m_size.x
				, m_size.y
				, m_depth);

			// ��ʼ����
			clipper.begin();

			Vector3 posIntersect;
			ETTerrain *terrain = IDecalSystem::getSingleton().worldRayQuery(posIntersect , Ray(m_position + Vector3(0 , 10000 , 0) , Vector3::NEGATIVE_UNIT_Y));

			if(terrain)
			{
				TileGrid &grids = terrain->getTerrainImpl()->getTileGrid();
				for (size_t i = 0; i < grids.size(); ++i)
				{
					for (size_t j = 0; j < grids[i].size(); ++j)
					{
						Tile *tile = grids[i][j];
						clipper.clipMesh(tile->getIndexSysBufferCount() , tile->getIndexSysBuffer() , tile->getVertexSysBuffer() , tile->getNormalSysBuffer());
					}
				}
			}
			
			//TerrainRenderable *renderable;
			//TerrainSceneManager *sceneMgr = ((TerrainSceneManager*)m_sceneMgr);

			//const float tileCount = 2.0f;
			//const float startTile = - (CHUNK_LENGTH * tileCount) / 2.0f;
			//const float endTile = -startTile;

			//if(m_intersectWMOCellID == -1)
			//{
			//	//// �ҵ������ĵ��ο�
			//	for(float y = startTile ; y <= endTile ; y += CHUNK_LENGTH)
			//	{
			//		for(float x = startTile ; x <= endTile ; x += CHUNK_LENGTH)
			//		{
			//			renderable = sceneMgr->getTerrainTile(m_position + Vector3(x , 0 , y));
			//			// ֻ�е����ο��������ཻ����Ҫ���õ�����
			//			if(renderable && clipper.intersects(renderable->getWorldBoundingBox()))
			//			{
			//				// ��û�����
			//				Vector3 *pNormals = renderable->getNormalBuffer();
			//				Vector3 *pVertices = renderable->getPositionBuffer();
			//				ushort *pIndex = renderable->getIndicesBuffer();
			//				// ������������
			//				clipper.clipMesh(renderable->getIndexCount()
			//					, pIndex
			//					, pVertices
			//					, pNormals
			//					);
			//			}
			//		}
			//	}
			//}
			//else
			//{
			//	std::vector<WMOMeshDecalDataChunk> meshDataList;
			//	IInDoorSceneManager::getSingleton().getMeshDataByBoundBox(clipper.getWorldBoundingBox()
			//		, m_intersectWMOCellID
			//		, m_intersectWMOCellName
			//		, meshDataList);
			//	for(int i = 0 ; i < meshDataList.size() ; i ++)
			//	{
			//		WMOMeshDecalDataChunk &chunk = meshDataList[i];
			//		for(int j = 0 ; j < chunk.indicesList.size() ; j ++)
			//		{
			//			clipper.clipMesh(chunk.triangleCountList[j] * 3
			//				, chunk.indicesList[j]
			//				, chunk.VertexData
			//					, chunk.TriNormalList
			//					);
			//		}
			//	}
			//}
			//
			//
			// ��������
			clipper.end();
			// �������
			m_decal->fillData(&clipper);
			m_decal->setMaterial(m_materialName);
		}
	}

#define VERTEX_BINDING 0

	DecalObject::DecalObject()
		: m_radius(0.0f)
	{
		mRenderOp.vertexData = new VertexData();
		mRenderOp.indexData = new IndexData();

		mRenderOp.vertexData->vertexCount = MaxDecalVertices; 
		mRenderOp.vertexData->vertexStart = 0; 

		mRenderOp.indexData->indexCount = MaxDecalVertices * 3; 
		mRenderOp.indexData->indexStart = 0; 

		mRenderOp.operationType = RenderOperation::OT_TRIANGLE_LIST; 
		mRenderOp.useIndexes = true; 

		VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
		VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;

		size_t offset = 0;
		decl->addElement(VERTEX_BINDING, offset, VET_FLOAT3, VES_POSITION);
		offset += VertexElement::getTypeSize(VET_FLOAT3);
		decl->addElement(VERTEX_BINDING, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES , 0);
		offset += VertexElement::getTypeSize(VET_FLOAT2);
		decl->addElement(VERTEX_BINDING, offset, VET_COLOUR, VES_DIFFUSE);
		offset += VertexElement::getTypeSize(VET_COLOUR);


		// �������㻺����
		HardwareVertexBufferSharedPtr vbuf = 
			HardwareBufferManager::getSingleton().createVertexBuffer(
			decl->getVertexSize(VERTEX_BINDING)
			, mRenderOp.vertexData->vertexCount
			, HardwareBuffer::HBU_STATIC_WRITE_ONLY);

		// Bind buffer
		bind->setBinding(VERTEX_BINDING, vbuf);

		// ��������������
		mRenderOp.indexData->indexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(
			HardwareIndexBuffer::IT_16BIT
			, mRenderOp.indexData->indexCount
			, HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	}
	DecalObject::~DecalObject()
	{
		delete mRenderOp.vertexData;
		delete mRenderOp.indexData;
	}

	// ���Decal����
	void DecalObject::fillData(MeshDecalClipper *clipper)
	{
		HardwareVertexBufferSharedPtr vbuf =
			mRenderOp.vertexData->vertexBufferBinding->getBuffer(VERTEX_BINDING);     


		// �������㻺����
		MeshDecalClipper::Vertex* vertex = static_cast<MeshDecalClipper::Vertex*>(
			vbuf->lock(HardwareBuffer::HBL_DISCARD));
		size_t count = clipper->getDecalVertexCount();
		RenderSystem* rs = Root::getSingleton().getRenderSystem();
		const MeshDecalClipper::Vertex* vertexArray = clipper->getVertexArray();
		// д�붥������
		memcpy(vertex , vertexArray , sizeof(MeshDecalClipper::Vertex) * count);
		vbuf->unlock();
		// ���¼����Χ��
		mBox.setNull();
		for(size_t i = 0 ; i < count ; i ++)
		{
			mBox.merge(vertexArray[i].pos);
		}

		m_radius = (mBox.getMaximum() - mBox.getMinimum()).length() / 2.0f;

		// ��������
		mRenderOp.vertexData->vertexCount = count;

		// ��������������
		ushort* indices = static_cast<ushort*>(
			mRenderOp.indexData->indexBuffer->lock(HardwareBuffer::HBL_DISCARD));
		memcpy(indices , clipper->getIndexArray() , clipper->getDecalIndexCount() * sizeof(ushort));
		mRenderOp.indexData->indexBuffer->unlock();
		// ��������
		mRenderOp.indexData->indexCount = clipper->getDecalIndexCount();
	}

	// Override this method to prevent parent transforms (rotation,translation,scale)
	void DecalObject::getWorldTransforms( Matrix4* xform ) const
	{
		// return identity matrix to prevent parent transforms
		*xform = Matrix4::IDENTITY;
	}
	//-----------------------------------------------------------------------
	const Quaternion& DecalObject::getWorldOrientation(void) const
	{
		return Quaternion::IDENTITY;
	}
	//-----------------------------------------------------------------------
	const Vector3& DecalObject::getWorldPosition(void) const
	{
		return Vector3::ZERO;
	}
	//-----------------------------------------------------------------------
	Real DecalObject::getSquaredViewDepth(const Camera* cam) const
	{
		Vector3 min, max, mid, dist;
		min = mBox.getMinimum();
		max = mBox.getMaximum();
		mid = ((max - min) * 0.5) + min;
		dist = cam->getDerivedPosition() - mid;
		return dist.squaredLength();
	}

};