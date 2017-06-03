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
#include "Invader.h"

// Static Variables

// Static Function Prototypes

// Implementation

CInvader::CInvader()
	: m_fMoveSpeedX(30.0f), m_fMoveStepY(21.0f)
{

}

CInvader::~CInvader()
{

}

bool
CInvader::Initialise()
{
	VALIDATE(CEntity::Initialise(IDB_BITMAP1, IDB_BITMAP2));

	return (true);
}

void
CInvader::Draw()
{
	CEntity::Draw();
}

void
CInvader::Process(float _fDeltaTick)
{
	m_fX += static_cast<float>(m_fMoveSpeedX) * _fDeltaTick;
	CEntity::Process(_fDeltaTick);
}

float 
CInvader::MoveSpeedX()
{
	return m_fMoveSpeedX;
}

void
CInvader::SetMoveSpeedX(float newVal)
{
	m_fMoveSpeedX = newVal;
}

float
CInvader::MoveStepY()
{
	return m_fMoveStepY;
}

void
CInvader::SetMoveStepY(float newVal)
{
	m_fMoveStepY = newVal;
}