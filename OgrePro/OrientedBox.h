#pragma once

namespace Ogre
{
	// �����Χ�е�ʵ��
	class OGREPRO_API OrientedBox
	{
	public:

		OrientedBox()
			: m_center()
			, m_extents(0.5,0.5,0.5)
			, m_rot(Matrix3::IDENTITY)
		{
		}

		OrientedBox( const AxisAlignedBox& box )
			: m_rot(Matrix3::IDENTITY)
		{ 
			m_extents = (box.getMaximum() - box.getMinimum())*0.5;
			m_center = box.getMinimum() + m_extents;
		}

		OrientedBox( const AxisAlignedBox& box , const Matrix3& axes)
			: m_rot(axes)
		{ 
			m_extents = (box.getMaximum() - box.getMinimum())*0.5;
			m_center = box.getMinimum() + m_extents;
		}

		// ��������
		OrientedBox(const OrientedBox& obb )
			: m_center(obb.m_center)
			, m_extents(obb.m_extents)
			, m_rot(obb.m_rot)
		{
		}

		// ��ȫ����
		OrientedBox(const Vector3& c, const Vector3& ex, const Matrix3& mat  )
			: m_center(c)
			, m_extents(ex)
			, m_rot(mat)
		{
		}

		// ��ȫ����
		OrientedBox(const Vector3& c, const Vector3& ex, const Quaternion& quat  )
			: m_center(c)
			, m_extents(ex)
		{
			quat.ToRotationMatrix(m_rot);
		}

		// ���ð�Χ�к���ת
		void setByAABB(const AxisAlignedBox& box , const Matrix3& axes)
		{
			m_extents = (box.getMaximum() - box.getMinimum())*0.5;
			m_center = box.getMinimum() + m_extents;
			m_rot = axes;
		}
		// ���ð�Χ�к���ת
		void setByAABB(const AxisAlignedBox& box , const Quaternion& quat)
		{
			m_extents = (box.getMaximum() - box.getMinimum())*0.5;
			m_center = box.getMinimum() + m_extents;
			quat.ToRotationMatrix(m_rot);
		}
		// �������ĵ�
		void setCenter(const Vector3 &pos)
		{
			m_center = pos;
		}
		// ���ô�С
		void setExtents(const Vector3 &ext)
		{
			m_extents = ext;
		}
		// ���÷���
		void setOrientation(const Matrix3 &mat)
		{
			m_rot = mat;
		}
		// ���÷���
		void setOrientation(const Quaternion &quat)
		{
			quat.ToRotationMatrix(m_rot);
		}

		// ��ô�С
		const Vector3& getExtents() const
		{
			return m_extents;
		}

		// ������ĵ�
		const Vector3& getCenter() const
		{
			return m_center;
		}

		// ��÷���
		const Matrix3& getOrientation() const
		{
			return m_rot;
		}

		// �������
		inline Real volume() const
		{
			return m_extents.x *m_extents.y *m_extents.z * 8.0;
		}

		// ���ظ�����point�Ƿ���obb֮��
		bool contains(const Vector3& point ) const
		{
			Vector3 L = point - m_center;

			Real coord = m_rot.GetColumn(0).dotProduct( L );
			if( coord > m_extents.x || coord < -m_extents.x ) return false;

			coord = m_rot.GetColumn(1).dotProduct( L );
			if( coord > m_extents.y || coord < -m_extents.y ) return false;

			coord = m_rot.GetColumn(2).dotProduct( L );
			if( coord > m_extents.z || coord < -m_extents.z ) return false;

			return true;
		}

		// ���ذ�Χ��box�Ƿ���obb�ཻ
		bool intersects( const AxisAlignedBox& box ) const;

		// ��������һ��obb�Ƿ����obb�ཻ
		bool intersects( const OrientedBox& obb ) const;

		// ����һ�������Ƿ���obb�ཻ
		bool intersects( const Sphere& s )const;

	public:

		// ���ĵ�
		Vector3 m_center;

		// ���Ӵ�С��һ�볤�ȣ�����������Χ���е�getHalfSize��
		Vector3 m_extents;

		// ��ת����
		/// A rotation matrix describing the orientation of this box.
		/// Each of its collumns define the axes corresponding to the orientation of this box.
		Matrix3 m_rot;
	};

	// ���򵥵ı�������
	typedef OrientedBox OBB;	
};