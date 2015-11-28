/******************************************************************
** �ļ���:	DecalSystem.h
** ��  Ȩ:	(C)  
** ������:	FreeKnight(������)
** ��  ��:	2008-1-10 9:53:30
** ��  ��:
******************************************************************/
#pragma once
#include "SimpleDecal.h"
namespace Ogre
{
	class MeshDecalClipper;

	class DecalObject : public SimpleRenderable
	{
	public:
		DecalObject();
		virtual ~DecalObject();

		Real getSquaredViewDepth(const Camera* cam) const;
		Real getBoundingRadius(void) const { return m_radius; }

		// ���Decal����
		void fillData(MeshDecalClipper *clipper);

	protected:
		/** Override this method to prevent parent transforms (rotation,translation,scale)
		*/
		void getWorldTransforms( Matrix4* xform ) const;
		/** @copydoc Renderable::getWorldOrientation */
		const Quaternion& getWorldOrientation(void) const;
		/** @copydoc Renderable::getWorldPosition */
		const Vector3& getWorldPosition(void) const;
		Real m_radius;
	};

	// Mesh����ʵ��
	class MeshDecal : public SimpleDecal
	{
	public:
		MeshDecal(SceneManager *mgr , const String &name);
		virtual ~MeshDecal();

		// �������������
		virtual const DecalType getType()const
		{
			return DT_MESH;
		}


	protected:
		// �����Ըı��ˣ���Ҫ�ع�������ʱ��
		virtual void updateDecal();

		// ���Decal����
		void fillData(MeshDecalClipper *clipper);
		// ����ƽ��
		Plane m_decalPlane;
		// ��������
		DecalObject* m_decal;
	};

};