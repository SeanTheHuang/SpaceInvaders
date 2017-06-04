//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2016 Media Design School
//
// File Name	: 
// Description	: 
// Author		: Your Name
// Mail			: your.name@mediadesign.school.nz
//

#pragma once

#if !defined(__SHIELD_H_)
#define __SHIELD_H_

// Library Includes

// Local Includes
#include "entity.h"

// Types

// Constants

// Prototypes

class CShield : public CEntity
{
	// Member Functions
public:
	CShield();
	virtual ~CShield();

	virtual bool Initialise();

	virtual void Draw();
	virtual void Process(float _fDeltaTick);

	int CurrentLivesLeft();
	void ShieldWasHit();
protected:

private:
	CShield(const CShield& _kr);
	CShield& operator= (const CShield& _kr);

	// Member Variables
public:

protected:
	int m_iCurrentLives;

private:

};


#endif    // __SHIELD_H_

