#pragma once

namespace Ogre
{
	// ��������
	class OGREPRO_API PropertyContainer
	{
	public:
		PropertyContainer(){};
		virtual ~PropertyContainer(){};
		// ��������
		enum PropertyType
		{
			PT_GROUP,	// ���飬��������������
			PT_BOOL,
			PT_REAL,
			PT_INT,
			PT_UINT,
			PT_SHORT,
			PT_USHORT,
			PT_LONG,
			PT_ULONG,
			PT_STRING,
			PT_VECTOR2,
			PT_VECTOR3,
			PT_MATRIX3,
			PT_MATRIX4,
			PT_QUATERNION,
			PT_COLOURVALUE,
			PT_AXISALIGNEDBOX,
			PT_UNKNOWN = 0xFFFF,
		};
		// ����
		class OGREPRO_API Property
		{
		public:
			Property();
			Property(uint inID , PropertyType inType , const String &inName , const String &inDesc , bool inShowable , bool inEditable , bool inSaveable);
			// ����ID
			uint id;
			// �ڽ�������ʾ������
			String name;
			// ��������
			PropertyType type;
			// ����˵��
			String desc;
			// �Ƿ�������ʾ���û�
			bool showable;
			// �Ƿ������û��޸�
			bool editable;
			// �Ƿ�������
			bool saveable;
			// ������
			typedef std::vector<Property> VTProperty;
			VTProperty childs;
			// ������
			Property *parent;

			inline void addChild(Property &property)
			{
				property.parent = this;
				childs.push_back(property);
			}
		};
		typedef std::map<uint , Property> MapProperty;
		typedef std::vector<Property> VTProperty;

		struct PropertyVisitor
		{
			virtual bool visit(const Property &property) = 0;
		};
		// ����������ԣ���������ʽ
		virtual const VTProperty* getPropertiesTree()const = 0;
		// ����������ԣ����б����ʽ
		virtual const MapProperty* getPropertiesList()const = 0;

		// �ѵ�ǰ���Լ����������Կ�����target��
		void cloneProperties(PropertyContainer *target);

		// ��������
		virtual bool setProperty(uint id , const Any &value) = 0;
		// ��ȡ����ֵ
		virtual Any getProperty(uint id)const = 0;
		// ��ȡ����ֵ
		Any getProperty(const String &name)const;

		// ��������
		bool setPropertyByName(const String &name , const String &value);
		// ��ȡ����ֵ
		String getPropertyByName(const String &name)const;
		// ��������
		bool setPropertyByName(const String &name , const Any &value);


		// ��������
		bool setPropertyByID(uint id , const String &value);
		// ��ȡ����ֵ
		String getPropertyByID(uint id)const;

		// �����������ƻ�ȡ����ID������0���Ҳ���
		uint getPropertyID(const String &name);
		// ��������ID��ȡ��������
		String getPropertyName(uint id);

		// ����������������
		void visitProperties(PropertyVisitor *visitor);
		void visitProperties(PropertyVisitor *visitor)const;
		// ��������һ����������
		static void visitProperties(PropertyVisitor *visitor , const VTProperty &properties);

		// ��Anyֵת����String����
		static String toString(const Any &any , PropertyType type);
		// ��String���͸�������ת����Any
		static Any toAny(const String &value , PropertyType type);

		// д��Anyֵ���ļ�file
		static void writeAny(FILE* file , const Any &any , PropertyType type);
		// ���ļ�file��ȡһ��Anyֵ
		static Any readAny(FILE* file , PropertyType type);
		// ��������stream��ȡһ��Anyֵ
		static Any readAny(DataStreamPtr &stream , PropertyType type);
	protected:
		// ö���������ԣ���������ʽ
		virtual void enumPropertiesTree(VTProperty &properties)const = 0;
		// ö���������ԣ����б����ʽ
		void enumPropertiesList(MapProperty &properties)const;
	};
}