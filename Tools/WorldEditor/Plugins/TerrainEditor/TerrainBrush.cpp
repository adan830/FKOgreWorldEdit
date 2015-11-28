#include "stdafx.h"
#include "TerrainBrush.h"
#include "ETBrush.h"
namespace Ogre
{

	TerrainBrush::TerrainBrush()
		: mBrush(0)
		, m_strength(1.0f)
		, m_size(10)
		, m_image(0)
	{
	};

	TerrainBrush::~TerrainBrush()
	{
		clear();
		OGRE_DELETE(m_image);
	};

	void TerrainBrush::clear()
	{
		OGRE_DELETE(mBrush);
	};

	// ��һ���Ҷ�ͼ�����ˢ
	bool TerrainBrush::loadBrushFromImage(const Ogre::Image& image)
	{
		if(image.getSize() == 0)
			return false;
		clear();
		mBrush = new Brush(Ogre::loadBrushFromImage(image));


		// �ص���������
		ExecuteListener(TerrainBrushListener , updateBrush);

		return true;
	}

	/** Saves a brush to a grayscale image */
	void TerrainBrush::saveBrushToImage(Ogre::Image& image)
	{
		if(!mBrush)
			return;
		Ogre::saveBrushToImage(*mBrush , image);
	}


	// �����ļ���ȡ��ˢ
	bool TerrainBrush::loadBrushFromFile(const String &fileName)
	{
		if(m_brushFileName == fileName && m_image && m_image->getWidth() == m_size && m_image->getHeight() == m_size)
			return true;
		m_brushFileName = fileName;
		OGRE_DELETE(m_image);
		m_image = new Image;
		try
		{
			m_image->load(m_brushFileName , "General");

			// ���ı�ˢ��С
			m_image->resize(m_size , m_size);
			// ֱ�Ӷ�ȡͼƬ����ˢ
			return loadBrushFromImage(*m_image);
		}
		catch(Exception&e)
		{
			e.getLine();
			return false;
		}
		return false;
	}


	// ���ݵ�ǰ��ˢ��ͼ��ı��С
	void TerrainBrush::setSize(int size)
	{
		m_size = size;
		// �ص���������
		ExecuteListener(TerrainBrushListener , updateSize);
		if(!m_brushFileName.empty())
		{
			loadBrushFromFile(m_brushFileName);
		}
	}
	void TerrainBrush::setStrength(const float &fStrength)
	{
		m_strength = fStrength;
		// �ص���������
		ExecuteListener(TerrainBrushListener , updateStrength);
	}
	size_t TerrainBrush::getWidth() const
	{
		return mBrush->getWidth();
	}
	size_t TerrainBrush::getHeight() const
	{
		return mBrush->getHeight();
	}

};