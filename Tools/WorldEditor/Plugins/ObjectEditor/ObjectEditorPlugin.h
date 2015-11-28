// ============================================================
// �ļ���	:	ObjectEditorPlugin.h
// ������	:	������
// ˵��		:	����༭���ʵ��
// ����ʱ��	:	2008-1-24 19:22:49
// ============================================================
#pragma once
#include "IPlugin.h"
#include "DlgPlaceSceneObject.h"
#include "DlgEditTools.h"
#include "DlgTrackTools.h"
#include "DlgSceneObjectProperty.h"
#include "DlgObjectList.h"
namespace Ogre
{
	class ObjectAction;
	class OBJECTEDITOR_API ObjectEditorPlugin : public IPlugin 
		, public IMainFrame::Listener
		, public IChildView::Listener
		, public IMapManager::Listener
		, public IScene::Listener
		, public Singleton<ObjectEditorPlugin>
	{
	public:
		ObjectEditorPlugin(HINSTANCE hInstance);
		virtual ~ObjectEditorPlugin(void);
		// ����ģʽ�ӿ�
		static ObjectEditorPlugin& getSingleton(void);
		static ObjectEditorPlugin* getSingletonPtr(void);


		// ��ʼ���������������worldEditor������༭���Ľӿ�
		virtual bool init(IWorldEditor *worldEditor);
		// �رղ��
		virtual void shutdown();
		// �Ƿ��Ѿ���ʼ��
		virtual bool isInited()const;

		// ������
		virtual void activate();
		// ��������
		virtual void deactivate();
		// �Ƿ��Ѿ�����
		virtual bool isActivate()const;

		// �ӿڸ���
		virtual void preViewportUpdate(const RenderTargetViewportEvent& evt);

		// ��ò������
		virtual const Description &getDescription()const;

		// ��ѯһЩ���ݣ�Ȼ�󷵻ص�result��
		virtual bool query(const String &name , const NameValuePairList *params , void *result);
		// ��������Ϣ
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		// ��������֪ͨ
		LRESULT onDockingPaneNotify(WPARAM wParam, LPARAM lParam);


		// �������༭���ӿ�
		IWorldEditor *getWorldEditor()
		{
			return m_worldEditor;
		}

		// �Ƿ�֧�ִ�����Ķ�̬�༭
		bool isEditable(ISceneObject *obj)const;
		// ��һ��MO���һ���ɱ༭�ĳ������
		ISceneObject* getEditableSceneObject(MovableObject *obj)const;

		// ���õ�ǰ�����������
		void setObjectAction(ObjectAction *objectAction);

		// ������ײ��⣬�����ص�һ��������ײ����Ϣ�����û�е��Σ�����ײ0ƽ�棩
		// @ray ����
		// @collideWorld �Ƿ��������ײ���磺���Σ�
		// @collideObject �Ƿ�������ײ
		// @intersection �������
		virtual bool collide(const Ray& ray , bool collideWorld , bool collideObject , Vector3& intersection);


		// �½�����
		virtual void onNewScene(IScene *scene);
		// ����һ������
		virtual void onPreLoadScene(const String &fileName ,IScene *scene);
		// ���ٳ���
		virtual void onDestroyScene(IScene *scene);

		// ������һ��������������
		virtual void onCreateSceneObject(ISceneObject *obj);

		// ѡ��һ����������
		virtual void onSelectObject(ISceneObject *obj);
		// ��ѡһ����������
		virtual void onDeselectObject(ISceneObject *obj);

		// ��ȡ�켣�༭��
		DlgTrackTools *getDlgTrackTools()
		{
			return &m_DlgTrackTools;
		}
	protected:
		// ֧�ֱ༭��SceneObject����
		std::list<String> m_editableSceneObjectType;

		bool m_inited;
		bool m_activated;

		// �������
		Description m_description;

		// ����༭���ӿ�
		IWorldEditor *m_worldEditor;

		DlgPlaceSceneObject m_PlaceSceneObject;
		DlgEditTools m_DlgEditTools;
		DlgTrackTools m_DlgTrackTools;
		DlgSceneObjectProperty m_DlgObjectProperty;
		DlgObjectList m_DlgObjectList;

		// ��ǰ�����������
		ObjectAction *m_objectAction;
	};

};