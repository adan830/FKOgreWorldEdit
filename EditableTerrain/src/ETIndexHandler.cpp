#include "ETPrerequisites.h"
#include "ETIndexHandler.h"

#include <OgreHardwareBufferManager.h>



namespace Ogre
{
	IndexHandler::IndexHandler(ushort tileSize)
		: m_tileSize(tileSize)
		, m_lodCount(2)
		, m_indicesData(0)
		, m_indicesBuffer(0)
		, m_lodDistance(0)
	{
		m_indicesData = new IndexData*[m_lodCount];
		memset(m_indicesData , 0 , m_lodCount * sizeof(IndexData*));
		m_indicesBuffer = new ushort*[m_lodCount];
		memset(m_indicesBuffer , 0 , m_lodCount * sizeof(ushort*));
		m_lodDistance = new float[m_lodCount];
		memset(m_lodDistance , 0 , m_lodCount * sizeof(float));
		for(ushort i = 0 ; i < m_lodCount ; i ++)
		{
			m_lodDistance[i] = ((i + 1) * 33) * ((i + 1) * 33);
		}
		createIndices();
	}

	IndexHandler::~IndexHandler()
	{
		destroyIndices();
		OGRE_DELETE_ARRAY(m_lodDistance);
	}

	// ������������
	IndexData* IndexHandler::getIndexData(ushort lod)
	{
		assert(lod < m_lodCount && "Requested unexpected LOD level");
		return m_indicesData[lod];
	}

	// �����������ڴ�����
	ushort* IndexHandler::getIndexBuffer(ushort lod)
	{
		assert(lod < m_lodCount && "Requested unexpected LOD level");
		return m_indicesBuffer[lod];
	}
	// ��÷�ϴ�����������
	IndexData* IndexHandler::getSutureIndexData(ushort lod , Edge edge)
	{
		assert(lod < m_lodCount && "Requested unexpected LOD level");
		return m_mapSutureIndexData[_toSutureKey(lod , edge)];
	}


	// ������������
	void IndexHandler::destroyIndices()
	{
		for (ushort i = 0; i < m_lodCount; ++i)
		{
			OGRE_DELETE(m_indicesData[i]);
			OGRE_DELETE_ARRAY(m_indicesBuffer[i]);
		}
		OGRE_DELETE_ARRAY(m_indicesData);
		OGRE_DELETE_ARRAY(m_indicesBuffer);
		for(MapSutureIndexData::iterator iter = m_mapSutureIndexData.begin() ; iter != m_mapSutureIndexData.end() ; iter ++)
		{
			delete iter->second;
		}
		m_mapSutureIndexData.clear();

	}

	// ������������
	void IndexHandler::createIndices()
	{
		for(ushort i = 0 ; i < m_lodCount ; i ++)
		{
			// ������������
			createIndices(i);
			// ���ɷ������
			for(int j = 0 ; j < Edge_Max ; j ++)
			{
				createSutureIndices(i , (Edge)j);
			}
		}
	}
	// ������ϴ�����������
	void IndexHandler::createSutureIndices(ushort lod , Edge edge)
	{
		IndexData *&data = m_mapSutureIndexData[_toSutureKey(lod , edge)];
		// ����Ѿ������ˣ��Ͳ�������
		if(data)
			return;

		// �������㻺����
		data = new IndexData;
		if(lod == 0)
		{
			data->indexCount = (m_tileSize - 1) * 2 * 3;
		}
		else
		{
			data->indexCount = (m_tileSize - 1) / (1 << lod) * 4 * 3;
		}

		data->indexBuffer = 
			HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_16BIT
			, data->indexCount
			, HardwareBuffer::HBU_STATIC_WRITE_ONLY
			);

		// ����������
		ushort* buffer = static_cast<ushort*>(
			data->indexBuffer->lock(0, 
			data->indexBuffer->getSizeInBytes(), 
			HardwareBuffer::HBL_DISCARD));
		ushort tileIndexSplit = (1 << lod);
		// ��仺����
		switch(edge)
		{
		case Edge_Top:
			fillBuffer(buffer , data->indexCount , lod , GridVector2(0 , 0) , GridVector2(m_tileSize - 1 , 1));
			break;
		case Edge_Left:
			fillBuffer(buffer , data->indexCount , lod , GridVector2(0 , 0) , GridVector2(1 , m_tileSize - 1));
			break;
		case Edge_Bottom:
			fillBuffer(buffer , data->indexCount , lod , GridVector2(0 , m_tileSize - 1 - tileIndexSplit) , GridVector2(m_tileSize - 1 , m_tileSize - 1));
			break;
		case Edge_Right:
			fillBuffer(buffer , data->indexCount , lod , GridVector2(m_tileSize - 1 - tileIndexSplit , 0) , GridVector2(m_tileSize - 1 , m_tileSize - 1));
			break;
		}
		// ����
		data->indexBuffer->unlock();
	}
	// ����lod��������������
	ushort IndexHandler::getIndexCount(ushort lod)
	{
		if(lod == 0)
		{
			return (m_tileSize - 1) * (m_tileSize - 1) * 2 * 3;
		}
		else
		{
			return (m_tileSize - 1) / (1 << lod) * (m_tileSize - 1) / (1 << lod) * 4 * 3;
		}
	}
	// ����lod��������������������С���ֽ�Ϊ��λ
	size_t IndexHandler::getIndexBufferSize(ushort lod)
	{
		return getIndexCount(lod) * sizeof(ushort);
	}
	// ���㲢����������ݵ�buffer
	size_t IndexHandler::fillBuffer(ushort *dstBuffer , size_t dstBufferSize , ushort lod , const GridVector2 &from , const GridVector2 &to)
	{
		ushort *buffer = dstBuffer;
		// ��������������
		if(lod == 0)
		{
			ushort tileIndexSplit = (1 << lod);
			ushort tileIndexSplitPower = (1 << (lod + 1));
			for(ushort y = from.y ; y < to.y ; y += tileIndexSplit)
			{
				for(ushort x = from.x ; x < to.x ; x += tileIndexSplit)
				{
					if((y % tileIndexSplitPower == 0 && x % tileIndexSplitPower == 0)
						|| (y % tileIndexSplitPower > 0 && x % tileIndexSplitPower > 0))
					{
						*buffer++ = y * m_tileSize + x;
						*buffer++ = (y + tileIndexSplit) * m_tileSize + (x + tileIndexSplit);
						*buffer++ = (y + tileIndexSplit) * m_tileSize + x;

						*buffer++ = y * m_tileSize + x;
						*buffer++ = y * m_tileSize + (x + tileIndexSplit);
						*buffer++ = (y + tileIndexSplit) * m_tileSize + (x + tileIndexSplit);
					}
					else
					{
						*buffer++ = y * m_tileSize + x;
						*buffer++ = y * m_tileSize + (x + tileIndexSplit);
						*buffer++ = (y + tileIndexSplit) * m_tileSize + x;

						*buffer++ = y * m_tileSize + (x + tileIndexSplit);
						*buffer++ = (y + tileIndexSplit) * m_tileSize + (x + tileIndexSplit);
						*buffer++ = (y + tileIndexSplit) * m_tileSize + x;
					}
				}
			}
		}
		else
		{
			ushort tileIndexSplit = (1 << lod);
			ushort tileCenter = tileIndexSplit / 2;
			for(ushort y = from.y ; y < to.y ; y += tileIndexSplit)
			{
				for(ushort x = from.x ; x < to.x ; x += tileIndexSplit)
				{
					// ��������
					*buffer++ = y * m_tileSize + x;
					*buffer++ = y * m_tileSize + (x + tileIndexSplit);
					*buffer++ = (y + tileCenter) * m_tileSize + (x + tileCenter);

					// ��������
					*buffer++ = y * m_tileSize + x;
					*buffer++ = (y + tileCenter) * m_tileSize + (x + tileCenter);
					*buffer++ = (y + tileIndexSplit) * m_tileSize + x;

					// ��������
					*buffer++ = y * m_tileSize + (x + tileIndexSplit);
					*buffer++ = (y + tileIndexSplit) * m_tileSize + (x + tileIndexSplit);
					*buffer++ = (y + tileCenter) * m_tileSize + (x + tileCenter);

					// ��������
					*buffer++ = (y + tileIndexSplit) * m_tileSize + x;
					*buffer++ = (y + tileCenter) * m_tileSize + (x + tileCenter);
					*buffer++ = (y + tileIndexSplit) * m_tileSize + (x + tileIndexSplit);
				}
			}
		}
		assert((size_t)(buffer - dstBuffer) <= (size_t)dstBufferSize);

		return buffer - dstBuffer;
	}
	// ����һ��Lod����������
	void IndexHandler::createIndices(ushort lod)
	{
		// ����Ѿ������ˣ��Ͳ�������
		if(m_indicesBuffer[lod])
			return;

		// ����������
		m_indicesBuffer[lod] = new ushort[getIndexCount(lod)];
		ushort *buffer = m_indicesBuffer[lod];
		// ��仺����
		fillBuffer(buffer , getIndexCount(lod) , lod , GridVector2(0 , 0) , GridVector2(m_tileSize - 1 , m_tileSize - 1));
		// ���������ݴ��ڴ濽�����Դ�
		fillBufferData(lod);
	}
	// ���������ݴ��ڴ濽�����Դ�
	void IndexHandler::fillBufferData(ushort lod)
	{
		if(m_indicesData[lod])
			return;
		// �������㻺����
		m_indicesData[lod] = new IndexData;
		m_indicesData[lod]->indexBuffer = 
			HardwareBufferManager::getSingleton().createIndexBuffer(
			HardwareIndexBuffer::IT_16BIT,
			getIndexCount(lod), HardwareBuffer::HBU_STATIC_WRITE_ONLY);

		// ����������
		ushort* indexBuffer = static_cast<ushort*>(
			m_indicesData[lod]->indexBuffer->lock(0, 
			m_indicesData[lod]->indexBuffer->getSizeInBytes(), 
			HardwareBuffer::HBL_DISCARD));
		// д������
		memcpy(indexBuffer , m_indicesBuffer[lod] , getIndexBufferSize(lod));
		// ����
		m_indicesData[lod]->indexBuffer->unlock();
		// д����������
		m_indicesData[lod]->indexCount = getIndexCount(lod);
	}
}
