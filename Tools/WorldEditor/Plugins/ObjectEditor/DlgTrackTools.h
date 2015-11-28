#pragma once
#include "Resource.h"
#include "TrackKeyFrameObject.h"

// DlgTrackTools dialog
using namespace Ogre;

class DlgTrackTools;

// �ؼ�֡����洢������
class KeyObjectData
{
public:
	KeyObjectData()
		: m_index(-1)
		, m_object(0)
		, m_dlgTrackTools(0)
		, m_keyObject(0)
	{

	}
	// �Ƿ�Ϊ��
	bool isNull()const
	{
		return m_index == -1 || m_object == 0 || m_dlgTrackTools == 0 || m_keyObject == 0;
	}

	// ���л�����
	inline friend std::ostream& operator <<
	( std::ostream& o, const KeyObjectData& v )
	{
		o << "KeyObjectData(" << v.m_index << ", " << (v.m_object ? v.m_object->getName() : StringUtil::BLANK) << ")";
		return o;
	}
	// �ؼ�֡����
	size_t m_index;
	// �����켣����
	AnimationTrackObject *m_object;
	// �����ؼ�֡��ʾ����
	TrackKeyFrameObject *m_keyObject;
	// �켣�༭��
	DlgTrackTools *m_dlgTrackTools;
};

// �켣�༭��
class DlgTrackTools : public CDialog
	, public IChildView::Listener
	, public FrameListener
{
	DECLARE_DYNAMIC(DlgTrackTools)

public:
	DlgTrackTools(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgTrackTools();

// Dialog Data
	enum { IDD = IDD_DIALOG_TRACK_TOOLS };

	// ����༭һ�������켣����
	void activate(AnimationTrackObject *obj);
	// ������
	void deactivate();

	// �Ƿ��Ѿ�����
	bool isActivate()const
	{
		return m_object != 0;
	}
	// ���ӿ���Ϣ������false��ִ�к����������
	virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	// ɾ��һ���ؼ�֡
	void deleteKeyFrame(KeyObjectData &data);
	// ��¡һ���ؼ�֡
	KeyObjectData cloneKeyFrame(KeyObjectData &data);


	// ��β������һ���ؼ�֡
	KeyObjectData pushBackKeyFrame(const AnimationTrackObject::KeyFrame &keyFrame);
	// ��β����һ���ؼ�֡ɾ��
	void popBackKeyFrame();

	// �����������йؼ�֡����
	void rebuildKeyFrameIndex();

	// ��ö����켣����
	AnimationTrackObject *getAnimationTrackObject()const
	{
		return m_object;
	}
	// ÿ֡�����Ƿ�ģ�����
	virtual bool frameStarted(const FrameEvent& evt);
	// ��ֹ��Enter��Esc�Ѵ��ڹر���
	virtual void OnOK(){};
	virtual void OnCancel(){};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	// ����һ���ؼ������
	KeyObjectData createKeyObject(size_t index);
	// �����켣����
	AnimationTrackObject *m_object;

	typedef std::deque<TrackKeyFrameObject*> ListKeyObject;
	// ���йؼ���
	ListKeyObject m_listKeyObject;

	// �ؼ���ĸ�
	ISceneObject *m_keyObjectRoot;

	// ģ�����
	CharacterObject *m_simulateObject;
	// ģ�������
	AnimationTrackObject::IAnimationTrackController *m_controller;
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonSelect();
	afx_msg void OnBnClickedButtonDelete3();
	afx_msg void OnBnClickedButtonClone2();
	afx_msg void OnBnClickedButtonPlacefloor2();
	afx_msg void OnBnClickedButtonDrag();
	afx_msg void OnBnClickedButtonMoveX();
	afx_msg void OnBnClickedButtonRotateX2();
	afx_msg void OnBnClickedButtonScaleX2();
	afx_msg void OnBnClickedButtonSimulate();
};
