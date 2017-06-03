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
#include "Player.h"

// Static Variables

// Static Function Prototypes

// Implementation

CPlayer::CPlayer()
	: m_fMoveSpeed(100)
	, m_bFireOnCooldown(false)
	, m_fBulletCooldown(0.5f)
	, m_fBulletCooldownTimer(0)
{

}

CPlayer::~CPlayer()
{

}

bool
CPlayer::Initialise()
{
	VALIDATE(CEntity::Initialise(IDB_PADDLESPRITE, IDB_PADDLEMASK));

	return (true);
}

void
CPlayer::Draw()
{
	CEntity::Draw();
}

void
CPlayer::Process(float _fDeltaTick)
{
	//Process bullets

	//Process cooldown
	if (m_bFireOnCooldown)
	{ 
		m_fBulletCooldownTimer += _fDeltaTick;
		if (m_fBulletCooldownTimer >= m_fBulletCooldown)
		{	//Finished cooling down
			m_bFireOnCooldown = false;
			m_fBulletCooldownTimer = 0;
		}
	}

	//Process movement
	float fHalfPaddleW = m_pSprite->GetWidth() / 2.0;

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_fX += m_fMoveSpeed * _fDeltaTick;
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_fX -= m_fMoveSpeed * _fDeltaTick;
	}

	if (m_fX - fHalfPaddleW <= 0)
	{
		m_fX = fHalfPaddleW;
	}
	else if (m_fX + fHalfPaddleW >= 405)
	{
		m_fX = 405 - fHalfPaddleW;
	}

	//Process other
	CEntity::Process(_fDeltaTick);
}

bool CPlayer::CannonOffCooldown() { return !m_bFireOnCooldown; }
void CPlayer::setCannonOnCooldown() { m_bFireOnCooldown = true; }