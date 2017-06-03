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

#if !defined(__PLAYER_H__)
#define __PLAYER_H__

// Library Includes

// Local Includes
#include "entity.h"
#include "Sprite.h"

// Types

// Constants

// Prototypes
class CPlayer : public CEntity
{
	// Member Functions
public:
	CPlayer();
	virtual ~CPlayer();

	virtual bool Initialise();

	virtual void Draw();
	virtual void Process(float _fDeltaTick);

	bool CannonOffCooldown();
	void setCannonOnCooldown();

protected:

private:
	CPlayer(const CPlayer& _kr);
	CPlayer& operator= (const CPlayer& _kr);

	// Member Variables
public:

protected:
	float m_fMoveSpeed;

	bool m_bFireOnCooldown;
	float m_fBulletCooldown;
	float m_fBulletCooldownTimer;

private:

};

#endif    // __PLAYER_H__
