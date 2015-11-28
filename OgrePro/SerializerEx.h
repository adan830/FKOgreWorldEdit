#pragma once
#include "PropertyContainer.h"
namespace Ogre
{

	// ���л����ļ�ǿ�棬������һЩ�ǳ����õĺ���
	class OGREPRO_API SerializerEx : public Serializer
	{
	public:
		SerializerEx(void);
		virtual ~SerializerEx(void);
	protected:
		struct Chunk
		{
			Chunk(uint16 chunkStartID , uint16 chunkEndID , size_t posStart)
				: startID(chunkStartID)
				, endID(chunkEndID)
				, pos(posStart)
			{

			}
			// ����ʼID
			uint16 startID;
			// ������ID
			uint16 endID;
			// ����ʼλ��
			size_t pos;
		};
		typedef std::list<Chunk> ListChunk;
		ListChunk m_chunks;

		// ��ʼһ��Chunk
		void beginWriteChunk(uint16 chunkStartID , uint16 chunkEndID);
		// ����һ��Chunk
		void endWriteChunk(uint16 chunkStartID , uint16 chunkEndID);
		// ������ǰ��
		void skipCurrentChunk(DataStreamPtr &stream);

		// д��NameValuePairList����
		void writeNameValuePairList(const NameValuePairList& params);
		// ��ȡNameValuePairList����
		void readNameValuePairList(DataStreamPtr& stream , NameValuePairList &params);

		// д��StringVector����
		void writeStringVector(const StringVector& params);
		// ��ȡStringVector����
		void readStringVector(DataStreamPtr& stream , StringVector &params);


		// д��Anyֵ
		void writeAny(const Any &any , PropertyContainer::PropertyType type);
		// ��ȡAnyֵ
		Any readAny(DataStreamPtr& stream , PropertyContainer::PropertyType type);
	};

};