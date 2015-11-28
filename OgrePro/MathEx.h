#pragma once

namespace Ogre
{
	// ��ѧ��ǿ��
	class OGREPRO_API MathEx
	{
	public:
		// ��ŷ����ת������Ԫ����ŷ���ǵķ����û��ȱ�ʾ
		static Quaternion EularToQuaternion(const Vector3 &eular);
		// ����Ԫ��ת����ŷ���ǣ�ŷ���ǵķ����û��ȱ�ʾ
		static Vector3 QuaternionToEular(const Quaternion &orientation);

		// ���ش�һ��λ��ָ������һ��λ�õķ������Ԫ��
		static Quaternion PositionToQuaternion(const Vector3 &srcPosition , const Vector3 &dstPosition , const Vector3 &forward = Vector3::NEGATIVE_UNIT_Z);

		// �ӵ�λ����ת������Ԫ��
		static Quaternion DirectionToQuaternion(const Vector3 &direction , const Vector3 &forward = Vector3::NEGATIVE_UNIT_Z);
		// ����Ԫ��ת������λ����
		static Vector3 QuaternionToDirectionZ(const Quaternion &orientation);
		static Vector3 QuaternionToDirectionY(const Quaternion &orientation);
		static Vector3 QuaternionToDirectionX(const Quaternion &orientation);

		// ����ӳ��ٶȼ��ٵ�ĩ�ٶ������ʱ��
		static float calcTime(float startVelocity , float endVelocity , float accel)
		{
			// t = (v2 - v2) / a
			return (endVelocity - startVelocity) / accel;
		}

		// ����ӳ��ٶȼ��ٵ�ĩ�ٶ������·��
		static float calcDistanceByTime(float startVelocity , float t , float accel)
		{
			// S = v1*t + 1/2 * a * t * t
			return startVelocity * t + 0.5f * accel * t * t;
		}

		// ����ӳ��ٶȼ��ٵ�ĩ�ٶ������·��
		static float calcDistance(float startVelocity , float endVelocity , float accel)
		{
			float t = calcTime(startVelocity , endVelocity , accel);
			// S = v1*t + 1/2 * a * t * t
			return startVelocity * t + 0.5f * accel * t * t;
		}

		// ����ĩ�ٶȡ����ٶȡ����������ٶ�
		static float calcStartVelocity(float endVelocity , float accel , float distance)
		{
			return Math::Sqrt(endVelocity * endVelocity - 2.0f * accel * distance);
		}
		// ����ĩ�ٶ�
		static float calcEndVelocity(float startVelocity , float accel , float distance)
		{
			float t = (Math::Sqrt(startVelocity * startVelocity + 2.0f * accel * distance) - startVelocity) / accel;
			return startVelocity + accel * t;
		}
		// �����м�����ٶ�
		static float calcMiddleVelocity(float startVelocity , float accel , float distance)
		{
			return Math::Sqrt(accel * distance + 0.5f * startVelocity * startVelocity);
		}
		// ���㵽���м�����ٶ������·��
		static float calcMiddleDistance(float startVelocity , float accel , float distance)
		{
			float v2 = calcMiddleVelocity(startVelocity , accel , distance);
			return MathEx::calcDistance(startVelocity , v2 , accel);
		}

		// ����Բ��һ�������x��ĽǶ�
		static Radian calcCircleRadian(const Vector2 &center , const Vector2 &pointOnCircle);
		// ����Բ��һ�������x��ĽǶȣ�����Բ��Ϊ0,0
		static Radian calcCircleRadian(const Vector2 &pointOnCircle);

	};
};