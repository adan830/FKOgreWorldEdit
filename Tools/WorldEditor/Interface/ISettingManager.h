// ============================================================
// �ļ���	:	ISettingManager.h
// ������	:	������
// ˵��		:	���ù�����
// ����ʱ��	:	2008-1-26 9:56:54
// ============================================================

#pragma once

namespace Ogre
{
	// ���ù�����
	class ISettingManager
	{
	public:

		ISettingManager(void)
		{
		}

		virtual ~ISettingManager(void)
		{
		}


		// ��ʾ���öԻ���
		virtual void showSettingDlg() = 0;

		// ���ϱ�������
		virtual void flush() = 0;

		// ע    �� : getϵ�к����ǻ�ȡ���õĺ��������ָ���������ֶ��������ڣ�����Ĭ��ֵ���֮
		// �������� : ���õ�����
		// ��    �� : name			������
		// ��    �� : defaultValue	Ĭ��ֵ
		// ��    �� : group			������

		virtual String getString(const String& name, const String& defaultValue = StringUtil::BLANK, const String& group = StringUtil::BLANK) = 0;
		virtual void getBinary(const String& name, uchar** ppData, uint* pBytes, const String& group = StringUtil::BLANK) = 0;

		virtual bool getBool(const String& name, bool defaultValue = false, const String& group = StringUtil::BLANK) = 0;
		virtual double getDouble(const String& name, double defaultValue = 0.0, const String& group = StringUtil::BLANK) = 0;
		virtual float getFloat(const String& name, float defaultValue = 0.0f, const String& group = StringUtil::BLANK) = 0;

		virtual char getChar(const String& name, char defaultValue = 0, const String& group = StringUtil::BLANK) = 0;
		virtual short getShort(const String& name, short defaultValue = 0, const String& group = StringUtil::BLANK) = 0;
		virtual int getInt(const String& name, int defaultValue = 0, const String& group = StringUtil::BLANK) = 0;
		virtual long getLong(const String& name, long defaultValue = 0, const String& group = StringUtil::BLANK) = 0;
		virtual __int64 getInt64(const String& name, __int64 defaultValue = 0, const String& group = StringUtil::BLANK) = 0;

		virtual uchar getUChar(const String& name, uchar defaultValue = 0, const String& group = StringUtil::BLANK) = 0;
		virtual ushort getUShort(const String& name, ushort defaultValue = 0, const String& group = StringUtil::BLANK) = 0;
		virtual uint getUInt(const String& name, uint defaultValue = 0, const String& group = StringUtil::BLANK) = 0;
		virtual ulong getULong(const String& name, ulong defaultValue = 0, const String& group = StringUtil::BLANK) = 0;

		virtual Vector2 getVector2(const String& name, const Vector2& defaultValue = Vector2::ZERO, const String& group = StringUtil::BLANK) = 0;
		virtual Vector3 getVector3(const String& name, const Vector3& defaultValue = Vector3::ZERO, const String& group = StringUtil::BLANK) = 0;
		virtual ColourValue getColourValue(const String& name, const ColourValue& defaultValue = ColourValue::White, const String& group = StringUtil::BLANK) = 0;
		virtual Matrix3 getMatrix3(const String& name, const Matrix3& defaultValue = Matrix3::IDENTITY, const String& group = StringUtil::BLANK) = 0;
		virtual Matrix4 getMatrix4(const String& name, const Matrix4& defaultValue = Matrix4::IDENTITY, const String& group = StringUtil::BLANK) = 0;
		virtual Quaternion getQuaternion(const String& name, const Quaternion& defaultValue = Quaternion::IDENTITY, const String& group = StringUtil::BLANK) = 0;
		virtual Radian getRadian(const String& name, const Radian &defaultValue = Radian(0.0f), const String& group = StringUtil::BLANK) = 0;
		virtual Degree getDegree(const String& name, const Degree &defaultValue = Degree(0.0f), const String& group = StringUtil::BLANK) = 0;
		virtual StringVector getStringVector(const String& name, const StringVector &defaultValue = StringVector(), const String& group = StringUtil::BLANK) = 0;

		// ö���������õ�����
		virtual StringVector enumSettingName(const String& group = StringUtil::BLANK) = 0;


		// ע    �� : setϵ�к����Ǳ������õĺ���
		// ��    �� : name		������
		// ��    �� : value			��������
		// ��    �� : group	������

		virtual void setString(const String& name, const String& value, const String& group = StringUtil::BLANK) = 0;
		virtual void setBinary(const String& name, uchar* data, uint size, const String& group = StringUtil::BLANK) = 0;

		virtual void setBool(const String& name, bool value, const String& group = StringUtil::BLANK) = 0;
		virtual void setDouble(const String& name, double value, const String& group = StringUtil::BLANK) = 0;
		virtual void setFloat(const String& name, float value, const String& group = StringUtil::BLANK) = 0;

		virtual void setChar(const String& name, char value, const String& group = StringUtil::BLANK) = 0;
		virtual void setShort(const String& name, short value, const String& group = StringUtil::BLANK) = 0;
		virtual void setInt(const String& name, int value, const String& group = StringUtil::BLANK) = 0;
		virtual void setLong(const String& name, long value, const String& group = StringUtil::BLANK) = 0;
		virtual void setInt64(const String& name, __int64 value, const String& group = StringUtil::BLANK) = 0;

		virtual void setUChar(const String& name, uchar value, const String& group = StringUtil::BLANK) = 0;
		virtual void setUShort(const String& name, ushort value, const String& group = StringUtil::BLANK) = 0;
		virtual void setUInt(const String& name, uint value, const String& group = StringUtil::BLANK) = 0;
		virtual void setULong(const String& name, ulong value, const String& group = StringUtil::BLANK) = 0;

		virtual void setVector2(const String& name, const Vector2& value , const String& group = StringUtil::BLANK) = 0;
		virtual void setVector3(const String& name, const Vector3& value , const String& group = StringUtil::BLANK) = 0;
		virtual void setColourValue(const String& name, const ColourValue& value , const String& group = StringUtil::BLANK) = 0;
		virtual void setMatrix3(const String& name, const Matrix3& value , const String& group = StringUtil::BLANK) = 0;
		virtual void setMatrix4(const String& name, const Matrix4& value , const String& group = StringUtil::BLANK) = 0;
		virtual void setQuaternion(const String& name, const Quaternion& value , const String& group = StringUtil::BLANK) = 0;
		virtual void setRadian(const String& name, const Radian &value , const String& group = StringUtil::BLANK) = 0;
		virtual void setDegree(const String& name, const Degree &value , const String& group = StringUtil::BLANK) = 0;
		virtual void setStringVector(const String& name, const StringVector &value , const String& group = StringUtil::BLANK) = 0;
	};
};