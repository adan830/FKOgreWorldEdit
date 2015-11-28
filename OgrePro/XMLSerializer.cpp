#include "StdAfx.h"
#include "XMLSerializer.h"
#include "StringConverterEx.h"

namespace Ogre
{
	XMLSerializer::XMLSerializer(void)
	{
	}

	XMLSerializer::~XMLSerializer(void)
	{
		closeXML();
	}
	// �½�һ��XML�ļ�
	bool XMLSerializer::newXML(const String &rootElementName)
	{
		closeXML();
		m_xmlDoc.bind(new TiXmlDocument());
		TiXmlElement element(rootElementName.c_str());
		m_xmlDoc->InsertEndChild(element);
		return true;
	}

	// ��XML�ļ�
	bool XMLSerializer::openXML(DataStreamPtr& stream)
	{
		closeXML();
		m_xmlDoc.bind(new TiXmlDocument());
		// ����XML�ļ�
		size_t iSize = stream->size();
		char *pBuf = new char[iSize+1];
		memset(pBuf, 0, iSize+1);
		stream->read(pBuf, iSize);
		stream.setNull();
		m_xmlDoc->Parse(pBuf);
		OGRE_DELETE_ARRAY(pBuf);

		// ���XML�ļ��Ƿ��ȡ�ɹ�
		if(m_xmlDoc->Error())
		{
			m_xmlDoc.setNull();

			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"��XML�ļ�ʧ��",
				"XMLSerializer::openXML");
			return false;
		}
		return true;
	}
	// ����XML�ļ�
	bool XMLSerializer::saveXML(const String& fileName)
	{
		if(m_xmlDoc.isNull())
			return false;
		return m_xmlDoc->SaveFile(fileName.c_str());
	}
	// �ر�XML�ļ�
	void XMLSerializer::closeXML()
	{
		m_xmlDoc.setNull();
	}

	// �����ɫ���ԣ���Ӧxml�ڵ����r��g��b���ԣ�0~1��ʾ�������
	ColourValue XMLSerializer::getColorAttribute(TiXmlElement *element)
	{
		return ColourValue(StringConverter::parseReal(getAttribute(element , "r"))
			, StringConverter::parseReal(getAttribute(element , "g"))
			, StringConverter::parseReal(getAttribute(element , "b"))
			, element->Attribute("a") ? StringConverter::parseReal(getAttribute(element , "a")) : 1.0f
			);
	}
	// ���Vector3����Ӧxml�ڵ��x/y/z����
	Vector3 XMLSerializer::getVector3Attribute(TiXmlElement *element)
	{
		return Vector3(StringConverter::parseReal(getAttribute(element , "x"))
			, StringConverter::parseReal(getAttribute(element , "y"))
			, StringConverter::parseReal(getAttribute(element , "z"))
			);
	}
	// ���Quatation����Ӧxml�ڵ��x/y/z/w����
	Quaternion XMLSerializer::getQuaternionAttribute(TiXmlElement *element)
	{
		return Quaternion(StringConverter::parseReal(getAttribute(element , "w"))
			, StringConverter::parseReal(getAttribute(element , "x"))
			, StringConverter::parseReal(getAttribute(element , "y"))
			, StringConverter::parseReal(getAttribute(element , "z"))
			);
	}
	// ����ַ�������
	String XMLSerializer::getAttribute(TiXmlElement *element , const char *name)
	{
		if(!element)
			return StringUtil::BLANK;
		const char *value = element->Attribute(name);
		if(!value)
			return StringUtil::BLANK;
		else
			return value;
	}



	// �½�һ���ڵ�
	TiXmlElement *XMLSerializer::newElement(TiXmlElement *parent , const String &name)
	{
		TiXmlElement element(name.c_str());
		return (TiXmlElement*)parent->InsertEndChild(element);
	}

	// ��������
	void XMLSerializer::setAttribute(TiXmlElement *element , const String &name , const String &value)
	{
		element->SetAttribute(name.c_str() , value.c_str());
	}

	void XMLSerializer::setAttribute(TiXmlElement *element , const String &name , const NameValuePairList &value)
	{
		setAttribute(element , name , StringConverterEx::toString(value));
	}

};