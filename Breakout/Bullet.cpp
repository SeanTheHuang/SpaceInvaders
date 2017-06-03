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
#include "Bullet.h"

// Static Variables

// Static Function Prototypes

// Implementation

CBullet::CBullet()
	: m_fMoveSpeed(-50)
	, m_eWhoFiredBullet(PLAYER)
{

}

CBullet::~CBullet()
{

}

bool
CBullet::Initialise()
{
	VALIDATE(CEntity::Initialise(IDB_BITMAP4, IDB_BITMAP3));

	return (true);
}

void
CBullet::Draw()
{
	CEntity::Draw();
}

void
CBullet::Process(float _fDeltaTick)
{
	m_fY += m_fMoveSpeed * _fDeltaTick;

	//Process other
	CEntity::Process(_fDeltaTick);
}

void
CBullet::setBulletOwner(BULLET_OWNER owner)
{
	m_eWhoFiredBullet = owner;
}