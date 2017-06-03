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

#if !defined(__LEVEL_H__)
#define __LEVEL_H__

// Library Includes
#include <vector>
#include <string>

// Local Includes

// Types

// Constants

// Prototypes
class CBall;
class CPlayer;
class CInvader;
class CFPSCounter;
class CBackGround;
class CBullet;

class CLevel
{
    // Member Functions
public:
    CLevel();
    virtual ~CLevel();

    virtual bool Initialise(int _iWidth, int _iHeight);

    virtual void Draw();
    virtual void Process(float _fDeltaTick);

protected:
	void MoveInvadersDown();
	void ProcessInvadersInBounds();
	bool ProcessPlayerFiringBullet();

	void ProcessBulletsHitInvader();
	void ProcessBulletsHitPlayer();
	void ProcessBulletsHitShield();

    void UpdateScoreText();
    void DrawScore();
	void DrawFPS();

private:
    CLevel(const CLevel& _kr);
    CLevel& operator= (const CLevel& _kr);

    // Member Variables
public:

protected:
	CPlayer* m_pPlayer;

	std::vector<CInvader*> m_vecInvaders;
	std::vector<CBullet*> m_vecBullets;
	

	CFPSCounter* m_fpsCounter;

    int m_iWidth;
    int m_iHeight;

	int m_iInvaderMoveSpeed;
	int m_iInvaderDropStep;

	int m_iCurrentScore;

    std::string m_strScore;

private:

};

#endif    // __LEVEL_H__
