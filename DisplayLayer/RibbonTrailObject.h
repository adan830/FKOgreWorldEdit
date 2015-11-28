#pragma once
#include "NullObject.h"
namespace Ogre
{
	class EntityObject;

	// ��������Ķ��󹤳�
	class DISPLAYLAYER_API RibbonTrailObjectFactory : public ISceneObjectFactory
	{
	public:
		static const String FACTORY_TYPE;
		// ��øù�������������
		virtual const String &getType() const;
		// ��������Ĵ�������
		virtual ISceneObject *create(const String &name , IScene *scene , const NameValuePairList* params);
		// ������������ٷ���
		virtual void destroy(ISceneObject *obj);
	};

	// ģ��ʵ�����
	class DISPLAYLAYER_API RibbonTrailObject : public NullObject
	{
	public:
		RibbonTrailObject(const String &name);
		virtual ~RibbonTrailObject(void);
		// ��ö�������
		virtual const String &getType()const
		{
			return RibbonTrailObjectFactory::FACTORY_TYPE;
		}
		// ��������ʵ��
		virtual void create(IScene *scene , const NameValuePairList* params);
		// ���ٶ���ʵ��
		virtual void destroy();
		// ���MovableObject
		virtual MovableObject *getOgreMovableObject()const
		{
			return m_ribbonTrail;
		}
		// ���RibbonTrail
		virtual RibbonTrail *getOgreRibbonTrail()const
		{
			return m_ribbonTrail;
		}

		// ��������������������󣬷���chainIndex
		size_t trail(ISceneObject *obj);
		size_t trail(const String &objName);

		// �����������������������chainIndex
		size_t trail(EntityObject *obj , const String &boneName);
		size_t trail(const String &entityObjName , const String &boneName);

		// ֹͣ����
		void stopTrail(ISceneObject *obj);
		void stopTrail(const String &objName);
		void stopTrail(size_t chainIndex);

		// ֹͣ����
		void stopTrail(EntityObject *obj , const String &boneName);
		void stopTrail(const String &entityObjName , const String &boneName);


		// ����������ʼ��ɫ
		void setInitialColour(size_t chainIndex, const ColourValue& col);
		const ColourValue& getInitialColour(size_t chainIndex) const;

		// ����������ɫÿ���ȥ����ɫ
		void setColourChange(size_t chainIndex, const ColourValue& valuePerSecond);
		const ColourValue& getColourChange(size_t chainIndex) const;

		// ��������Ĭ�Ͽ��
		void setInitialWidth(size_t chainIndex, float width);
		float getInitialWidth(size_t chainIndex) const;

		// ��������ÿ���ȥ�Ŀ��
		void setWidthChange(size_t chainIndex, float widthDeltaPerSecond);
		float getWidthChange(size_t chainIndex) const;

		// ���������ĳ���
		void setTrailLength(float len);
		float getTrailLength(void) const;

		// ���������Ĳ���
		void setMaterialName(const String &name);
		const String &getMaterialName(void) const;

		// �����ٵĶ���
		struct TrailObject
		{
			TrailObject(ISceneObject *obj , const String &bone = StringUtil::BLANK , TagPoint *tag = 0)
				: object(obj)
				, boneName(bone)
				, boneTag(tag)
			{

			}
			union
			{
				ISceneObject *object;
				EntityObject *entity;
			};			
			// ��ͷ���ƣ������ҽӵ���ͷʱ��������
			String boneName;
			// ��ͷ�ڵ�
			TagPoint *boneTag;

			bool operator==(ISceneObject *obj)const
			{
				return object == obj && boneName.empty() && boneTag == 0;
			}
		};
		typedef std::vector<TrailObject> ListTrailObject;
		typedef ConstVectorIterator<ListTrailObject> TrailObjectIterator;
		// ��ñ����ٵĶ����б�
		TrailObjectIterator getTrailObjectIterator()const
		{
			return TrailObjectIterator(m_listTrailObject.begin() , m_listTrailObject.end());
		}

	protected:
		// Ogreʵ��
		RibbonTrail *m_ribbonTrail;

		// ���ڸ��ٵĶ���
		ListTrailObject m_listTrailObject;
	};


};