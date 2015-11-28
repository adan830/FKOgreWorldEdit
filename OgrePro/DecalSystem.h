/******************************************************************
** �ļ���:	DecalSystem.h
** ��  Ȩ:	(C)  
** ������:	FreeKnight(������)
** ��  ��:	2008-1-10 9:53:30
** ��  ��:
******************************************************************/
#pragma once
#include "IDecalSystem.h"

namespace Ogre
{
	class TerrainSceneManager;
	class MeshDecal;

	class DecalSystem : public IDecalSystem
		, public FrameListener
	{
	public:
		DecalSystem(SceneManager *mgr);
		virtual ~DecalSystem();


		// ʹ����������������������������֮��createDecal��ʹ���������������
		virtual void acceptSceneMgr(SceneManager *mgr);
		// ��ó���������
		virtual SceneManager* getSceneManager()const
		{
			return m_sceneMgr;
		}

		// ���ݲ�����ʵ��������һ�����������ʵ����Ϊ�վ��Զ�����
		virtual IDecal *createDecal(DecalType decalType
			, const String &name = StringUtil::BLANK
			, const NameValuePairList *params = 0
			);

		// ����һ������
		virtual void destroyDecal(IDecal *decal);
		virtual void destroyDecal(const String &name);

		// ������������
		virtual void destroyAllDecal();

		// ����ʵ�������һ������
		virtual IDecal *getDecal(const String &name);
		// ����Ƿ������ʵ����������
		virtual bool hasDecal(const String &name)const;

		// �����������ö��
		virtual DecalIterator getDecalIterator();

		// �����������ö��
		virtual ConstDecalIterator getDecalIterator()const;


		// ������ģ�������߼�⣨ֻ��Ե��Σ�
		ETTerrain* worldRayQuery(Vector3 &posIntersect , const Vector3 &posStart , const Vector3 &posEnd);
		ETTerrain* worldRayQuery(Vector3 &posIntersect , const Ray &ray);

		// ����������������߼�⣨�������Ρ��������塢��ҡ�����ȵȣ�
		bool rayQuery(Vector3 &posIntersect , const Vector3 &posStart , const Vector3 &posEnd);

		// ��������ԣ��������罻�����꣨���Σ�
		ETTerrain* hitTestWorld(Vector3 &posIntersect , short cursorX, short cursorY , Camera *camera);

		// ÿ֡������������
		virtual bool frameStarted(const FrameEvent& evt);
	protected:
		// ���Ψһ����������
		String getUniqueDecalName(const String &preName = StringUtil::BLANK)
		{
			static uint s_index = 0;
			return preName + "_AutoDecal_" + StringConverter::toString(++s_index);
		}
		// ���������б�
		MapDecal m_mapDecal;
		// ������γ���������
		SceneManager *m_sceneMgr;

		// ���߲�ѯ
		RaySceneQuery* m_rayQuery;

	};
};