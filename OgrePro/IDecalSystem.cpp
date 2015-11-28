/******************************************************************
** �ļ���:	IDecalSystem.cpp
** ��  Ȩ:	(C)  
** ������:	FreeKnight(������)
** ��  ��:	2008-1-10 9:53:29
** ��  ��:
******************************************************************/
#include "stdafx.h"
#include "IDecalSystem.h"
#include "DecalSystem.h"

namespace Ogre
{
	template<> IDecalSystem* Singleton<IDecalSystem>::ms_Singleton = 0;
	IDecalSystem* IDecalSystem::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	IDecalSystem& IDecalSystem::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	IDecalSystem::IDecalSystem()
	{
	}

	IDecalSystem::~IDecalSystem()
	{

	}


	static DecalSystem *g_decalSystem = 0;

	// ��ʼ������ϵͳ
	void IDecalSystem::init(SceneManager *mgr)
	{
		if(!g_decalSystem)
		{
			g_decalSystem = new DecalSystem(mgr);
		}
	}
	// ��������ϵͳ
	void IDecalSystem::shutdown()
	{
		OGRE_DELETE(g_decalSystem);
	}
};