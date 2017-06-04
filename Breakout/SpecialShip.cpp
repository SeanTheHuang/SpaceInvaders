//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2016 Media Design School.
//
// File Name	: 
// Description	: 
// Author		: Your Name
// Mail			: your.name@mediadesign.school.nz
//

// Library Includes

// Local Includes
#include "resource.h"
#include "utils.h"

// This Include
#include "SpecialShip.h"

// Static Variables

// Static Function Prototypes

// Implementation

CSpecialShip::CSpecialShip()
	: moveSpeed(100.0f)
{

}

CSpecialShip::~CSpecialShip()
{

}

bool
CSpecialShip::Initialise()
{
	VALIDATE(CEntity::Initialise(IDB_BITMAP7, IDB_BITMAP8));

	return (true);
}

void
CSpecialShip::Draw()
{
		CEntity::Draw();
}

void
CSpecialShip::Process(float _fDeltaTick)
{
	m_fX += moveSpeed * _fDeltaTick;
	CEntity::Process(_fDeltaTick);
}

