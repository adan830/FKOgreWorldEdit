/******************************************************************
** �ļ���:	DecalSystem.h
** ��  Ȩ:	(C)  
** ������:	FreeKnight(������)
** ��  ��:	2008-1-10 9:53:30
** ��  ��:	����������ʵ�֣���������λ�õķ����Զ�������������
******************************************************************/
#pragma once
#include "SimpleDecal.h"
namespace Ogre
{
	class NormalDecal : public SimpleDecal
	{
	public:
		NormalDecal(SceneManager *mgr , const String &name);
		virtual ~NormalDecal();

		// �������������
		virtual const DecalType getType()const
		{
			return DT_NORMAL;
		}

	protected:
		// �����Ըı��ˣ���Ҫ�ع�������ʱ��
		virtual void updateDecal();
		// ��������
		ManualObject *m_decal;
		// ����ƽ��
		Plane m_decalPlane;
	};

};