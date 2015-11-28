// ============================================================
// �ļ���	:	TerrainEditorPlugin.h
// ������	:	������
// ˵��		:	������ʵ��
// ����ʱ��	:	2008-1-24 19:22:49
// ============================================================
#pragma once
#include "IPlugin.h"
#include "DlgTerrainBrush.h"
#include "DlgTerrainHeight.h"
#include "DlgTerrainPaint.h"
#include "TerrainEditorDll.h"

namespace Ogre
{
	class ETTerrain;
	class Selection;
	class SelectionRenderer;
	class TerrainAction;
	class TerrainBrush;

	class TERRAINEDITOR_API TerrainEditorPlugin : public IPlugin 
		, public IChildView::Listener
		, public IMainFrame::Listener
		, public IMapManager::Listener
		, public Singleton<TerrainEditorPlugin>
	{
	public:
		TerrainEditorPlugin(HINSTANCE hInstance);
		virtual ~TerrainEditorPlugin(void);
		// ����ģʽ�ӿ�
		static TerrainEditorPlugin& getSingleton(void);
		static TerrainEditorPlugin* getSingletonPtr(void);

		// ���ӿ���Ϣ
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);


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
		// ��������֪ͨ
		virtual LRESULT onDockingPaneNotify(WPARAM wParam, LPARAM lParam);

		IWorldEditor *getWorldEditor()
		{
			return m_worldEditor;
		}

		// �����Ա�༭�������
		void activateTerrain(ETTerrain *terrain);
		// ���������
		void deactivateTerrain();

		// ʵ��������β�����
		void setTerrainAction(TerrainAction *action);

		TerrainAction* getTerrainAction()
		{
			return m_terrainAction;
		}

		Selection* getSelection()
		{
			return m_selection;
		}

		SelectionRenderer* getSelectionRenderer()
		{
			return m_selectionRenderer;
		}

		TerrainBrush* getTerrainBrush()
		{
			return m_brush;
		}

		ETTerrain* getActivatedTerrain()
		{
			return m_activatedTerrain;
		}

		// ���ٳ���
		virtual void onDestroyScene(IScene *scene);
		// ���泡��
		virtual void onPreSaveScene(const String &fileName ,IScene *scene);
		// ����һ������
		virtual void onPostLoadScene(const String &fileName ,IScene *scene);
		// �½�����
		virtual void onNewScene(IScene *scene);

		// Ϊ��ǰ����ĵ������һ���ϰ��������ʾPass
		void addTerrainBlockerPass();
		// �ѵ�ǰ����ĵ��ε��ϰ�������ʾPass�Ƴ�
		void removeTerrainBlockerPass();

	protected:

		// Ϊ�������һ���ϰ��������ʾPass
		void addTerrainBlockerPass(ETTerrain *terrain);
		// �ѵ��ε��ϰ�������ʾPass�Ƴ�
		void removeTerrainBlockerPass(ETTerrain *terrain);


		bool m_inited;
		bool m_activated;

		// �������
		Description m_description;

		// ����༭���ӿ�
		IWorldEditor *m_worldEditor;

		DlgTerrainBrush m_DlgTerrainBrush;
		DlgTerrainHeight m_DlgTerrainHeight;
		DlgTerrainPaint m_DlgTerrainPaint;

		// ѡ���˵����ݱ��浽����
		Selection *m_selection;
		// ѡ����Ⱦ��
		SelectionRenderer *m_selectionRenderer;
		// ��ѡ�����еĶ���
		TerrainAction *m_terrainAction;

		// ��ǰѡ��ı�ˢ
		TerrainBrush *m_brush;
		// ����ϵͳ
		ETTerrain* m_activatedTerrain;

	};

};