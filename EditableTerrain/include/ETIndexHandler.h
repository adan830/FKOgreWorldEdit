#ifndef __ETINDEXHANDLER_H__
#define __ETINDEXHANDLER_H__

#include "ETPrerequisites.h"
#include "GridVector2.h"

namespace Ogre
{

	// ���������������
	class IndexHandler
	{
	public:
		IndexHandler(ushort tileSize);
		virtual ~IndexHandler();

		// ������������
		IndexData* getIndexData(ushort lod);

		enum Edge
		{
			Edge_Top,
			Edge_Left,
			Edge_Right,
			Edge_Bottom,

			Edge_Max
		};
		// ��÷�ϴ�����������
		IndexData* getSutureIndexData(ushort lod , Edge edge);

		// �����������ڴ�����
		ushort* getIndexBuffer(ushort lod);
		// ����lod��������������
		ushort getIndexCount(ushort lod);
		// ����lod��������������������С���ֽ�Ϊ��λ
		size_t getIndexBufferSize(ushort lod);
		// ���LOD����
		float getLodDistance(ushort lod)const
		{
			return m_lodDistance[lod];
		}
		// ���LOD����
		ushort getLodCount()const
		{
			return m_lodCount;
		}
	private:
		// ������������
		void createIndices();
		// ������������
		void createIndices(ushort lod);
		// ������ϴ�����������
		void createSutureIndices(ushort lod , Edge edge);
		// ת�����ؼ�����
		ulong _toSutureKey(ushort lod , Edge edge)
		{
			return ((ulong)lod << 16)  + (ulong)edge;
		}
		// ���㲢����������ݵ�buffer
		size_t fillBuffer(ushort *dstBuffer , size_t dstBufferSize , ushort lod , const GridVector2 &from , const GridVector2 &to);
		// ������������
		void destroyIndices();
		// ���������ݴ��ڴ濽�����Դ�
		void fillBufferData(ushort lod);

		typedef std::map<ulong , IndexData*> MapSutureIndexData;
		// ��Ӵ��Ķ������ݣ�map�е�first�ĸ�16λ��lod����16λ��edge
		MapSutureIndexData m_mapSutureIndexData;
		// �Կ�����������
		IndexData** m_indicesData;
		// �ڴ����������
		ushort **m_indicesBuffer;
		// һ��Tile�Ĵ�С
		ushort m_tileSize;
		// LOD����
		ushort m_lodCount;
		// LOD����
		float *m_lodDistance;
	};
}

#endif
