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

#if !defined(__INVADER_H__)
#define __INVADER_H__

// Library Includes

// Local Includes
#include "entity.h"

// Types

// Constants

// Prototypes

class CInvader : public CEntity
{
	// Member Functions
public:
	CInvader();
	virtual ~CInvader();

	virtual bool Initialise();

	virtual void Draw();
	virtual void Process(float _fDeltaTick);


	float MoveSpeedX();
	void SetMoveSpeedX(float);

	float MoveStepY();
	void SetMoveStepY(float);

protected:

private:
	CInvader(const CInvader& _kr);
	CInvader& operator= (const CInvader& _kr);

	// Member Variables
public:

protected:
	float m_fMoveSpeedX;
	float m_fMoveStepY;
private:

};


#endif    // __INVADER_H__

