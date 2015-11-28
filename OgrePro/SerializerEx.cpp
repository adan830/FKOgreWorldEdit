#include "StdAfx.h"
#include "SerializerEx.h"

namespace Ogre
{
	/// stream overhead = ID + size
	const size_t STREAM_OVERHEAD_SIZE = sizeof(uint16) + sizeof(uint32);

	SerializerEx::SerializerEx(void)
	{
	}

	SerializerEx::~SerializerEx(void)
	{
	}

	// ��ʼһ��Chunk
	void SerializerEx::beginWriteChunk(uint16 chunkStartID , uint16 chunkEndID)
	{
		// ѹ��յ��
		m_chunks.push_back(Chunk(chunkStartID , chunkEndID , ftell(mpfFile)));
		// ��ʼ��
		writeChunkHeader(chunkStartID , STREAM_OVERHEAD_SIZE);
	}

	// ����һ��Chunk
	void SerializerEx::endWriteChunk(uint16 chunkStartID , uint16 chunkEndID)
	{
		if(m_chunks.empty()
			|| m_chunks.back().startID != chunkStartID
			|| m_chunks.back().endID != chunkEndID
			)
		{
			LogManager::getSingleton().logMessage("beginWriteChunk / endWriteChunk ��ƥ�䣡���ڽ���Chunk "
				+ StringConverter::toString(chunkStartID));
			return;
		}
		Chunk &chunk = m_chunks.back();
		// ���浱ǰλ��
		size_t pos = ftell(mpfFile);
		// �ƶ���Chunk��С��λ��
		fseek(mpfFile , (long)(chunk.pos + sizeof(uint16)) , SEEK_SET);
		// д���С
		uint32 size = (uint32)(pos - chunk.pos + STREAM_OVERHEAD_SIZE);
		fwrite((void* const)&size, sizeof(size), 1, mpfFile);
		// �ƶ�����
		fseek(mpfFile , (long)pos , SEEK_SET);
		// д��end��ChunkID
		writeChunkHeader(chunkEndID , STREAM_OVERHEAD_SIZE);
		// ����յ
		m_chunks.pop_back();
	}

	// ������ǰ��
	void SerializerEx::skipCurrentChunk(DataStreamPtr &stream)
	{
		if (!stream->eof())
		{
			// һ��������ʼ�ͽ���Chunk
			if(mCurrentstreamLen)
			{
				stream->skip(mCurrentstreamLen);
			}
		}
	}

	void SerializerEx::writeNameValuePairList(const NameValuePairList& params)
	{
		uint32 size = (uint32)params.size();
		// д������
		writeInts(&size , 1);
		// �������ƺ�ֵ
		for(NameValuePairList::const_iterator iter = params.begin() ; iter != params.end() ; iter ++)
		{
			writeString(iter->first);
			writeString(iter->second);
		}
	}

	void SerializerEx::readNameValuePairList(DataStreamPtr& stream , NameValuePairList &params)
	{
		uint32 size;
		readInts(stream , &size , 1);
		String name , value;
		for(uint32 i = 0 ; i < size ; i ++)
		{
			name = readString(stream);
			value = readString(stream);
			params[name] = value;
		}
	}

	void SerializerEx::writeStringVector(const StringVector& params)
	{
		uint32 size = (uint32)params.size();
		// д������
		writeInts(&size , 1);
		// �������ƺ�ֵ
		for(StringVector::const_iterator iter = params.begin() ; iter != params.end() ; iter ++)
		{
			writeString(*iter);
		}

	}
	void SerializerEx::readStringVector(DataStreamPtr& stream , StringVector &params)
	{
		uint32 size;
		readInts(stream , &size , 1);
		String name , value;
		for(uint32 i = 0 ; i < size ; i ++)
		{
			name = readString(stream);
			params.push_back(value);
		}

	}


	// д��Anyֵ
	void SerializerEx::writeAny(const Any &any , PropertyContainer::PropertyType type)
	{
		PropertyContainer::writeAny(mpfFile , any , type);
	}
	// ��ȡAnyֵ
	Any SerializerEx::readAny(DataStreamPtr& stream , PropertyContainer::PropertyType type)
	{
		return PropertyContainer::readAny(stream , type);
	}
};