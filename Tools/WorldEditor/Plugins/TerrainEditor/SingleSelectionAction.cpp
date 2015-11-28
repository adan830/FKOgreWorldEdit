#include "StdAfx.h"
#include "SingleSelectionAction.h"
#include "TerrainEditorPlugin.h"

namespace Ogre
{

	SingleSelectionAction::SingleSelectionAction(ETTerrain* pTerrain 
		, SelectionRenderer *pSelectionRenderer
		, Selection *pSelection 
		, TerrainBrush *pBrush)
		: TerrainAction(pTerrain , pSelectionRenderer , pSelection , pBrush)
	{
		TerrainEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->addChildViewListener(this);
		// ��ʼ���������������
		mSelection->clear();
	}

	SingleSelectionAction::~SingleSelectionAction(void)
	{
		TerrainEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->removeChildViewListener(this);
	}
	// ���ӿ���Ϣ
	bool SingleSelectionAction::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_MOUSEMOVE:
			{
				short xPos = GET_X_LPARAM(lParam); 
				short yPos = GET_Y_LPARAM(lParam); 
				CRect rect;
				::GetClientRect(view->getWnd()->GetSafeHwnd() , &rect);
				Vector3 pos;
				Ray ray = view->getCamera()->getCameraToViewportRay(xPos / (float)rect.Width()
					, yPos / (float)rect.Height());

				RayCheckInfo info;
				if(TerrainEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getScene()->collide(ray , true , false , info))
				{
					if(info.obj->getType() == TerrainObjectFactory::FACTORY_TYPE)
					{
						TerrainObject *terrain = (TerrainObject*)info.obj;

						// �����������
						Vector2 grid = mTerrain->getTerrainGrid(ray.getPoint(info.info.distance));

						// ����λ��
						int iX = (int)grid.x - (int)mBrush->getWidth() / 2;
						int iY = (int)grid.y - (int)mBrush->getHeight() / 2;
						GridVector2 posNew(iX < 0 ? grid.x : iX , iY < 0 ? grid.y : iY);

						mSelection->setPosition(posNew);
						return true;
					}
				}
				mSelection->clear();
			}
			break;
		}
		return true;
	}

	// �ӿڸ���
	void SingleSelectionAction::preViewportUpdate(const RenderTargetViewportEvent& evt)
	{
		// ֻ�пյ�ʱ������
		if(mSelection->empty())
		{
			mSelection->addSel((ushort)mBrush->getWidth() / 2 , (ushort)mBrush->getHeight() / 2 , *mBrush->getBrush() , mBrush->getStrength());
		}
	}

	void SingleSelectionAction::updateBrush()
	{
		mSelection->clear();
	}

};