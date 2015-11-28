/******************************************************************
** �ļ���:	DecalSystem.h
** ��  Ȩ:	(C)  
** ������:	FreeKnight(������)
** ��  ��:	2008-1-10 9:53:30
** ��  ��:
******************************************************************/
#pragma once
namespace Ogre
{
	// �������������
	static const long MaxDecalVertices = 256;
	// ����б��
	static const float DecalEpsilon = 0.25f;
	// ����������
	class MeshDecalClipper
	{
	public:

		struct Vertex
		{
			Vector3 pos;
			Vector2 uv;
			uint32 color;
		};

		MeshDecalClipper(const Vector3& center, const Vector3& normal, const Vector3& tangent, float width, float height, float depth);
		// ��ʼ�������������¹�����Ҫ����clipMesh
		void begin();
		// ������������
		void clipMesh(size_t indexCount, const ushort *indices, const Vector3 *vertex, const Vector3 *normal);
		// ������������
		void clipMesh(Renderable *renderable);
		// ����Χ���Ƿ���Decal�ཻ
		bool intersects(const AxisAlignedBox &box);
		// �����Ƿ���Decal�ཻ
		bool intersects(const Vector3 &pt);
		// ��������������������Ϻ�Ҫ����
		void end();

		// ���������ManualObject
		void fill(ManualObject *obj , const String &matName);

		// ��������İ�Χ��
		const AxisAlignedBox &getWorldBoundingBox()const
		{
			return m_aabb;
		}

		// �������������
		long getDecalVertexCount()const
		{
			return m_decalVertexCount;
		}

		// �������������
		long getDecalIndexCount()const
		{
			return m_decalTriangleCount * 3;
		}

		// ��ö��㻺����
		const Vertex* getVertexArray()const
		{
			return m_vertexArray;
		}

		// �������������
		const ushort* getIndexArray()const
		{
			return (ushort*)m_triangleArray;
		}

	private:
		struct Triangle
		{
			unsigned short	index[3];
		};
		Vector3		m_decalCenter;
		Vector3		m_decalTangent;
		Vector3		m_decalBinormal;
		Vector3		m_decalNormal;

		Vector4		m_leftPlane;
		Vector4		m_rightPlane;
		Vector4		m_bottomPlane;
		Vector4		m_topPlane;
		Vector4		m_frontPlane;
		Vector4		m_backPlane;

		float		m_width;
		float		m_height;
		float		m_depth;

		long		m_decalVertexCount;
		long		m_decalTriangleCount;

		Vertex		m_vertexArray[MaxDecalVertices];
		Triangle	m_triangleArray[MaxDecalVertices];

		// ������aabb
		AxisAlignedBox m_aabb;

		bool addPolygon(long vertexCount, const Vector3 *vertex, const Vector3 *normal);
		long clipPolygon(long vertexCount, const Vector3 *vertex, const Vector3 *normal, Vector3 *newVertex, Vector3 *newNormal) const;
		static long clipPolygonAgainstPlane(const Vector4& plane, long vertexCount, const Vector3 *vertex, const Vector3 *normal, Vector3 *newVertex, Vector3 *newNormal);
	};
};