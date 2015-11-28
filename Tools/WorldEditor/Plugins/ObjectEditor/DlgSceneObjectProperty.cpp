#include "StdAfx.h"
#include "DlgSceneObjectProperty.h"
#include "ObjectEditorPlugin.h"

DlgSceneObjectProperty::DlgSceneObjectProperty(CWnd* pParent)
: DlgObjectProperty(pParent)
{
}

DlgSceneObjectProperty::~DlgSceneObjectProperty(void)
{
	ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->removeListener(this);
}

// ѡ��һ����������
void DlgSceneObjectProperty::onSelectObject(ISceneObject *obj)
{
	refreshProperty(obj);
}
// ѡ��һ����������
void DlgSceneObjectProperty::onSelectObject(const IMapManager::ListSceneObject &listSceneObject)
{
	ListPropertyContainer propertiesList;
	for(IMapManager::ListSceneObject::const_iterator iter = listSceneObject.begin() ; iter != listSceneObject.end() ; iter ++)
	{
		propertiesList.push_back(*iter);
	}
	refreshProperty(propertiesList);
}

// ��ѡһ����������
void DlgSceneObjectProperty::onDeselectObject(ISceneObject *obj)
{
	if(std::count(m_listObject.begin() , m_listObject.end() , obj) > 0)
	{
		clearProperty();
	}
}
// ��ѡһ����������
void DlgSceneObjectProperty::onDeselectObject(const IMapManager::ListSceneObject &listSceneObject)
{
	for(ListPropertyContainer::iterator iter = m_listObject.begin() ; iter != m_listObject.end() ; iter ++)
	{
		if(std::count(listSceneObject.begin() , listSceneObject.end() , *iter) > 0)
		{
			clearProperty();
			return;
		}
	}
}