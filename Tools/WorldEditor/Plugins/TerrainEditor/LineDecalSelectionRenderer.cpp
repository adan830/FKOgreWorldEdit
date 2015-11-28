#include "StdAfx.h"
#include "LineDecalSelectionRenderer.h"

namespace Ogre
{

#define MAX_VERTEX_COUNT 4096
	static const int VERTEX_PREQUAD = 6;
	static const int VERTEX_BINDING = 0;
	//-----------------------------------------------------------------------
	LineDecalSelectionRenderer::LineDecalSelectionRenderer(Selection *pSelection , TerrainBrush *brush)
			: SelectionRenderer(pSelection , brush)
	{
		mRenderOp.vertexData = new VertexData();
		mRenderOp.indexData = new IndexData();

		setMaterial("LineDecal");

		// Initialization stuff
		mRenderOp.vertexData->vertexCount = MAX_VERTEX_COUNT;
		mRenderOp.vertexData->vertexStart = 0;
		mRenderOp.indexData->indexCount = mRenderOp.vertexData->vertexCount * VERTEX_PREQUAD;
		mRenderOp.indexData->indexStart = 0;
		mRenderOp.operationType = RenderOperation::OT_TRIANGLE_LIST; // OT_LINE_LIST, OT_LINE_STRIP
		mRenderOp.useIndexes = true;

		VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;
		VertexBufferBinding *bind = mRenderOp.vertexData->vertexBufferBinding;

		size_t offset = 0;
		offset += decl->addElement (VERTEX_BINDING, offset, VET_FLOAT3, VES_POSITION).getSize();
		offset += decl->addElement (VERTEX_BINDING, offset, VET_COLOUR, VES_DIFFUSE).getSize();

		HardwareVertexBufferSharedPtr vbuf =
			HardwareBufferManager::getSingleton().createVertexBuffer(
			decl->getVertexSize(VERTEX_BINDING),
			mRenderOp.vertexData->vertexCount,
			//HardwareBuffer::HBU_STATIC_WRITE_ONLY);
			HardwareBuffer::HBU_DYNAMIC);

		// add by ������ 2007-8-8
		// ��������������
		mRenderOp.indexData->indexBuffer =
			HardwareBufferManager::getSingleton().createIndexBuffer(
			HardwareIndexBuffer::IT_16BIT ,
			mRenderOp.indexData->indexCount,
			HardwareBuffer::HBU_DYNAMIC);


		bind->setBinding(VERTEX_BINDING, vbuf);

		setRenderQueueGroup(RENDER_QUEUE_3);

		m_selection->getTerrain()->getTerrainImpl()->_getManager()->getRootSceneNode()->attachObject(this);

	}

	//-----------------------------------------------------------------------
	LineDecalSelectionRenderer::~LineDecalSelectionRenderer()
	{
		m_selection->getTerrain()->getTerrainImpl()->_getManager()->getRootSceneNode()->detachObject(this);

		delete mRenderOp.vertexData;
		delete mRenderOp.indexData;
	}


	//-----------------------------------------------------------------------
	Real LineDecalSelectionRenderer::getSquaredViewDepth(const Camera *cam) const
	{
		const Vector3 vMin = mBox.getMinimum();
		const Vector3 vMax = mBox.getMaximum();
		const Vector3 vMid = ((vMin - vMax) * 0.5) + vMin;
		const Vector3 vDist = cam->getDerivedPosition() - vMid;

		return vDist.squaredLength();
	}

	//-----------------------------------------------------------------------
	Real LineDecalSelectionRenderer::getBoundingRadius(void) const
	{
		return Math::Sqrt(std::max(mBox.getMaximum().squaredLength(), mBox.getMinimum().squaredLength()));
	}

	//-----------------------------------------------------------------------
	const Quaternion &LineDecalSelectionRenderer::getWorldOrientation(void) const
	{
		return Quaternion::IDENTITY;
	}

	//-----------------------------------------------------------------------
	const Vector3 &LineDecalSelectionRenderer::getWorldPosition(void) const
	{
		return Vector3::ZERO;
	}


	struct Vertex
	{
		Vector3 pos;
		uint32 colour;
	};
	
	void LineDecalSelectionRenderer::updateVertexBuffer()
	{
		mGridPos.clear();
		// �������㻺����
		HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(VERTEX_BINDING);             
		Vertex *pPos = static_cast<Vertex*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));

		Vector3 vaabMin;
		Vector3 vaabMax;

		mRenderOp.vertexData->vertexCount = 0;

		RenderSystem *rs = Root::getSingleton().getRenderSystem();
		// ѭ������ѡ��
		for(Selection::const_iterator iter = m_selection->begin() ; iter != m_selection->end() ; iter ++)
		{
			// Խ��Ĳ�Ҫ
			if(size_t(iter->first.getX() + m_selection->getPosition().getX()) >= m_selection->getTerrain()->getTerrainInfo()->getWidth()
				|| size_t(iter->first.getY() + m_selection->getPosition().getY()) >= m_selection->getTerrain()->getTerrainInfo()->getHeight())
				continue;
			// ���涥������
			pPos->pos = m_selection->getTerrain()->getTerrainPos(iter->first.getX() + m_selection->getPosition().getX(), iter->first.getY() + m_selection->getPosition().getY());
			// ������ɫ����
			ColourValue color(1 , 0 , 0);
			color.a *= iter->second;
			rs->convertColourValue(color , &pPos->colour);
			// ��Χ�г�ʼֵ
			if(mGridPos.empty())
			{
				vaabMin = pPos->pos;
				vaabMax = pPos->pos;
			}
			mGridPos[iter->first] = (ushort)mRenderOp.vertexData->vertexCount;

			// �����Χ��
			if(pPos->pos.x < vaabMin.x)
				vaabMin.x = pPos->pos.x;
			if(pPos->pos.y < vaabMin.y)
				vaabMin.y = pPos->pos.y;
			if(pPos->pos.z < vaabMin.z)
				vaabMin.z = pPos->pos.z;

			if(pPos->pos.x > vaabMax.x)
				vaabMax.x = pPos->pos.x;
			if(pPos->pos.y > vaabMax.y)
				vaabMax.y = pPos->pos.y;
			if(pPos->pos.z > vaabMax.z)
				vaabMax.z = pPos->pos.z;

			pPos++;
			mRenderOp.vertexData->vertexCount ++;
			// �����������㻺��������
			if(mRenderOp.vertexData->vertexCount >= MAX_VERTEX_COUNT)
				break;
		}
		
		// ����������
		vbuf->unlock();
		// ���°�Χ��
		mBox.setExtents(vaabMin, vaabMax);

	}
	void LineDecalSelectionRenderer::updateIndexBuffer()
	{
		// ��������������
		ushort* pIdx = static_cast<ushort*>(mRenderOp.indexData->indexBuffer->lock(HardwareBuffer::HBL_DISCARD) );

		ushort iIndexBottomLeft = 0 , iIndexBottomRight = 0 , iIndexTopLeft = 0 , iIndexTopRight = 0;
		mRenderOp.indexData->indexCount = 0;
		MapGridVector2::iterator itPos;
		for(Selection::const_iterator iter = m_selection->begin() ; iter != m_selection->end() ; iter ++)
		{
			// Խ��Ĳ�Ҫ
			if(size_t(iter->first.getX() + m_selection->getPosition().getX()) >= m_selection->getTerrain()->getTerrainInfo()->getWidth()
				|| size_t(iter->first.getY() + m_selection->getPosition().getY()) >= m_selection->getTerrain()->getTerrainInfo()->getHeight())
				continue;
			
			// Ѱ�����ϵ�
			itPos = mGridPos.find(iter->first + GridVector2(0 , 1));
			if(itPos != mGridPos.end())
			{
				iIndexTopLeft = itPos->second;
				
				// Ѱ�����µ�
				itPos = mGridPos.find(iter->first + GridVector2(1 , 0));
				if(itPos != mGridPos.end())
				{
					iIndexBottomRight = itPos->second;
					// Ѱ�����ϵ�
					itPos = mGridPos.find(iter->first + GridVector2(1 , 1));
					if(itPos != mGridPos.end())
					{
						iIndexTopRight = itPos->second;

						// ���ö�������ֵ
						// �����������Σ��������ε�˳�����Ҫ�����λ�������˳��һ�£��������ֵ��δ��������
						// ���λ������ε�˳������ʱ���Ȼ����µ������Σ��ٻ����ϵ�������
						*pIdx++ = iIndexBottomLeft;
						*pIdx++ = iIndexTopLeft;
						*pIdx++ = iIndexBottomRight;

						*pIdx++ = iIndexTopLeft;
						*pIdx++ = iIndexTopRight;
						*pIdx++ = iIndexBottomRight;
						mRenderOp.indexData->indexCount += 6;
						// �����������㻺��������
						if(mRenderOp.indexData->indexCount >= MAX_VERTEX_COUNT * VERTEX_PREQUAD)
							break;
					}
				}

			}
			iIndexBottomLeft++;
		}
		mRenderOp.indexData->indexBuffer->unlock();
	}
};