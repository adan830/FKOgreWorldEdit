// ============================================================
// ������Ŀ	:	MMORPG
// �ļ���	:	TerrainActionImpl.h
// ������	:	������
// ˵��		:	������Ϊ��ʵ��
// ����ʱ��	:	2007-8-20 15:31:41
// ============================================================
#pragma once
#include "ETSelection.h"
#include "ETTerrain.h"
#include "SelectionRenderer.h"
#include "TerrainBrush.h"
namespace Ogre
{



	// add by ������ 2007-7-23
	// ��ѡ��
	class MultiSelectionAction : public TerrainAction
	{
	public:
		MultiSelectionAction(Camera *pCamera , SelectionRenderer *pSelectionRenderer , ETTerrain* pTerrain , InputListener *pListener , Selection *pSelection , TerrainBrush *pBrush , int paintChannel = 0);
		virtual ~MultiSelectionAction(void);
		
		/**
		*/
		virtual void update(float deltaTime);
		/**
		*/
		virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed);

		/**
		*/
		virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed);

		/**
		*/
		virtual bool mouseMoved(const OIS::MouseEvent &arg, bool& consumed);
	};


	// add by ������ 2007-7-23
	// �������θ߶�
	template<typename BaseClass>
	class TRaiseAction : public BaseClass
	{
	public:
		TRaiseAction(Camera *pCamera , SelectionRenderer *pSelectionRenderer , ETTerrain* pTerrain , InputListener *pListener , Selection *pSelection , TerrainBrush *pBrush, int paintChannel = 0)
		: BaseClass(pCamera  , pSelectionRenderer, pTerrain , pListener , pSelection , pBrush, paintChannel)
		{

		}
		virtual ~TRaiseAction(void){mSelection->clear();};
		

		virtual void update(float deltaTime)
		{
			BaseClass::update(deltaTime);
		}
		
		virtual bool keyReleased(const OIS::KeyEvent &arg, bool& consumed)
		{
			// add by ������ 2007-8-10
			// ��ESC������������
			if(arg.key == OIS::KC_ESCAPE)
			{
				setLock(false);
				((OIS::MouseState&)mMouse->getMouseState()).buttons = 0;
			}
			return BaseClass::keyReleased(arg , consumed);
		}


		virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed)
		{
			// add by ������ 2007-7-27
			// �������Ƿ���GUI���棬����ھͲ�����
			if(consumed || CEGUI::System::getSingleton().injectMousePosition(arg.state.X.abs, arg.state.Y.abs))
				return true;

			// add by ������ 2007-8-8
			// ������¾�����ѡ��
			if(id == OIS::MB_Left)
			{
				setLock(true);
			}
			return BaseClass::mousePressed(arg , id , consumed);
		}


		virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed)
		{
			// add by ������ 2007-8-8
			// �������ͽ���ѡ��
			if(id == OIS::MB_Left)
			{
				setLock(false);
			}
			return BaseClass::mouseReleased(arg , id , consumed);
		}


		virtual bool mouseMoved(const OIS::MouseEvent &arg, bool& consumed)
		{
			if(consumed)
				return BaseClass::mouseMoved(arg , consumed);
			// add by ������ 2007-7-23
			// ����������͵����߶�
			if(arg.state.buttonDown(OIS::MB_Left))
			{
				// add by ������ 2007-7-23
				// ����Y���ƶ������������߶�
				mTerrain->getTerrainManager()->deform(*mSelection , -(float)arg.state.Y.rel * 0.01f * mBrush->getStrength());
				mSelectionRenderer->needRedraw();
			}
			return BaseClass::mouseMoved(arg , consumed);
		}

	};

	// add by ������ 2007-7-23
	// �������θ߶�
	template<typename BaseClass>
	class TRaiseUpAction : public BaseClass
	{
	protected:
		bool mLeftMouseDown;
	public:
		TRaiseUpAction(Camera *pCamera , SelectionRenderer *pSelectionRenderer , ETTerrain* pTerrain , InputListener *pListener , Selection *pSelection , TerrainBrush *pBrush, int paintChannel = 0)
		: BaseClass(pCamera  , pSelectionRenderer, pTerrain , pListener , pSelection , pBrush, paintChannel)
		, mLeftMouseDown(false)
		{

		}
		virtual ~TRaiseUpAction(void){mSelection->clear();};

		virtual void update(float deltaTime)
		{
			if(mLeftMouseDown)
			{
				// add by ������ 2007-7-23
				// ����Y���ƶ������������߶�
				mTerrain->getTerrainManager()->deform(*mSelection , deltaTime* 1.0f * mBrush->getStrength());
				mSelectionRenderer->needRedraw();
			}
			BaseClass::update(deltaTime);
		}


		virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed)
		{
			// add by ������ 2007-7-27
			// �������Ƿ���GUI���棬����ھͲ�����
			if(consumed || CEGUI::System::getSingleton().injectMousePosition(arg.state.X.abs, arg.state.Y.abs))
				return true;

			// add by ������ 2007-8-8
			// ������¾�����ѡ��
			if(id == OIS::MB_Left)
			{
				mLeftMouseDown = true;
			}
			return BaseClass::mousePressed(arg , id , consumed);
		}


		virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed)
		{
			// add by ������ 2007-8-8
			// �������ͽ���ѡ��
			if(id == OIS::MB_Left)
			{
				mLeftMouseDown = false;
			}
			return BaseClass::mouseReleased(arg , id , consumed);
		}


		virtual bool mouseMoved(const OIS::MouseEvent &arg, bool& consumed)
		{
			if(consumed)
				return BaseClass::mouseMoved(arg , consumed);

			return BaseClass::mouseMoved(arg , consumed);
		}

	};

	// add by ������ 2007-7-23
	// �������θ߶�
	template<typename BaseClass>
	class TRaiseDownAction : public BaseClass
	{
	protected:
		bool mLeftMouseDown;
	public:
		TRaiseDownAction(Camera *pCamera , SelectionRenderer *pSelectionRenderer , ETTerrain* pTerrain , InputListener *pListener , Selection *pSelection , TerrainBrush *pBrush, int paintChannel = 0)
		: BaseClass(pCamera  , pSelectionRenderer, pTerrain , pListener , pSelection , pBrush, paintChannel)
		, mLeftMouseDown(false)
		{

		}
		virtual ~TRaiseDownAction(void){mSelection->clear();};
		

		virtual void update(float deltaTime)
		{
			if(mLeftMouseDown)
			{
				// add by ������ 2007-7-23
				// ����Y���ƶ������������߶�
				mTerrain->getTerrainManager()->deform(*mSelection , -deltaTime* 1.0f * mBrush->getStrength());
				mSelectionRenderer->needRedraw();
			}
			BaseClass::update(deltaTime);
		}


		virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed)
		{
			// add by ������ 2007-7-27
			// �������Ƿ���GUI���棬����ھͲ�����
			if(consumed || CEGUI::System::getSingleton().injectMousePosition(arg.state.X.abs, arg.state.Y.abs))
				return true;

			// add by ������ 2007-8-8
			// ������¾�����ѡ��
			if(id == OIS::MB_Left)
			{
				mLeftMouseDown = true;
			}
			return BaseClass::mousePressed(arg , id , consumed);
		}


		virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed)
		{
			// add by ������ 2007-8-8
			// �������ͽ���ѡ��
			if(id == OIS::MB_Left)
			{
				mLeftMouseDown = false;
			}
			return BaseClass::mouseReleased(arg , id , consumed);
		}


		virtual bool mouseMoved(const OIS::MouseEvent &arg, bool& consumed)
		{
			if(consumed)
				return BaseClass::mouseMoved(arg , consumed);

			return BaseClass::mouseMoved(arg , consumed);
		}

	};
	// add by ������ 2007-7-25
	// ����ƽ������
	template<typename BaseClass>
	class TSmoothAction : public BaseClass
	{
	public:
		TSmoothAction(Camera *pCamera , SelectionRenderer *pSelectionRenderer , ETTerrain* pTerrain , InputListener *pListener , Selection *pSelection , TerrainBrush *pBrush, int paintChannel = 0)
			: BaseClass(pCamera , pSelectionRenderer , pTerrain , pListener , pSelection , pBrush , paintChannel)
			, mLeftMouseDown(false)
			, mRightMouseDown(false)
		{
			
		}
		virtual ~TSmoothAction(void){mSelection->clear();};


		virtual void update(float deltaTime)
		{
			// add by ������ 2007-7-25
			// �����������ƽ������
			if(mLeftMouseDown)
			{
				mTerrain->getTerrainManager()->smooth(*mSelection , 1.0f * deltaTime* 1.0f);
				mSelectionRenderer->needRedraw();
			}

			BaseClass::update(deltaTime);
		}


		virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed)
		{
			// add by ������ 2007-7-27
			// �������Ƿ���GUI���棬����ھͲ�����
			if(consumed || CEGUI::System::getSingleton().injectMousePosition(arg.state.X.abs, arg.state.Y.abs))
				return true;

			if(id == OIS::MB_Left)
			{
				mLeftMouseDown = true;
			}
			else if(id == OIS::MB_Right)
			{
				mRightMouseDown = true;
			}
			return BaseClass::mousePressed(arg , id , consumed);
		}


		virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed)
		{
			if(id == OIS::MB_Left)
			{
				mLeftMouseDown = false;
			}
			else if(id == OIS::MB_Right)
			{
				mRightMouseDown = false;
			}
			return BaseClass::mouseReleased(arg , id , consumed);
		}


		virtual bool mouseMoved(const OIS::MouseEvent &arg, bool& consumed)
		{
			return BaseClass::mouseMoved(arg , consumed);
		}


	protected:
		bool mLeftMouseDown;
		bool mRightMouseDown;
	};

	// add by ������ 2007-7-25
	// Ĩƽ����
	template<typename BaseClass>
	class TAvgFlattenAction : public BaseClass
	{
	public:
		TAvgFlattenAction(Camera *pCamera , SelectionRenderer *pSelectionRenderer , ETTerrain* pTerrain , InputListener *pListener , Selection *pSelection , TerrainBrush *pBrush, int paintChannel = 0)
			: BaseClass(pCamera , pSelectionRenderer , pTerrain , pListener , pSelection , pBrush, paintChannel)
			, mLeftMouseDown(false)
			, mRightMouseDown(false)
		{

		}
		virtual ~TAvgFlattenAction(void){mSelection->clear();};


		virtual void update(float deltaTime)
		{

			// add by ������ 2007-7-25
			// �������,Ĩƽ����
			if(mLeftMouseDown)
			{
				mTerrain->getTerrainManager()->avgFlatten(*mSelection , 1.0f * deltaTime* 1.0f);
				mSelectionRenderer->needRedraw();
			}

			BaseClass::update(deltaTime);
		}


		virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed)
		{
			// add by ������ 2007-7-27
			// �������Ƿ���GUI���棬����ھͲ�����
			if(consumed || CEGUI::System::getSingleton().injectMousePosition(arg.state.X.abs, arg.state.Y.abs))
				return true;

			if(id == OIS::MB_Left)
			{
				mLeftMouseDown = true;
			}
			else if(id == OIS::MB_Right)
			{
				mRightMouseDown = true;
			}
			return BaseClass::mousePressed(arg , id , consumed);
		}


		virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed)
		{
			if(id == OIS::MB_Left)
			{
				mLeftMouseDown = false;
			}
			else if(id == OIS::MB_Right)
			{
				mRightMouseDown = false;
			}
			return BaseClass::mouseReleased(arg , id , consumed);
		}


		virtual bool mouseMoved(const OIS::MouseEvent &arg, bool& consumed)
		{
			return BaseClass::mouseMoved(arg , consumed);
		}


	protected:
		bool mLeftMouseDown;
		bool mRightMouseDown;
	};

	// add by ������ 2007-7-25
	// ���õ�����ͼ
	template<typename BaseClass>
	class TPaintAction : public BaseClass
	{
	public:
		TPaintAction(Camera *pCamera , SelectionRenderer *pSelectionRenderer , ETTerrain* pTerrain , InputListener *pListener , Selection *pSelection , TerrainBrush *pBrush, int paintChannel = 0)
			: BaseClass(pCamera , pSelectionRenderer , pTerrain , pListener , pSelection , pBrush, paintChannel)
			, mLeftMouseDown(false)
			, mRightMouseDown(false)
		{
			
		}
		virtual ~TPaintAction(void){mSelection->clear();};


		virtual void update(float deltaTime)
		{
			// add by ������ 2007-8-21
			// ������
			if(mLeftMouseDown)
			{
				mTerrain->getSplattingManager()->paint(mPaintChannel, *mSelection , mBrush->getStrength());
			}

			BaseClass::update(deltaTime);
		}


		virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed)
		{
			// add by ������ 2007-7-27
			// �������Ƿ���GUI���棬����ھͲ�����
			if(consumed || CEGUI::System::getSingleton().injectMousePosition(arg.state.X.abs, arg.state.Y.abs))
				return true;

			if(id == OIS::MB_Left)
			{
				mLeftMouseDown = true;
			}
			else if(id == OIS::MB_Right)
			{
				mRightMouseDown = true;
			}
			return BaseClass::mousePressed(arg , id , consumed);
		}


		virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed)
		{
			if(id == OIS::MB_Left)
			{
				mLeftMouseDown = false;
			}
			else if(id == OIS::MB_Right)
			{
				mRightMouseDown = false;
			}
			return BaseClass::mouseReleased(arg , id , consumed);
		}


		virtual bool mouseMoved(const OIS::MouseEvent &arg, bool& consumed)
		{
			return BaseClass::mouseMoved(arg , consumed);
		}


	protected:
		bool mLeftMouseDown;
		bool mRightMouseDown;
	};

	// add by ������ 2007-8-10
	// ���Զ���ľ���
	class TestDistanceAction : public TerrainAction
		, public SimpleRenderable
	{
	public:
		TestDistanceAction(Camera *pCamera , ETTerrain* pTerrain , InputListener *pListener);
		virtual ~TestDistanceAction(void);

		virtual void update(float deltaTime);

		virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed);
		virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed);
		virtual bool keyReleased(const OIS::KeyEvent &arg, bool& consumed);

		virtual bool mouseMoved(const OIS::MouseEvent &arg, bool& consumed);
		void pushPoint(const Vector3 &pos);
		void popPoint();
		void updateLastPoint(const Vector3 &pos);

		void clearAllPoints();

		Real getSquaredViewDepth(const Camera *cam) const;
		Real getBoundingRadius() const;
	protected:
		void setPointPosition(size_t index , const Vector3 &pos);

		const Quaternion &getWorldOrientation() const;
		const Vector3 &getWorldPosition() const;

		struct PointText
		{
			PointText(SceneManager *pMgr)
				: mMovableText(0)
				, mMovableTextNode(0)
				, mLen(0.0f)
				, mAllLen(0.0f)
				, mHeightLen(0.0f)
				, mAllHeightLen(0.0f)
				, mSceneMgr(pMgr)
			{
			}
			~PointText()
			{
				OGRE_DELETE(mMovableText);
				if(mMovableTextNode)
				{
					String strName = mMovableTextNode->getName();
					mSceneMgr->destroySceneNode(strName);
				}
			}
			MovableText *mMovableText;
			SceneNode *mMovableTextNode;
			// add by ������ 2007-8-13
			// ����һ����֮��ľ���
			Real mLen;
			// add by ������ 2007-8-13
			// ���о���
			Real mAllLen;
			// add by ������ 2007-8-13
			// ����һ����֮��ĸ߶Ȳ��
			Real mHeightLen;
			// add by ������ 2007-8-13
			// ���и߶Ȳ��
			Real mAllHeightLen;

			SceneManager *mSceneMgr;
		};
		typedef std::vector<TestDistanceAction::PointText*> PointList;
		PointList mPointList;
	};

	
	// add by ������ 2007-8-10
	// ���Զ���ľ���
	class FixVertexTestDistanceAction : public TestDistanceAction
	{
	public:
		FixVertexTestDistanceAction(Camera *pCamera , ETTerrain* pTerrain , InputListener *pListener);
		virtual ~FixVertexTestDistanceAction(void){};

		virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed);
		virtual bool mouseMoved(const OIS::MouseEvent &arg, bool& consumed);
	};


	// add by ������ 2007-7-23
	// ��ѡ״̬�µĵ����߶ȣ�����ģʽ
	typedef TRaiseAction<SingleSelectionAction> SingleRaiseAction;
	typedef TRaiseUpAction<SingleSelectionAction> SingleRaiseUpAction;
	typedef TRaiseDownAction<SingleSelectionAction> SingleRaiseDownAction;
	// add by ������ 2007-7-25
	// ��ѡ״̬�µĵ���ƽ�����ɣ�����ģʽ
	typedef TSmoothAction<SingleSelectionAction> SingleSmoothAction;
	// add by ������ 2007-7-25
	// ��ѡ״̬�µĵ���Ĩƽ������ģʽ
	typedef TAvgFlattenAction<SingleSelectionAction> SingleAvgFlattenAction;
	// add by ������ 2007-7-25
	// ��ѡ״̬�µ����õ�����ͼ������ģʽ
	typedef TPaintAction<SingleSelectionAction> SinglePaintAction;
	// add by ������ 2007-7-23
	// ��ѡ֮������߶�
	typedef TRaiseAction<TerrainAction> MultiRaiseAction;

};