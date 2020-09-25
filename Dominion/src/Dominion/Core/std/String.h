#pragma once

#include <iostream>
#include <string>

template <typename T>
class _Iterator
{
public:
	_Iterator(T* ptr)
		: m_Ptr(ptr)
	{
	}

	_Iterator& operator++()
	{
		++m_Ptr;
		return *this;
	}

	_Iterator& operator++(int)
	{
		_Iterator<T> iterator = *this;
		++m_Ptr;
		return iterator;
	}

	_Iterator& operator--()
	{
		--m_Ptr;
		return *this;
	}

	_Iterator& operator--(int)
	{
		_Iterator iterator = *this;
		--m_Ptr;
		return iterator;
	}

	T& operator[](size_t index)
	{
		return *(m_Ptr + index);
	}

	T* operator->()
	{
		return m_Ptr;
	}

	T& operator*()
	{
		return *m_Ptr;
	}

	bool operator==(const _Iterator& other) const
	{
		return m_Ptr == other.m_Ptr;
	}

	bool operator!=(const _Iterator& other) const
	{
		return m_Ptr != other.m_Ptr;
	}
private:
	T* m_Ptr;
};

class String
{
public:
	using Iterator = _Iterator<char>;
public:
	String() = default;
	String(const char* string);
	String(const String& string);
	~String() = default;

	String& operator=(const char* string);
	String& operator=(const String& string);

	String operator+(const String& rhs);
	String& operator+=(const char* rhs);
	String& operator+=(const String& rhs);

	bool operator==(const String& rhs);
	bool operator!=(const String& rhs);
	bool operator<(const String& rhs);
	bool operator<=(const String& rhs);
	bool operator>(const String& rhs);
	bool operator>=(const String& rhs);

	char& operator[](size_t index);
	char& At(size_t index);

	String& Append(const char* string);
	String& Append(const String& string);
	const char* Cstr();
	size_t Length() const;
	size_t Size() const;

	Iterator begin();
	Iterator end();

	friend std::ostream& operator<<(std::ostream& os, const String& string);
	friend std::istream& operator>>(std::istream& is, String& string);
private:
	String(const std::string& string);
private:
	std::string m_String;
};

class WString
{
public:
	using Iterator = _Iterator<wchar_t>;
public:
	WString() = default;
	WString(const wchar_t* string);
	WString(const WString& string);

	~WString() = default;

	WString& operator=(const wchar_t* string);
	WString& operator=(const WString& string);

	WString operator+(const WString& rhs);
	WString& operator+=(const wchar_t* rhs);
	WString& operator+=(const WString& rhs);

	bool operator==(const WString& rhs);
	bool operator!=(const WString& rhs);
	bool operator<(const WString& rhs);
	bool operator<=(const WString& rhs);
	bool operator>(const WString& rhs);
	bool operator>=(const WString& rhs);

	wchar_t& operator[](size_t index);
	wchar_t& At(size_t index);

	WString& Append(const wchar_t* string);
	WString& Append(const WString& string);

	const wchar_t* Cstr();
	size_t Length() const;
	size_t Size() const;

	Iterator begin();
	Iterator end();

	friend std::wostream& operator<<(std::wostream& os, const WString& string);
	friend std::wistream& operator>>(std::wistream& is, WString& string);
private:
	WString(const std::wstring& string);
private:
	std::wstring m_String;
};
