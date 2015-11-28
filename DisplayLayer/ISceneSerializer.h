#pragma once
namespace Ogre
{
	class IScene;
	class ISceneSerializer
	{
	public:

		ISceneSerializer(void)
		{
		}

		virtual ~ISceneSerializer(void)
		{
		}
		// ���볡��
		virtual bool importScene(DataStreamPtr& stream, IScene* scene) = 0;
		// ��������
		virtual bool exportScene(IScene* scene, const String& filename) = 0;
	};
};