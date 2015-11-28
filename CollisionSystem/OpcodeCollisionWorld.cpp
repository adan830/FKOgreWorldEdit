#include "StdAfx.h"
#include "OpcodeCollisionWorld.h"
#include "OpcodeCollisionEntity.h"
#include "OpcodeCollisionShape.h"

#include <opcode.h>

using namespace Opcode;
using namespace IceMaths;

namespace Ogre
{

	OpcodeCollisionWorld::OpcodeCollisionWorld(const String &name)
		: m_name(name)
	{

		mCollider = new AABBTreeCollider();
		mCollider->SetFirstContact(false);
		mCollider->SetFullBoxBoxTest (true);
		mCollider->SetFullPrimBoxTest (true);
		mCollider->SetTemporalCoherence (false);

		mRayColfaces = new CollisionFaces;

		mRayCollider = new RayCollider();
		mRayCollider->SetFirstContact(false);
		mRayCollider->SetTemporalCoherence(false);
		mRayCollider->SetClosestHit(true);
		mRayCollider->SetCulling(false);
		// add by ������ 2007-7-17
		// Ĭ�ϵľ���̫С�ˡ��������ǲ����������
		mRayCollider->SetMaxDist(/*300.0f*/);
		mRayCollider->SetDestination(mRayColfaces);

		mColCache = new BVTCache();
	}

	OpcodeCollisionWorld::~OpcodeCollisionWorld(void)
	{
		delete mRayColfaces;
		delete mRayCollider;
		delete mColCache;
		delete mCollider;
	}


	// ��һ��Entity������ײʵ��
	ICollisionEntity* OpcodeCollisionWorld::createEntityCollisionEntity(Entity *entity , CollisionShapeType type , const String &shapeName)
	{
		OpcodeCollisionEntity *collisionEntity = new OpcodeCollisionEntity(this);
		if(!collisionEntity->init(entity , type , shapeName))
		{
			delete collisionEntity;
			return 0;
		}
		return collisionEntity;
	}
	// ������ײʵ��
	void OpcodeCollisionWorld::destroyCollisionEntity(ICollisionEntity *entity)
	{
		delete entity;
	}


	// ��Ogre����ת����OpCode����
	IceMaths::Matrix4x4* OpcodeCollisionWorld::getIceMathsMatrix(const Matrix4& matrix) const
	{
		IceMaths::Matrix4x4* iceMathsMatrix = new IceMaths::Matrix4x4();

		for(unsigned int i = 0; i < 4; i++)
		{
			iceMathsMatrix->m[0][i] = matrix[i][0];
			iceMathsMatrix->m[1][i] = matrix[i][1];
			iceMathsMatrix->m[2][i] = matrix[i][2];
			iceMathsMatrix->m[3][i] = matrix[i][3];
		}	
		return iceMathsMatrix;
	}

	// ����ray��col֮�����ײ��������ײ��Ϣ������true������ײ
	bool OpcodeCollisionWorld::collide(const Ray& ray
		, const ICollisionShapePtr& col
		, const Matrix4& worldTrans
		, RayCollisionInfo& collisionInfo
		)
	{
		//cast into opcode collision shape
		const OpcodeCollisionShape* shape = static_cast<const OpcodeCollisionShape*>(col.getPointer());
		Opcode::Model* testModel = shape->getCollisionModel();

		IceMaths::Matrix4x4* worldMatrix = getIceMathsMatrix(worldTrans);

		Vector3 rayOrigin;
		Vector3 rayDirection;
		IceMaths::Ray worldRay;

		worldRay.mOrig.x = ray.getOrigin().x;
		worldRay.mOrig.y = ray.getOrigin().y;
		worldRay.mOrig.z = ray.getOrigin().z;
		worldRay.mDir.x = ray.getDirection().x;
		worldRay.mDir.y = ray.getDirection().y;
		worldRay.mDir.z = ray.getDirection().z; 

		bool isOK = mRayCollider->Collide(worldRay, *testModel, worldMatrix);

		delete worldMatrix;

		if(!isOK)
			return false;

		// The return type of collider is BOOL which is typedef'd to an int
		BOOL status = mRayCollider->GetContactStatus();
		if(!status)
			return false;

		const CollisionFace* collArray = mRayColfaces->GetFaces();
		int numColls = mRayColfaces->GetNbFaces();

		collisionInfo.distance = FLT_MAX;
		//currently we're just taking the last intersection if there is one, and not finding the closest one
		for (int i = 0; i < numColls; i++)
		{
			if(collArray[i].mDistance < collisionInfo.distance)
			{
				collisionInfo.distance = collArray[i].mDistance;
			}
		}

		return collisionInfo.distance != FLT_MAX;
	}
};