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

	class ETTerrain;
	// ����������
	enum DecalType
	{
		DT_NORMAL,		// ������������������λ�õķ����Զ�������������
		DT_MESH,		// ģ�������������������ڵ������Զ�����ģ��
		DT_PROJECTIVE,	// ͶӰ����������ͶӰ����ķ�ʽʵ�ֵ�������δʵ�֣�
		DT_GRID,		// ��������������ָ����С������������Ȼ���Զ����������λ��
	};

	// һ������
	struct IDecal
	{
		virtual ~IDecal(){};
		// ����������������
		virtual const String &getName()const = 0;

		// �������������
		virtual const DecalType getType()const = 0;

		// ��ó���������
		virtual SceneManager* getSceneManager()const = 0;

		// ����������������
		virtual void setMaterialName(const String &materialName) = 0;
		// ���������������
		virtual const String &getMaterialName()const = 0;

		// ������������λ��
		virtual void setPosition(const Vector3 &pos) = 0;
		// ��ȡ��������λ��
		virtual const Vector3 &getPosition()const = 0;

		// ����������С
		virtual void setSize(const Vector2 &size) = 0;
		// ��ȡ������С
		virtual const Vector2 &getSize()const = 0;

		// ��ȡ�����İ�Χ�У��������꣩
		virtual const AxisAlignedBox &getBoundingBox()const = 0;

		// ��ȡ�����������Χ�У��������꣩
		virtual const AxisAlignedBox &getWorldBoundingBox()const = 0;

		// ������������ȣ�ָ�������ĸ߶�
		virtual void setDepth(float depth) = 0;
		// ��ȡ�������
		virtual float getDepth()const = 0;
		// ����������Y�����ת��
		virtual void setRotate(const Radian &angle) = 0;
		// ��ȡ������Y�����ת��
		virtual const Radian &getRotate()const = 0;

		// ����������������
		virtual void setVisible(bool visible) = 0;
		// ��������Ƿ���ʾ
		virtual bool isVisible()const = 0;

		// ���ùҽ��������ƫ��
		virtual void setAttachObjectsOffset(const Vector3 &offsetPosition = Vector3::ZERO
			, const Quaternion &offsetOrientation = Quaternion::IDENTITY
			, const Vector3 &offsetScale = Vector3(1.0f , 1.0f , 1.0f)
			) = 0;
		// ��ùҽ����������ת
		virtual const Quaternion &getOffsetOrientation()const = 0;
		// ��ùҽ��������λ��
		virtual const Vector3 &getOffsetPosition()const = 0;
		// ��ùҽ������������
		virtual const Vector3 &getOffsetScale()const = 0;

		// �ҽ�һ��������������ĵ���
		virtual void attachObject(MovableObject *obj) = 0;
		// ȡ���ҽ����
		virtual void dettachObject(MovableObject *obj) = 0;
		// ����������ȡ���ҽ����
		virtual void dettachObject(ushort index) = 0;
		// ȡ���ҽ��������
		virtual void detachAllObjects() = 0;

		// ��������ڵ�
		virtual SceneNode* getSceneNode() = 0;
		// ���������������Ľڵ�
		virtual SceneNode* getAttachObjectSceneNode() = 0;

		// ���������Ż�ùҽ��˵����
		virtual MovableObject* getAttachObject(ushort index)const = 0;

		// ����ѹҽӵ����ö��
		virtual SceneNode::ObjectIterator getAttachedObjectIterator(void) = 0;

		// ÿ֡��������
		virtual void update(float timeSinceLastFrame) = 0;
	};

	// ����ϵͳ
	class OGREPRO_API IDecalSystem : public Singleton<IDecalSystem>
	{
	public:
		IDecalSystem();
		virtual ~IDecalSystem();

		// ����ģʽʵ��
		static IDecalSystem& getSingleton(void);
		static IDecalSystem* getSingletonPtr(void);

		// ��ʼ������ϵͳ
		static void init(SceneManager *mgr);
		// ��������ϵͳ
		static void shutdown();

		// ʹ����������������������������֮��createDecal��ʹ���������������
		virtual void acceptSceneMgr(SceneManager *mgr) = 0;
		// ��ó���������
		virtual SceneManager* getSceneManager()const = 0;

		// ���ݲ�����ʵ��������һ�����������ʵ����Ϊ�վ��Զ�����
		virtual IDecal *createDecal(DecalType decalType
			, const String &name = StringUtil::BLANK
			, const NameValuePairList *params = 0
			) = 0;

		// ����һ������
		virtual void destroyDecal(IDecal *decal) = 0;
		virtual void destroyDecal(const String &name) = 0;

		// ������������
		virtual void destroyAllDecal() = 0;

		// ����ʵ�������һ������
		virtual IDecal *getDecal(const String &name) = 0;
		// ����Ƿ������ʵ����������
		virtual bool hasDecal(const String &name)const = 0;

		typedef std::map<String , IDecal*> MapDecal;

		// �����������ö��
		typedef MapIterator<MapDecal> DecalIterator;
		virtual DecalIterator getDecalIterator() = 0;

		// �����������ö��
		typedef ConstMapIterator<MapDecal> ConstDecalIterator;
		virtual ConstDecalIterator getDecalIterator()const = 0;

		// ������ģ�������߼�⣨ֻ��Ե��Σ�
		virtual ETTerrain* worldRayQuery(Vector3 &posIntersect , const Vector3 &posStart , const Vector3 &posEnd) = 0;
		virtual ETTerrain* worldRayQuery(Vector3 &posIntersect , const Ray &ray) = 0;

		// ����������������߼�⣨�������Ρ��������塢��ҡ�����ȵȣ�
		virtual bool rayQuery(Vector3 &posIntersect , const Vector3 &posStart , const Vector3 &posEnd) = 0;

		// ��������ԣ��������罻�����꣨���Σ�
		virtual ETTerrain* hitTestWorld(Vector3 &posIntersect , short cursorX, short cursorY , Camera *camera) = 0;

	};
};