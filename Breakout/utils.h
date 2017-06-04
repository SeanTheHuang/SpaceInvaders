//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// File Name	: 
// Description	: 
// Author		: Sean Huang
// Mail			: sean.hua7281@mediadesign.school.nz
//

#pragma once

#if !defined(__UTILS_H__)
#define __UTILS_H__

// Library Includes
#include <Windows.h>
#include <strstream>
#include <sstream>
#include <string>


// Local Includes

// Types

// Constants

// Prototypes

#define VALIDATE(a) if (!a) return (false)

template<typename T>
std::string ToString(const T& _value)
{
	std::strstream theStream;
	theStream << _value << std::ends;
	return (theStream.str());
}

#endif    // __UTILS_H__

