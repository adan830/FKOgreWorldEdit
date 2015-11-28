#pragma once

namespace Ogre
{

	// ��ǿ��FileHandleDataStream
	class OGREPRO_API FileHandleDataStreamEx : public FileHandleDataStream
	{
	public:
		/// Create stream from a C file handle
		FileHandleDataStreamEx(FILE* handle , bool autoCloseFile);
		/// Create named stream from a C file handle
		FileHandleDataStreamEx(const String& name, FILE* handle , bool autoCloseFile);
		~FileHandleDataStreamEx();
	protected:
		// �Զ��ر��ļ�
		bool m_autoCloseFile;
	};
};