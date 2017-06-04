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

#if !defined(__SPECIAL_SHIP_H__)
#define __SPECIAL_SHIP_H__

// Library Includes

// Local Includes
#include "entity.h"

// Types

// Constants

// Prototypes

class CSpecialShip : public CEntity
{
	// Member Functions
public:
	CSpecialShip();
	virtual ~CSpecialShip();

	virtual bool Initialise();

	virtual void Draw();
	virtual void Process(float _fDeltaTick);


protected:

private:
	CSpecialShip(const CSpecialShip& _kr);
	CSpecialShip& operator= (const CSpecialShip& _kr);

	// Member Variables
public:

protected:
	float moveSpeed;
private:

};


#endif    // __SPECIAL_SHIP_H__

