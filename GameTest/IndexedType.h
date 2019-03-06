#pragma once
#include "stdafx.h"
template<typename T>
struct CIndexedType {
	float operator[](unsigned int index) const;
	//See if I can figure out a way to automatically increment the sentinel for each unique type (static and T help with this, but the increment idk).
	//static const unsigned int s_sentinel;
	//This also means we can generalize the initialized() method.

	//Could also generalize the memcopy which isn't in use.
	//template<typename U>
	//void copyValues(U);
};

//template<typename T>
//const unsigned int CIndexedType<T>::s_sentinel;

template<typename T>
inline float CIndexedType<T>::operator[](unsigned int index) const
{
#if _DEBUG
	assert(index < s_components);
#endif
	return values[index];
}
