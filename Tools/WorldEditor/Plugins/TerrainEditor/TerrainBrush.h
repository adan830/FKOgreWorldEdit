#pragma once

#include "TerrainEditorDll.h"
#include "TListener.h"

namespace Ogre
{
	class Brush;

	// ��ˢ������
	struct TerrainBrushListener
	{
		virtual void updateBrush(){};
		virtual void updateSize(){};
		virtual void updateStrength(){};
	};

	// ���α�ˢʵ��
	class TERRAINEDITOR_API TerrainBrush : public TListener<TerrainBrushListener>
	{
	public:
		TerrainBrush();

		virtual ~TerrainBrush();

		void clear();

		// ��һ���Ҷ�ͼ�����ˢ
		bool loadBrushFromImage(const Ogre::Image& image);
		// �����ˢ���Ҷ�ͼ
		void saveBrushToImage(Ogre::Image& image);

		// �����ļ���ȡ��ˢ
		bool loadBrushFromFile(const String &fileName);


		// ���ݵ�ǰ��ˢ��ͼ��ı��С
		void setSize(int size);

		void setStrength(const float &fStrength);

		size_t getWidth() const;
		size_t getHeight() const;

		const float &getStrength()const{return m_strength;}
		const float &getSize()const{return m_size;}
		const Brush *getBrush()const{return mBrush;};


		Image *getImage()
		{
			return m_image;
		}
protected:

		Brush *mBrush;
		float m_strength;
		float m_size;
		Image *m_image;

		String m_brushFileName;
	};
};