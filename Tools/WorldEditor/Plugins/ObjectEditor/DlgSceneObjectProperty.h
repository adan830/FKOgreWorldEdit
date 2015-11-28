#pragma once
#include "DlgObjectProperty.h"

class DlgSceneObjectProperty :
	public DlgObjectProperty
	, public IMapManager::Listener
{
public:
	DlgSceneObjectProperty(CWnd* pParent = NULL);
	virtual ~DlgSceneObjectProperty(void);


	// ѡ��һ����������
	virtual void onSelectObject(ISceneObject *obj);
	// ѡ��һ����������
	virtual void onSelectObject(const IMapManager::ListSceneObject &listSceneObject);
	// ��ѡһ����������
	virtual void onDeselectObject(ISceneObject *obj);
	// ��ѡһ����������
	virtual void onDeselectObject(const IMapManager::ListSceneObject &listSceneObject);

};
