#include "StdAfx.h"
#include "MathEx.h"
namespace Ogre
{
	// ��ŷ����ת������Ԫ��
	Quaternion MathEx::EularToQuaternion(const Vector3 &eular)
	{
		Matrix3 mat;
		mat.FromEulerAnglesXYZ(Radian(eular.x) , Radian(eular.y) , Radian(eular.z));
		return Quaternion(mat);
	}
	// ����Ԫ��ת����ŷ����
	Vector3 MathEx::QuaternionToEular(const Quaternion &orientation)
	{
		Matrix3 mat;
		orientation.ToRotationMatrix(mat);
		Radian x , y , z;
		mat.ToEulerAnglesXYZ(x , y , z);
		return Vector3(x.valueRadians() , y.valueRadians() , z.valueRadians());
	}
	// ���ش�һ��λ��ָ������һ��λ�õķ������Ԫ��
	Quaternion MathEx::PositionToQuaternion(const Vector3 &srcPosition , const Vector3 &dstPosition , const Vector3 &forward)
	{
		return DirectionToQuaternion((dstPosition - srcPosition).normalisedCopy() , forward);
	}

	// �ӵ�λ����ת������Ԫ��
	Quaternion MathEx::DirectionToQuaternion(const Vector3 &direction , const Vector3 &forward)
	{
		return forward.getRotationTo(direction);
	}
	// ����Ԫ��ת������λ����
	Vector3 MathEx::QuaternionToDirectionZ(const Quaternion &orientation)
	{
		return orientation.zAxis();
	}
	Vector3 MathEx::QuaternionToDirectionY(const Quaternion &orientation)
	{
		return orientation.yAxis();
	}
	Vector3 MathEx::QuaternionToDirectionX(const Quaternion &orientation)
	{
		return orientation.xAxis();
	}

	// ����Բ��һ�������x��ĽǶ�
	Radian MathEx::calcCircleRadian(const Vector2 &center , const Vector2 &pointOnCircle)
	{
		return calcCircleRadian(pointOnCircle - center);
	}
	// ����Բ��һ�������x��ĽǶȣ�����Բ��Ϊ0,0
	Radian MathEx::calcCircleRadian(const Vector2 &pointOnCircle)
	{
		float k = 0.0f;
		// �ڵ�1/2����
		if(pointOnCircle.x >= 0)
		{
			k = 0.0f;
		}
		else
		{
			k = 1.0f;
		}
		return Radian(k * Math::PI + atan(pointOnCircle.y / pointOnCircle.x));
	}


};