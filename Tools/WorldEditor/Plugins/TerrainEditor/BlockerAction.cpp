#include "StdAfx.h"
#include "BlockerAction.h"
#include "TerrainEditorPlugin.h"

namespace Ogre
{
	BlockerAction::BlockerAction(bool isBlock , ETTerrain* pTerrain , SelectionRenderer *pSelectionRenderer , Selection *pSelection , TerrainBrush *pBrush)
		: SingleSelectionAction(pTerrain , pSelectionRenderer , pSelection , pBrush)
		, m_isBlock(isBlock)
		, m_canAction(false)
	{
		Root::getSingleton().addFrameListener(this);
	}

	BlockerAction::~BlockerAction(void)
	{
		Root::getSingleton().removeFrameListener(this);
	}

	// ���ӿ���Ϣ
	bool BlockerAction::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_LBUTTONDOWN:
			m_canAction = true;
			break;
		case WM_LBUTTONUP:
			m_canAction = false;
			break;
		}
		return SingleSelectionAction::onWndMsg(view , message , wParam , lParam , pResult);
	}

	bool BlockerAction::frameStarted(const FrameEvent& evt)
	{
		if(m_canAction)
		{
			// ����������
			TexturePtr &blockerTexture = mTerrain->getBlockerTexture();
			HardwarePixelBufferSharedPtr hardwareBuffer = blockerTexture->getBuffer();
			ARGB *buffer = (ARGB*)hardwareBuffer->lock(0 , hardwareBuffer->getSizeInBytes() , HardwareBuffer::HBL_DISCARD);

			size_t rowPitch = hardwareBuffer->getWidth();
			// д������
			for(size_t y = mSelection->getMin().y ; y < mSelection->getMax().y && y < hardwareBuffer->getHeight(); y ++)
			{
				for(size_t x = mSelection->getMin().x ; x < mSelection->getMax().x && x < hardwareBuffer->getWidth() ; x ++)
				{
					ARGB &color = buffer[y * rowPitch + x];
					ColourValue ogreColor;
					ogreColor.setAsARGB(color);
					// ������ϰ��������óɰ�͸������������ϰ��������ó�ȫ��͸��
					ogreColor.a = (m_isBlock ? 0.5f : 0.0f);
					color = ogreColor.getAsARGB();
				}
			}
			hardwareBuffer->unlock();
		}
		return true;
	}
}; 