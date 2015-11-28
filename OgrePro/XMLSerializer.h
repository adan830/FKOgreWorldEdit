#pragma once
#include "TinyXMLInclude.h"
#include "PropertyContainer.h"

namespace Ogre
{

	// XML���л���
	class OGREPRO_API XMLSerializer
	{
	public:
		XMLSerializer(void);
		virtual ~XMLSerializer(void);

	protected:
		// XML�ĵ�
		TiXmlDocumentPtr m_xmlDoc;

		// �½�һ��XML�ļ�
		bool newXML(const String &rootElementName);
		// ��XML�ļ�
		bool openXML(DataStreamPtr& stream);
		// ����XML�ļ�
		bool saveXML(const String& fileName);
		// �ر�XML�ļ�
		void closeXML();

		// �����ɫ���ԣ���Ӧxml�ڵ����r��g��b���ԣ�0~1��ʾ�������
		ColourValue getColorAttribute(TiXmlElement *element);
		// ���Vector3����Ӧxml�ڵ��x/y/z����
		Vector3 getVector3Attribute(TiXmlElement *element);
		// ���Quatation����Ӧxml�ڵ��x/y/z/w����
		Quaternion getQuaternionAttribute(TiXmlElement *element);
		// ����ַ�������
		String getAttribute(TiXmlElement *element , const char *name);

		// ��������
		void setAttribute(TiXmlElement *element , const String &name , const String &value);
		void setAttribute(TiXmlElement *element , const String &name , const NameValuePairList &value);

		// �½�һ���ڵ�
		TiXmlElement *newElement(TiXmlElement *parent , const String &name);
	};

};