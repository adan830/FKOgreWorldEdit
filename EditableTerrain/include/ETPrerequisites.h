#ifndef __ETPREREQUISITES_H__
#define __ETPREREQUISITES_H__

#include <Ogre.h>
#include <CollisionSystem.h>
#if (OGRE_PLATFORM == OGRE_PLATFORM_WIN32 )
#   ifdef ET_MANAGER_EXPORTS
#       define ET_EXPORT __declspec(dllexport)
#   else
#       if defined( __MINGW32__ )
#           define ET_EXPORT
#       else
#    		define ET_EXPORT __declspec(dllimport)
#       endif
#   endif
#elif defined ( OGRE_GCC_VISIBILITY )
#    define ET_EXPORT  __attribute__ ((visibility("default")))
#else
#   define ET_EXPORT
#endif

// ���ͼ����
#define SPLATTING_TEXTURE_NUM 6
// һ��͸����ͼ����ɫͨ������
#define COVERAGE_CHANNEL_NUM 4
// ���ͼ��͸����ͼ������4����ɫͨ����ÿ��ͨ���洢��ÿ�������ͼ��alphaֵ���ڵ��α༭��ʱ��ᶯ̬�ı�
#define COVERAGE_TEXTURE_NUM ((SPLATTING_TEXTURE_NUM + COVERAGE_CHANNEL_NUM - 1) / COVERAGE_CHANNEL_NUM)

#endif
