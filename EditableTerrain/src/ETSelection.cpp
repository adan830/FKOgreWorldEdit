#include "ETPrerequisites.h"
#include "ETSelection.h"
namespace Ogre
{
	Selection::Selection(ETTerrain *terrain)
		: mPosition(0.0f , 0.0f)
		, mMin(65535 , 65535)
		, mMax(0 , 0)
		, m_terrain(terrain)
	{
	}

	Selection::~Selection(void)
	{
	}

	void Selection::clear()
	{
				
		// add by ������ 2007-7-27
		// �ص���������
		for(VTSelectionListener::iterator iter = mVTSelectionListener.begin() ; iter != mVTSelectionListener.end() ; iter ++)
		{
			(*iter)->clearPre();
		}
		MapSelection::clear();
		mMin.setX(65535);
		mMin.setY(65535);
		mMax.setX(0);
		mMax.setY(0);
		// add by ������ 2007-8-10
		// ��������λ��
		//mPosition.setX(0);
		//mPosition.setY(0);
		
		// add by ������ 2007-7-27
		// �ص���������
		for(VTSelectionListener::iterator iter = mVTSelectionListener.begin() ; iter != mVTSelectionListener.end() ; iter ++)
		{
			(*iter)->clearPost();
		}
	}

	void Selection::addSel(ushort x, ushort z, const Brush &brush, float intensity)
	{

		// add by ������ 2007-7-27
		// �ص���������
		for(VTSelectionListener::iterator iter = mVTSelectionListener.begin() ; iter != mVTSelectionListener.end() ; iter ++)
		{
			(*iter)->addSelPre(x , z , brush , intensity);
		}


		// ��ñ�ˢ��ʼλ��
		x -= (ushort)brush.getWidth()/2;
		z -= (ushort)brush.getHeight()/2;

		// ������ˢ��ÿ����,�Ӷ����ÿ����� x����ֵ,z����ֵ,Ȩ��ֵ ��󱣴���mSeletedPoint��
		for (size_t i = 0; i < brush.getWidth(); ++i)
		{
			ushort posX = x + (ushort)i;
			for (size_t j = 0; j < brush.getHeight(); ++j)
			{
				ushort posZ = z + (ushort)j;
				
				// add by ������ 2007-7-27
				// ������С���������
				mMin.setX(std::min(mMin.getX() , posX));
				mMin.setY(std::min(mMin.getY() , posZ));
				
				mMax.setX(std::max(mMax.getX() , posX));
				mMax.setY(std::max(mMax.getY() , posZ));

				// �����ظ��ĵ�,����ظ��򲻼���
				MapSelection::iterator it = find(GridVector2(posX,posZ));
				if(it == end())
				{
					insert(MapSelection::value_type(GridVector2(posX,posZ)
						, brush.at(i,j) * intensity));
				}
				else
				{
					it->second += brush.at(i,j) * intensity;
					// add by ������ 2007-7-23
					// ���1
					if(it->second > 1.0f)
					{
						it->second = 1.0f;
					}
					else if(it->second < -1.0f)
					{
						it->second = -1.0f;
					}
				}
			}
		}

		
		// add by ������ 2007-7-27
		// �ص���������
		for(VTSelectionListener::iterator iter = mVTSelectionListener.begin() ; iter != mVTSelectionListener.end() ; iter ++)
		{
			(*iter)->addSelPost(x , z , brush , intensity);
		}
	}

	void Selection::clearSel(ushort x, ushort z, const Brush &brush)
	{
		
		// add by ������ 2007-7-27
		// �ص���������
		for(VTSelectionListener::iterator iter = mVTSelectionListener.begin() ; iter != mVTSelectionListener.end() ; iter ++)
		{
			(*iter)->clearSelPre(x , z , brush);
		}
		// ��ñ�ˢ��ʼλ��
		x -= (ushort)brush.getWidth()/2;
		z -= (ushort)brush.getHeight()/2;

		// ������ˢ��ÿ����
		for (size_t i = 0; i < brush.getWidth(); ++i)
		{
			int posX = x + (ushort)i;
		
			for (size_t j = 0; j < brush.getHeight(); ++j)
			{
				int posZ = z + (ushort)j;
				
				// ���ұ�ˢ�غϵĵ�,����غ����mSeletedPoint�����
				MapSelection::iterator it = find(GridVector2(posX,posZ));
				if(it != end())
				{
					erase(it);
				}
			}
		}
		// add by ������ 2007-7-27
		// ������С�������
		updateMinMax();

				
		// add by ������ 2007-7-27
		// �ص���������
		for(VTSelectionListener::iterator iter = mVTSelectionListener.begin() ; iter != mVTSelectionListener.end() ; iter ++)
		{
			(*iter)->clearSelPost(x , z , brush);
		}
	}

	void Selection::updateMinMax()
	{
		mMin.setX(65535);
		mMin.setY(65535);
		mMax.setX(0);
		mMax.setY(0);
		for(iterator iter = begin() ; iter != end() ; iter ++)
		{
			// add by ������ 2007-7-27
			// ������С���������
			mMin.setX(std::min(mMin.getX() , iter->first.getX()));
			mMin.setY(std::min(mMin.getY()  , iter->first.getY()));
			
			mMax.setX(std::max(mMax.getX()  , iter->first.getX()));
			mMax.setY(std::max(mMax.getY()  , iter->first.getY()));
		}
	}
}