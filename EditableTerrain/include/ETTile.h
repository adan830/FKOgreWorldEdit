#ifndef __ETTILE_H__
#define __ETTILE_H__

#include "ETPrerequisites.h"

#include "ETIndexHandler.h"
#include "ETOptions.h"

// forward declarations
namespace Ogre
{
	class SceneManager;
	class RenderOperation;
	class Matrix4;
	class Quaternion;
	class Vector3;
	class Camera;
}

namespace Ogre
{
	// forward declarations
	class TerrainInfo;
	class SutureRenderable;

	class IndexHandler;
	class TerrainImpl;

	// �Զ�������Ӳ������
	class ET_EXPORT AutoBuffer
	{
	public:
		AutoBuffer(HardwareBuffer* data)
		{
			m_data = data;
			m_buffer = m_data->lock(HardwareBuffer::HBL_DISCARD);
		}
		void *getBuffer()
		{
			return m_buffer;
		}
		virtual ~AutoBuffer()
		{
			m_data->unlock();
		}
	private:
		void *m_buffer;
		HardwareBuffer* m_data;
	};

	typedef SharedPtr<AutoBuffer> AutoBufferPtr;
	/** A tile is the unit of terrain used for rendering. */
	class ET_EXPORT Tile : public Renderable , public MovableObject
	{
	public:
		Tile(TerrainImpl* tm,
			IndexHandler* indexHandler, TerrainInfo* info, const Options &opts, size_t startx, size_t startz);
		~Tile();

		static const String MOVABLE_OBJECT_TYPE;

		uint32 getTypeFlags() const;

		const MaterialPtr& getMaterial() const;

		void getRenderOperation(RenderOperation& op);

		void getWorldTransforms(Matrix4* m) const;

		const Quaternion& getWorldOrientation() const;

		const Vector3& getWorldPosition() const;

		Real getSquaredViewDepth(const Camera* cam) const;

		const LightList& getLights() const;

		const String& getMovableType() const;

		const AxisAlignedBox& getBoundingBox() const;

		Real getBoundingRadius() const;

		void _updateRenderQueue(RenderQueue* queue);

		/** Determines the LOD to use based on the current camera distance */
		void _notifyCurrentCamera(Camera* cam);

		void setRenderQueue(uint8 qid);

		/** Updates the custom morph factor parameter for the morph vertex shader */
		void _updateCustomGpuParameter(const GpuProgramParameters::AutoConstantEntry& constEntry,
			GpuProgramParameters* params) const;


		/** Tells the tile to update its contents based on updated heightmap data.
		* This is what makes terrain deforming working.
		*/
		void updateTerrain(size_t startx, size_t startz, size_t endx, size_t endz);
		// ����Tile����ؽڵ�
		void attachTile(SceneNode* parent);



		// ��ñ�����ϵͳ�ڴ��еĶ��㻺����
		Vector3 *getVertexSysBuffer()
		{
			return mVertexSysBuffer;
		}

		// ��ñ�����ϵͳ�ڴ��еķ��߻�����
		Vector3 *getNormalSysBuffer()
		{
			return mNormalSysBuffer;
		}

		// ��ñ�����ϵͳ�ڴ��е�����
		ushort* getIndexSysBuffer();
		// ��ñ�����ϵͳ�ڴ��е�����
		size_t getIndexSysBufferCount();

		// ����ray�͵���tile֮�����ײ������true������ײ��posIntersection������ײ����
		bool collide(const Ray& ray , Vector3 &posIntersection);

		// ����ray�͵���tile֮�����ײ������true������ײ��distance������ײ�������������ľ���
		bool collide(const Ray& ray , float &distance);
		// ������ײģ��
		void createCollisionShape();

		// ���Lod
		ushort getLod()const
		{
			return mLOD;
		}


		// ��ö��㻺����
		VertexData *_getVertexData()const
		{
			return mVertexData;
		}
	private:
		/** Initialises the vertices */
		void createVertexData(size_t startx, size_t startz);

		// ���¼������з���
		void _calculateNormals(size_t startx, size_t startz, size_t endx, size_t endz);

		// ������ײģ��
		void destroyCollisionShape();

		// ��Ⱦ��ϱ�Ե
		void renderSutureData(RenderQueue* queue , Tile *tileNeighbour , IndexHandler::Edge edge);

		/** Retrieves the position vector for the given vertex */
		Vector3 getVector(size_t x, size_t z) const;

		/** Convenience function to empty a delta buffer */
		void emptyBuffer(HardwareVertexBufferSharedPtr buf);

		TerrainImpl* mTerrainImpl;
		IndexHandler* mIndexHandler;

		ushort mLOD;
		TerrainInfo* mInfo;
		const Options &mOpt;
		SutureRenderable *m_sutureRenderable;

		/** Info about this tile in regards to all tiles */
		size_t mTileX, mTileZ;
		size_t mStartX, mStartZ;

		AxisAlignedBox mBounds;
		Real mBoundingRadius;
		Vector3 mCenter;

		mutable bool mLightListDirty;
		mutable LightList mLightList;

		/** The tile's vertices */
		VertexData* mVertexData;
		HardwareVertexBufferSharedPtr mMainBuffer;

		// ������ϵͳ�ڴ��еĶ��㻺����
		Vector3 *mVertexSysBuffer;
		// ������ϵͳ�ڴ��еķ��߻�����
		Vector3 *mNormalSysBuffer;

		// ��ײģ��
		ICollisionShapePtr m_collisionShape;
	};
}

#endif
