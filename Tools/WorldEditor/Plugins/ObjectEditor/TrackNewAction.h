#pragma once
#include "ObjectAction.h"
class DlgTrackTools;
namespace Ogre
{
	// �½��켣���в���
	class TrackNewAction : public ObjectAction
		, public IChildView::Listener
	{
	public:
		TrackNewAction(DlgTrackTools *tools);
		virtual ~TrackNewAction(void);
		// ���ӿ���Ϣ
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	protected:
		// �����켣�༭��
		DlgTrackTools *m_dlgTrackTools;
		// ��ѡ��֮ǰ�ȷ�ѡ�������
		virtual void deselectAllObject();

	};
};