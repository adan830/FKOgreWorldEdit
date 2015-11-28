#include "ETPrerequisites.h"
#include "ETTerrainImpl.h"
#include "ETTile.h"
#include "ETIndexHandler.h"
#include "ETTerrainInfo.h"
#include "ETTerrain.h"
#include "OgreTagPoint.h"
#include "SutureRenderable.h"

#include <OgreColourValue.h>
#include <OgreHardwareBufferManager.h>



namespace Ogre
{
	const unsigned short MAIN_BINDING = 0;
	const unsigned short DELTA_BINDING = 1;
	const unsigned int MORPH_CUSTOM_PARAM_ID = 77;

	const String Tile::MOVABLE_OBJECT_TYPE("EditableTerrainTile");

	Tile::Tile(TerrainImpl* tm,
		IndexHandler* indexHandler, TerrainInfo* info, const Options &opts, size_t startx, size_t startz)
		: MovableObject(tm->getName() + "_" + StringConverter::toString(startx) + "_" + StringConverter::toString(startz))
		, mTerrainImpl(tm)
		, mIndexHandler(indexHandler)
		, mInfo(info)
		, mOpt(opts)
		, mStartX(startx)
		, mStartZ(startz)
		, mLightListDirty(true)
		, mVertexData(0)
		, mVertexSysBuffer(0)
		, mNormalSysBuffer(0)
		, mLOD(0)
		, m_sutureRenderable(0)
	{
		m_sutureRenderable = new SutureRenderable[IndexHandler::Edge_Max];

		mTileX = startx / (mOpt.tileSize-1);
		mTileZ = startz / (mOpt.tileSize-1);

		createVertexData(startx, startz);
	}

	Tile::~Tile()
	{
		destroyCollisionShape();
		OGRE_DELETE(mVertexData);
		OGRE_DELETE_ARRAY(mNormalSysBuffer);
		OGRE_DELETE_ARRAY(mVertexSysBuffer);
	}

	const String& Tile::getMovableType() const
	{
		return MOVABLE_OBJECT_TYPE;
	}

	const MaterialPtr& Tile::getMaterial() const
	{
		return mTerrainImpl->getMaterial();
	}

	const AxisAlignedBox& Tile::getBoundingBox() const
	{
		return mBounds;
	}

	Real Tile::getBoundingRadius() const
	{
		return mBoundingRadius;
	}

	void Tile::getWorldTransforms(Matrix4* m) const
	{
		*m = mTerrainImpl->getParentSceneNode()->_getFullTransform();
	}

	const Quaternion& Tile::getWorldOrientation() const
	{
		return mTerrainImpl->getParentSceneNode()->_getDerivedOrientation();
	}

	const Vector3& Tile::getWorldPosition() const
	{
		return mTerrainImpl->getParentSceneNode()->_getDerivedPosition();
	}


	Real Tile::getSquaredViewDepth(const Camera* cam) const
	{
		Vector3 diff = mCenter - cam->getDerivedPosition();
		return diff.squaredLength() * cam->_getLodBiasInverse();
	}


	const LightList& Tile::getLights() const
	{
		if (mLightListDirty)
		{
			mTerrainImpl->_getManager()->_populateLightList(mCenter, getBoundingRadius(), mLightList);
			mLightListDirty = false;
		}
		return mLightList;
	}


	uint32 Tile::getTypeFlags() const
	{
		return SceneManager::WORLD_GEOMETRY_TYPE_MASK;
	}


	void Tile::emptyBuffer(HardwareVertexBufferSharedPtr buf)
	{
		void* pVoid = buf->lock(HardwareBuffer::HBL_DISCARD);
		memset(pVoid, 0, mOpt.tileSize*mOpt.tileSize*sizeof(float));
		buf->unlock();
	}



	void Tile::createVertexData(size_t startx, size_t startz)
	{
		mVertexData = new VertexData;
		mVertexData->vertexStart = 0;
		mVertexData->vertexCount = mOpt.tileSize * mOpt.tileSize;
	
		mVertexSysBuffer = new Vector3[mOpt.tileSize * mOpt.tileSize];
		mNormalSysBuffer = new Vector3[mOpt.tileSize * mOpt.tileSize];
		Vector3 *vertexSysBuffer = mVertexSysBuffer;

		VertexDeclaration* decl = mVertexData->vertexDeclaration;
		VertexBufferBinding* bind = mVertexData->vertexBufferBinding;

		// first we need to declare the contents of our vertex buffer
		size_t offset = 0;
		decl->addElement(MAIN_BINDING, offset, VET_FLOAT3, VES_POSITION);
		offset += VertexElement::getTypeSize(VET_FLOAT3);
		decl->addElement(MAIN_BINDING, offset, VET_FLOAT3, VES_NORMAL);
		offset += VertexElement::getTypeSize(VET_FLOAT3);
		decl->addElement(MAIN_BINDING, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
		offset += VertexElement::getTypeSize(VET_FLOAT2);

		mMainBuffer = HardwareBufferManager::getSingleton().createVertexBuffer(
			decl->getVertexSize(MAIN_BINDING), mVertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY);

		// bind the buffer
		bind->setBinding(MAIN_BINDING, mMainBuffer);


		// now construct the vertex buffer from the heightmap data
		size_t endx = startx + mOpt.tileSize;
		size_t endz = startz + mOpt.tileSize;
		mBounds.setNull();
		const VertexElement* posElem = decl->findElementBySemantic(VES_POSITION);
		const VertexElement* texElem0 = decl->findElementBySemantic(VES_TEXTURE_COORDINATES, 0);

		unsigned char* pBase = static_cast<unsigned char*>(mMainBuffer->lock(HardwareBuffer::HBL_DISCARD));

		for (size_t j = startz; j < endz; ++j)
		{
			for (size_t i = startx; i < endx; ++i)
			{
				Vector3* pPos;
				float* pTex0;//, * pTex1;
				posElem->baseVertexPointerToElement(pBase, (float**)&pPos);
				texElem0->baseVertexPointerToElement(pBase, &pTex0);

				Real height = mInfo->getOffset().y + mInfo->at(i, j) * mInfo->getScaling().y;
				pPos->x = mInfo->getOffset().x + mInfo->getScaling().x * i;
				pPos->y = height;
				pPos->z = mInfo->getOffset().z + mInfo->getScaling().z * j;

				mBounds.merge(*pPos);
				*vertexSysBuffer++ = *pPos;
				pPos++;

				*pTex0++ = float(i) / (mInfo->getWidth() - 1);
				*pTex0++ = float(j) / (mInfo->getHeight() - 1);

				pBase += mMainBuffer->getVertexSize();
			}
		}
		mMainBuffer->unlock();

		mCenter = mBounds.getCenter();

		mBoundingRadius = (mBounds.getMaximum() - mCenter).length();

		_calculateNormals(mStartX , mStartZ , mStartX + mOpt.tileSize , mStartZ + mOpt.tileSize);

	}

	// ���¼������з���
	void Tile::_calculateNormals(size_t startx, size_t startz, size_t endx, size_t endz)
	{
		// determine the area of this tile that needs to be updated
		size_t fromX = std::max(mStartX, startx);
		size_t fromZ = std::max(mStartZ, startz);
		size_t toX = std::min(endx, mStartX+mOpt.tileSize-1);
		size_t toZ = std::min(endz, mStartZ+mOpt.tileSize-1);

		const VertexElement* posElem = mVertexData->vertexDeclaration->findElementBySemantic(VES_NORMAL);
		unsigned char* pBase = static_cast<unsigned char*>(mMainBuffer->lock(HardwareBuffer::HBL_NORMAL));
		// update all necessary vertices
		for (size_t j = fromZ; j <= toZ; ++j)
		{
			for (size_t i = fromX; i <= toX; ++i)
			{
				size_t tX = i - mStartX;
				size_t tZ = j - mStartZ;
				unsigned char* pBasePos = pBase + (tZ*mOpt.tileSize + tX) * mMainBuffer->getVertexSize();
				Vector3 &pos = mVertexSysBuffer[tZ*mOpt.tileSize + tX];
				
				
				Vector3* pPos;
				posElem->baseVertexPointerToElement(pBasePos, (void**)&pPos);
				*pPos = mInfo->getNormalAt(pos.x , pos.z);

				// д���ڴ�����
				mNormalSysBuffer[tZ*mOpt.tileSize + tX] = *pPos;
			}
		}

		mMainBuffer->unlock();
	}
	// ��ñ�����ϵͳ�ڴ��е�����(������
	ushort* Tile::getIndexSysBuffer()
	{
		return mIndexHandler->getIndexBuffer(mLOD);
	}
	// ��ñ�����ϵͳ�ڴ��е�����(������
	size_t Tile::getIndexSysBufferCount()
	{
		return mIndexHandler->getIndexCount(mLOD);
	}

	// ������ײģ��
	void Tile::createCollisionShape()
	{
		destroyCollisionShape();

		size_t indexCount , indexStart;
		IndexData* data = mIndexHandler->getIndexData(0);
		indexCount = data->indexCount;
		indexStart = data->indexStart;
		AutoBufferPtr index(new AutoBuffer(data->indexBuffer.getPointer()));
		// �ӻ�����������ײģ��
		m_collisionShape = ICollisionSystem::getSingleton().createVertexCollisionShape(getName()
			, mVertexSysBuffer
			, mVertexData->vertexCount
			, false
			, ((uint16*)index->getBuffer()) + indexStart
			, indexCount
			, false
			);
	}
	// ����ray�͵���tile֮�����ײ������true������ײ��distance������ײ�������������ľ���
	bool Tile::collide(const Ray& ray , float &distance)
	{
		if(m_collisionShape.isNull())
			return false;
		// �Ż���ֱ��ѯ
		if(ray.getDirection() == Vector3::NEGATIVE_UNIT_Y || ray.getDirection() == Vector3::UNIT_Y)
		{
			float height = mInfo->getHeightAt(ray.getOrigin().x , ray.getOrigin().z);
			if(height != mInfo->getOffset().y - 1)
			{
				distance = ray.getOrigin().y - height;
			}
			return true;
		}
		else
		{
			Matrix4 mat;
			getWorldTransforms(&mat);
			RayCollisionInfo collisionInfo;
			if(!mTerrainImpl->getTerrain()->getCollisionWorld()->collide(ray , m_collisionShape , mat , collisionInfo))
				return false;
			distance = collisionInfo.distance;
			return true;
		}
	}

	// ����ray�͵���tile֮�����ײ������true������ײ��posIntersection������ײ����
	bool Tile::collide(const Ray& ray , Vector3 &posIntersection)
	{
		float distance;
		if(!collide(ray , distance))
			return false;
		posIntersection = ray.getPoint(distance);
		return true;
	}

	// ������ײģ��
	void Tile::destroyCollisionShape()
	{
		if(!m_collisionShape.isNull())
		{
			ICollisionSystem::getSingleton().destroyCollisionShape(m_collisionShape);
		}
	}

	void Tile::getRenderOperation(RenderOperation& op)
	{
		op.useIndexes = true;
		op.operationType = RenderOperation::OT_TRIANGLE_LIST;
		op.vertexData = mVertexData;
		op.indexData = mIndexHandler->getIndexData(mLOD);
	}
	// ��Ⱦ��ϱ�Ե
	void Tile::renderSutureData(RenderQueue* queue , Tile *tileNeighbour , IndexHandler::Edge edge)
	{
		if(tileNeighbour && mLOD > tileNeighbour->getLod())
		{
			IndexData* indexData = mIndexHandler->getSutureIndexData(tileNeighbour->getLod() , edge);
			if(indexData)
			{
				m_sutureRenderable[edge].setSutureData(this , indexData);
				queue->addRenderable(&m_sutureRenderable[edge], mTerrainImpl->getRenderQueueGroup());
			}
		}
	}

	void Tile::_updateRenderQueue(RenderQueue* queue)
	{
		mLightListDirty = true;
		// ��Ⱦ������
		queue->addRenderable(this, mTerrainImpl->getRenderQueueGroup());
		// ��Ⱦ��Ե��ϴ�
		TileGrid &tiles = mTerrainImpl->getTileGrid();
		
		// �ϱ߷��
		if(mTileZ > 0)
		{
			Tile *tile = tiles[mTileX][mTileZ - 1];
			renderSutureData(queue , tile , IndexHandler::Edge_Top);
		}
		// ��߷��
		if(mTileX > 0)
		{
			Tile *tile = tiles[mTileX - 1][mTileZ];
			renderSutureData(queue , tile , IndexHandler::Edge_Left);
		}
		// �ұ߷��
		if(mTileX < tiles.size() - 1)
		{
			Tile *tile = tiles[mTileX + 1][mTileZ];
			renderSutureData(queue , tile , IndexHandler::Edge_Right);
		}
		// �±߷��
		if(mTileZ < tiles[mTileX].size() - 1)
		{
			Tile *tile = tiles[mTileX][mTileZ + 1];
			renderSutureData(queue , tile , IndexHandler::Edge_Bottom);
		}
	}


	void Tile::_updateCustomGpuParameter(const GpuProgramParameters::AutoConstantEntry& constEntry,
		GpuProgramParameters* params) const
	{
		Renderable::_updateCustomGpuParameter(constEntry, params);
	}


	Vector3 Tile::getVector(size_t x, size_t z) const
	{
		float posX = mInfo->getOffset().x + (mStartX + x)*mInfo->getScaling().x;
		float posZ = mInfo->getOffset().z + (mStartZ + z)*mInfo->getScaling().z;
		return Vector3(posX, mInfo->getHeightAt(posX, posZ), posZ);
	}

	void Tile::_notifyCurrentCamera(Camera* cam)
	{
		MovableObject::_notifyCurrentCamera(cam);

		Vector3 cpos = cam -> getDerivedPosition();
		const AxisAlignedBox& aabb = getWorldBoundingBox(true);
		Vector3 diff(0, 0, 0);
		diff.makeFloor(cpos - aabb.getMinimum());
		diff.makeCeil(cpos - aabb.getMaximum());

		Real L = diff.squaredLength();

		mLOD = mIndexHandler->getLodCount() - 1;
		// ����LOD����
		for(ushort lod = 0 ; lod < mIndexHandler->getLodCount() ; lod ++)
		{
			if(L < mIndexHandler->getLodDistance(lod))
			{
				mLOD = lod;
				break;
			}
		}

	}


	void Tile::updateTerrain(size_t startx, size_t startz, size_t endx, size_t endz)
	{
		{
			//OgreProfile("updateVertex");

			// determine the area of this tile that needs to be updated
			size_t fromX = std::max(mStartX, startx);
			size_t fromZ = std::max(mStartZ, startz);
			size_t toX = std::min(endx, mStartX+mOpt.tileSize-1);
			size_t toZ = std::min(endz, mStartZ+mOpt.tileSize-1);

			const VertexElement* posElem = mVertexData->vertexDeclaration->findElementBySemantic(VES_POSITION);
			unsigned char* pBase = static_cast<unsigned char*>(mMainBuffer->lock(HardwareBuffer::HBL_NORMAL));
			Vector3 *sysBuffer = mVertexSysBuffer;
			// update all necessary vertices
			for (size_t j = fromZ; j <= toZ; ++j)
			{
				for (size_t i = fromX; i <= toX; ++i)
				{
					size_t tX = i - mStartX;
					size_t tZ = j - mStartZ;
					unsigned char* pBasePos = pBase + (tZ*mOpt.tileSize + tX) * mMainBuffer->getVertexSize();
					float h = mInfo->at(i, j);
					h *= mInfo->getScaling().y;
					h += mInfo->getOffset().y;

					float* pPos;
					posElem->baseVertexPointerToElement(pBasePos, &pPos);
					pPos[1] = h;

					// д���ڴ涥������
					sysBuffer[tZ*mOpt.tileSize + tX].y = h;
					mBounds.merge(sysBuffer[tZ*mOpt.tileSize + tX]);
				}
			}

			mMainBuffer->unlock();
			mCenter = mBounds.getCenter();
			mBoundingRadius = (mBounds.getMaximum() - mCenter).length();
		}
		{
			//OgreProfile("calcNormal");
			_calculateNormals(startx , startz , endx , endz);
		}

	}


	// ����Tile����ؽڵ�
	void Tile::attachTile(SceneNode* parent)
	{
		if(parent)
		{
			if(!isAttached())
			{
				SceneNode *node = parent->createChildSceneNode();
				//node->showBoundingBox(true);
				node->attachObject(this);
			}
		}
		else if(isAttached() && getParentSceneNode())
		{
			getParentSceneNode()->getParentSceneNode()->removeAndDestroyChild(getParentSceneNode()->getName());
		}
	}
}
