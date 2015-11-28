#pragma once
namespace Ogre
{
	// ����������������ID
	enum ObjectPropertyID
	{

		ID_NullObject_Begin = 100000,
		ID_General,							// һ������
		ID_Name,							// ����
		ID_Type,							// ����
		ID_Position,						// λ��
		ID_Rotate,							// ��ת
		ID_Scale,							// ����
		ID_Visible,							// �ɼ�
		ID_Parent,							// ���ڵ�
		ID_CanCollide,						// �Ƿ�μ���ײ����
		ID_NullObject_End = 199999,

		ID_EntityObject_Begin = 200000,
		ID_Entity,							// ģ������
		ID_MeshFile,						// ģ���ļ�
		ID_Alpha,							// ͸����
		ID_CollisionShapeType,				// ��ײģ������
		ID_CollisionShapeName,				// ��ײģ������
		ID_EntityObject_End = 299999,

		ID_CameraObject_Begin = 300000,
		ID_Camera,							// ���������
		ID_PolygonMode,						// ������Ⱦģʽ
		ID_LodBias,							// LODƫ��
		ID_NearClipDistance,				// ������
		ID_FarClipDistance,					// Զ����
		ID_FOVy,							// FOVy�Ƕ�
		ID_AspectRatio,						// ���ڱ���
		ID_ProjectionType,					// ͶӰ��ʽ
		ID_OrthoWindow,						// ����ͶӰ���ڴ�С
		ID_CameraObject_End = 399999,

		ID_ParticleObject_Begin = 400000,
		ID_Particle,						// ��������
		ID_ParticleTemplate,				// ����ģ��
		ID_ParticleObject_End = 499999,

		ID_TerrainObject_Begin = 500000,
		ID_Terrain,							// ��������
		ID_TerrainFile,						// �����ļ�
		ID_TerrainObject_End = 599999,

		ID_EnvironmentObject_Begin = 600000,
		ID_Environment,						// ��������
		ID_SkyBox,							// ��պ�����
		ID_SkyBoxEnabled,					// �Ƿ�����պ�
		ID_SkyBoxMaterialName,				// ��պ�����
		ID_SkyBoxRenderDistance,			// ��Ⱦ����
		ID_Fog,								// ������
		ID_FogMode,							// ����ʽ
		ID_FogColor,						// ����ɫ
		ID_FogExpDensity,					// ���ܶ�
		ID_FogStart,						// ����ʼ����
		ID_FogEnd,							// ����������
		ID_Lighting,						// ��������
		ID_AmbientLight,					// ��������ɫ
		ID_EnvironmentObject_End = 699999,


		ID_TriggerObject_Begin = 700000,
		ID_Trigger,							// ����������
		ID_TriggerCondition,				// ��������
		ID_TriggerOnEnterEvent,				// �����¼�
		ID_TriggerOnLeaveEvent,				// �˳��¼�
		ID_TriggerObject_End = 709999,

		ID_BoxTriggerObject_Begin = 750000,
		ID_BoxTrigger,						// ���Ӵ���������
		ID_BoxTriggerAxisAlignedBox,		// ���Ӵ������İ�Χ��
		ID_BoxTriggerObject_End = 759999,

		ID_SphereTriggerObject_Begin = 760000,
		ID_SphereTrigger,					// ���崥��������
		ID_SphereRadius,					// ����뾶
		ID_SphereTriggerObject_End = 769999,

		ID_AnimationTrackObject_Begin = 800000,
		ID_AnimationTrack,					// �����켣
		ID_AnimationLoop,					// �Ƿ�ѭ��
		ID_AnimationInterpolationMode,		// ��ֵģʽ
		ID_AnimationRotationInterpolationMode,		// ��ת��ֵģʽ
		ID_AnimationKeyFrames,				// �ؼ�֡�б�
		ID_AnimationKeyFrames_Begin = 810000,// �ؼ�֡�б��ID��ʼ
		ID_AnimationKeyFrames_End = 850000,	// �ؼ�֡�б��ID����
		ID_AnimationTrackObject_End = 899999,


	};
};