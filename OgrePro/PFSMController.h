#pragma once
namespace Ogre
{
	// ������������״̬�ɲ��ɴ�״̬
	#define ALL_STATE -1
	// ��������״̬��ʵ����
	template<class T>
	class PFSMController
	{
	public:
		//�ص�����
		typedef void (T::*ProcCommand)(uint32 state);
		typedef void (T::*StateCommand)(float timeSinceLastFrame);
		typedef void (T::*RefuseCommand)(uint32 oldState , uint32 newState);
	protected:

		//״̬�ṹ
		struct State
		{
			State(){OnEnter=NULL; OnLeave=NULL;}
			uint32 state;
			ProcCommand OnEnter;
			ProcCommand OnLeave;
			StateCommand OnState;
		};

		typedef std::list<State> ASheet;		//һ��״̬��

		//����̬�ṹ
		struct PState
		{
			ASheet Sheet;
			uint32 CurrState;
			StateCommand OnState;
		};

		std::map<uint32, PState> m_Cont;		//����״̬��

		//����ת��״̬�ṹ
		struct InAccessible
		{
			InAccessible(){OnRefuse=NULL;}
			uint32 Sheet;
			uint32 StateFrom;
			uint32 StateTo;
			RefuseCommand OnRefuse;
		};
		std::list<InAccessible> m_InAccessible;	//����ת��״̬�б�

		T* m_CallbackClass;	

	public:

		/**
		�õ���ǰ״̬
		@param ���û�д�״̬��������0xFFFFFFFF
		*/
		uint32 getCurrentState(uint32 fsm);


		/**
		���ûص�������
		*/
		void setCallBackClass(T* callback)
		{
			m_CallbackClass = callback;
		}

		/**
		���һ������̬
		@param fsm ״̬ID
		*/
		void addFSM(uint32 fsm);

		/**
		ɾ��һ������̬
		@param fsm ״̬ID
		*/
		void deleteFSM(uint32 fsm);

		/**
		���һ����̬��һ������̬
		@param fsm ����̬ID
		@param state ��̬ID
		@param onEnter ����ص�
		@param onLeave �뿪�ص�
		*/
		void addState(uint32 fsm, uint32 state,
			ProcCommand onEnter = 0,
			ProcCommand onLeave = 0,
			StateCommand onState = 0);

		/**
		�Ӳ���̬��ɾ��һ����̬
		@param fsm ����̬ID
		@param state ��̬ID
		*/
		void deleteState(uint32 fsm, uint32 state);

		/**
		�ı�״̬
		@param fsm ����̬ID
		@param state ����̬ID
		@param force �Ƿ�ǿ�Ʊ任״̬������᲻�ɴ�������
		@note ǿ�Ʊ任ʱ��������ûص�����
		*/
		void changeState(uint32 fsm, uint32 newState, bool force = false);

		/**
		���ò��ɵ�������
		@param fsm ����̬ID
		@param stateFrom ��һ��̬
		@param stateTo	 ��仯������̬
		@param onResult  ��������ʱ�ص�
		*/
		void addInaccessible(uint32 fsm,
			uint32 stateFrom,
			uint32 stateTo,
			RefuseCommand onRefuse = 0);

		/**
		ɾ�����ɵ�������
		@param fsm ����̬ID
		@param stateFrom ��һ��̬
		@param stateTo   ��仯������̬
		*/
		void deleteInaccessible(uint32 fsm,
			uint32 stateFrom,
			uint32 stateTo);


		/**
		���ò��ɵ�������
		@param fsm ����̬ID
		@param onResult  ��������ʱ�ص�
		*/
		void addInaccessible(uint32 fsm,
			RefuseCommand onRefuse = 0);

		/**
		ɾ�����ɵ�������
		@param fsm ����̬ID
		*/
		void deleteInaccessible(uint32 fsm);

		/**
		��ǰ״̬��
		*/
		void update(float timeSinceLastFrame);

		// �����������
		void clearFSM()
		{
			m_Cont.clear();
			m_InAccessible.clear();
			m_CallbackClass = 0;
		}
	};

	template<class T> 
	void PFSMController<T>::addFSM(uint32 fsm)
	{
		//���ҵ�ǰfsm�Ƿ��Ѿ�����
		std::map<uint32, PState>::iterator it = m_Cont.find(fsm);
		if(it==m_Cont.end())
		{
			//��ӽ�ȥ
			PState newPState;
			m_Cont.insert(std::pair<uint32, PState>(fsm, newPState));
		}
		else
			return;
	}

	template<class T> 
	void PFSMController<T>::addState(uint32 fsm, uint32 state,
		ProcCommand onEnter = 0,
		ProcCommand onLeave = 0,
		StateCommand onState = 0)
	{
		//���ҵ�ǰfsm
		std::map<uint32, PState>::iterator it = m_Cont.find(fsm);
		if(it!=m_Cont.end())
		{

			PState* pState = &(it->second);
			ASheet::iterator it1 = (pState->Sheet).begin();
			for(;it1!=(pState->Sheet).end();++it1)
			{
				//���ظ�
				if(state == it1->state)
					return;
			}
			//�����״̬
			State newState;
			newState.OnEnter = onEnter;
			newState.OnLeave = onLeave;
			newState.OnState = onState;
			newState.state = state;
			(pState->Sheet).push_back(newState);
			pState->CurrState = state;		//�ı䵱ǰ״̬
			pState->OnState = onState;		//��״̬�ص�����
			return;
		}
	}

	template<class T> 
	void PFSMController<T>::changeState(uint32 fsm, uint32 newState, bool force)
	{
		//���ҵ�ǰfsm�Ƿ����
		std::map<uint32, PState>::iterator it = m_Cont.find(fsm);
		if(it!=m_Cont.end())
		{
			//������״̬�;�״̬�Ƿ����
			PState* pState = &(it->second);
			ASheet::iterator it1 = (pState->Sheet).begin();
			bool oldExist = false;
			bool newExist = false;
			ProcCommand newOnEnter = NULL;
			ProcCommand oldOnLeave = NULL;
			StateCommand newOnState = NULL;
			uint32 oldState = pState->CurrState;
			for(;it1!=(pState->Sheet).end();++it1)
			{
				//��״̬�뵱ǰ״̬�ظ�
				if(newState == pState->CurrState)
					return;

				if(it1->state == pState->CurrState)
				{
					oldExist = true;
					oldOnLeave = it1->OnLeave;
				}
				if(it1->state == newState)
				{
					newExist = true;
					newOnEnter = it1->OnEnter;
					newOnState = it1->OnState;
				}
			}
			//�����ǿ��ת��
			if(force == true && newExist == true)
			{
				if(oldOnLeave)
					(m_CallbackClass->*oldOnLeave)(newState);
				pState->CurrState = newState;
				pState->OnState = newOnState;
				if(newOnEnter!=NULL)
				{
					(m_CallbackClass->*newOnEnter)(oldState);
				}
				return;
			}

			if(newExist == true && oldExist == true)
			{
				//�����Ƿ��ǲ��ɵ�������
				std::list<InAccessible>::iterator it2 = m_InAccessible.begin();
				for(;it2!=m_InAccessible.end();++it2)
				{
					if((it2->Sheet == fsm &&
						it2->StateFrom == pState->CurrState &&
						it2->StateTo == newState)
						|| (it2->Sheet == fsm && 
						it2->StateFrom == ALL_STATE &&
						it2->StateTo == ALL_STATE)
						)
					{
						if(it2->OnRefuse)
							(m_CallbackClass->*(it2->OnRefuse))(oldState , newState);
						return;
					}
				}
				if(oldOnLeave)
					(m_CallbackClass->*oldOnLeave)(newState);
				pState->CurrState = newState;
				pState->OnState = newOnState;
				if(newOnEnter)
					(m_CallbackClass->*newOnEnter)(oldState);
			}
			return;
		}
	}

	template<class T> 
	void PFSMController<T>::addInaccessible(uint32 fsm, uint32 stateFrom, uint32 stateTo,
		RefuseCommand onRefuse)
	{
		//��������Ƿ��Ѿ�����
		std::list<InAccessible>::iterator it = m_InAccessible.begin();
		for(;it!=m_InAccessible.end();++it)
		{
			if(fsm == it->Sheet &&
				stateFrom == it->StateFrom &&
				stateTo == it->StateTo)
				return;
		}

		InAccessible tmp;
		tmp.Sheet = fsm;
		tmp.StateFrom = stateFrom;
		tmp.StateTo = stateTo;
		tmp.OnRefuse = onRefuse;

		m_InAccessible.push_back(tmp);
	}

	template<class T> 
	void PFSMController<T>::deleteInaccessible(uint32 fsm, uint32 stateFrom, uint32 stateTo)
	{
		//ȷ�������Ѿ�������
		std::list<InAccessible>::iterator it = m_InAccessible.begin();
		for(;it!=m_InAccessible.end();++it)
		{
			if(fsm == it->Sheet &&
				stateFrom == it->StateFrom &&
				stateTo == it->StateTo)
			{
				m_InAccessible.erase(it);
				return;
			}
		}
	}



	template<class T> 
	void PFSMController<T>::addInaccessible(uint32 fsm, RefuseCommand onRefuse)
	{
		//��������Ƿ��Ѿ�����
		std::list<InAccessible>::iterator it = m_InAccessible.begin();
		for(;it!=m_InAccessible.end();++it)
		{
			if(fsm == it->Sheet &&
				ALL_STATE == it->StateFrom &&
				ALL_STATE == it->StateTo)
				return;
		}

		InAccessible tmp;
		tmp.Sheet = fsm;
		tmp.StateFrom = ALL_STATE;
		tmp.StateTo = ALL_STATE;
		tmp.OnRefuse = onRefuse;

		m_InAccessible.push_back(tmp);
	}

	template<class T> 
	void PFSMController<T>::deleteInaccessible(uint32 fsm)
	{
		//ȷ�������Ѿ�������
		std::list<InAccessible>::iterator it = m_InAccessible.begin();
		for(;it!=m_InAccessible.end();++it)
		{
			if(fsm == it->Sheet &&
				ALL_STATE == it->StateFrom &&
				ALL_STATE == it->StateTo)
			{
				m_InAccessible.erase(it);
				return;
			}
		}
	}

	template<class T> 
	void PFSMController<T>::deleteFSM(uint32 fsm)
	{
		//ȷ���Ѿ�������
		std::map<uint32, PState>::iterator it = m_Cont.find(fsm);
		if(it!=m_Cont.end())
		{	
			m_Cont.erase(it);
			return;
		}
	}

	template<class T> 
	void PFSMController<T>::deleteState(uint32 fsm, uint32 state)
	{
		std::map<uint32, PState>::iterator it = m_Cont.find(fsm);
		if(it!=m_Cont.end())
		{
			PState* pState = &(it->second);
			ASheet::iterator it1 = pState->Sheet.begin();
			for(;it1!=pState->Sheet.end();++it1)
			{
				if(state == it1->state)
				{
					pState->Sheet.erase(it1);
					return;
				}
			}
		}
	}

	template<class T>
	void PFSMController<T>::update(float timeSinceLastFrame)
	{
		std::map<uint32, PState>::iterator it = m_Cont.begin();
		for(;it!=m_Cont.end();++it)
		{
			PState* pState = &(it->second);
			if(pState->OnState)
			{
				(m_CallbackClass->*(pState->OnState))(timeSinceLastFrame);
			}
		}
	}

	template<class T>
	uint32 PFSMController<T>::getCurrentState(uint32 fsm)
	{
		std::map<uint32, PState>::iterator it = m_Cont.find(fsm);
		if(it == m_Cont.end())
			return 0xFFFFFFFF;

		PState* pState = &(it->second);
		return pState->CurrState;
	}

};