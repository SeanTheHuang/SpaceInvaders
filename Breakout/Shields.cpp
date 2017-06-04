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
#include "Shields.h"

// Static Variables

// Static Function Prototypes

// Implementation

CShield::CShield()
	: m_iCurrentLives(8)
{

}

CShield::~CShield()
{

}

bool
CShield::Initialise()
{
	VALIDATE(CEntity::Initialise(IDB_BITMAP6, IDB_BITMAP5));

	return (true);
}

void
CShield::Draw()
{

	CEntity::Draw();
}

void
CShield::Process(float _fDeltaTick)
{

	CEntity::Process(_fDeltaTick);

}

int
CShield::CurrentLivesLeft()
{
	return m_iCurrentLives;
}

void
CShield::ShieldWasHit()
{
	m_iCurrentLives--;
}