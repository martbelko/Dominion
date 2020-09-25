#include "dmpch.h"
#include "String.h"

String::String(const char* string)
	: m_String(string)
{
}

String::String(const String& string)
	: m_String(string.m_String)
{
}

String::String(const std::string& string)
	: m_String(string)
{
}

String& String::operator=(const String& string)
{
	m_String = string.m_String;
	return *this;
}

String& String::operator=(const char* string)
{
	m_String = string;
	return *this;
}

String String::operator+(const String& rhs)
{
	return String(m_String + rhs.m_String);
}

String& String::operator+=(const String& rhs)
{
	return Append(rhs);
}

String& String::operator+=(const char* rhs)
{
	return Append(rhs);
}

bool String::operator==(const String& rhs)
{
	return m_String == rhs.m_String;
}

bool String::operator!=(const String& rhs)
{
	return m_String != rhs.m_String;
}

bool String::operator<(const String& rhs)
{
	return m_String < rhs.m_String;
}

bool String::operator<=(const String& rhs)
{
	return m_String <= rhs.m_String;
}

bool String::operator>(const String& rhs)
{
	return m_String > rhs.m_String;
}

bool String::operator>=(const String& rhs)
{
	return m_String >= rhs.m_String;
}

char& String::operator[](size_t index)
{
	return m_String[index];
}

char& String::At(size_t index)
{
	return m_String[index];
}

String& String::Append(const char* string)
{
	m_String.append(string);
	return *this;
}

String& String::Append(const String& string)
{
	m_String.append(string.m_String);
	return *this;
}

const char* String::Cstr()
{
	return m_String.c_str();
}

size_t String::Length() const
{
	return m_String.length();
}

size_t String::Size() const
{
	return m_String.size();
}

String::Iterator String::begin()
{
	return Iterator(m_String.data());
}

String::Iterator String::end()
{
	return Iterator(m_String.data() + m_String.length());
}

std::ostream& operator<<(std::ostream& os, const String& string)
{
	os << string.m_String;
	return os;
}

std::istream& operator>>(std::istream& is, String& string)
{
	is >> string.m_String;
	return is;
}

WString::WString(const wchar_t* string)
	: m_String(string)
{
}

WString::WString(const WString& string)
	: m_String(string.m_String)
{
}

WString::WString(const std::wstring& string)
	: m_String(string)
{
}

WString& WString::operator=(const WString& string)
{
	m_String = string.m_String;
	return *this;
}

WString& WString::operator=(const wchar_t* string)
{
	m_String = string;
	return *this;
}

WString WString::operator+(const WString& rhs)
{
	return WString(m_String + rhs.m_String);
}

WString& WString::operator+=(const wchar_t* rhs)
{
	m_String += rhs;
	return *this;
}

WString& WString::operator+=(const WString& rhs)
{
	m_String += rhs.m_String;
	return *this;
}

bool WString::operator<(const WString& rhs)
{
	return m_String < rhs.m_String;
}

bool WString::operator<=(const WString& rhs)
{
	return m_String <= rhs.m_String;
}

bool WString::operator>(const WString& rhs)
{
	return m_String > rhs.m_String;
}

bool WString::operator>=(const WString& rhs)
{
	return m_String >= rhs.m_String;
}

wchar_t& WString::operator[](size_t index)
{
	return m_String[index];
}

wchar_t& WString::At(size_t index)
{
	return m_String.at(index);
}

bool WString::operator!=(const WString& rhs)
{
	return m_String != rhs.m_String;
}

bool WString::operator==(const WString& rhs)
{
	return m_String == rhs.m_String;
}

WString& WString::Append(const wchar_t* string)
{
	m_String.append(string);
	return *this;
}

WString& WString::Append(const WString& string)
{
	m_String.append(string.m_String);
	return *this;
}

const wchar_t* WString::Cstr()
{
	return m_String.c_str();
}

size_t WString::Length() const
{
	return m_String.length();
}

size_t WString::Size() const
{
	return m_String.size();
}

WString::Iterator WString::begin()
{
	return _Iterator<wchar_t>(m_String.data());
}

WString::Iterator WString::end()
{
	return _Iterator<wchar_t>(m_String.data() + m_String.length());
}

std::wostream& operator<<(std::wostream& os, const WString& string)
{
	os << string.m_String;
	return os;
}

std::wistream& operator>>(std::wistream& is, WString& string)
{
	is >> string.m_String;
	return is;
}
