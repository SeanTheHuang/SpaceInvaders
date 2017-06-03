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

#if !defined(__BULLET_H__)
#define __BULLET_H__

// Library Includes

// Local Includes
#include "entity.h"
#include "Sprite.h"

// Types
enum BULLET_OWNER
{
	NO_OWNER = -1,
	PLAYER,
	INVADER
};

// Constants

// Prototypes
class CBullet : public CEntity
{
	// Member Functions
public:
	CBullet();
	virtual ~CBullet();

	virtual bool Initialise();

	virtual void Draw();
	virtual void Process(float _fDeltaTick);

	void setBulletOwner(BULLET_OWNER);

protected:

private:
	CBullet(const CBullet& _kr);
	CBullet& operator= (const CBullet& _kr);

	// Member Variables
public:

protected:
	float m_fMoveSpeed;
	BULLET_OWNER m_eWhoFiredBullet;

private:

};

#endif    // __BULLET_H__
