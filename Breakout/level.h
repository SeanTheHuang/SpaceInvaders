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
class CShield;
class CSpecialShip;

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
	bool ProcessInvaderFiringBullets(float _fDeltaTick);


	void ProcessBulletsHitInvader();	//Win condition is also in here!
	void ProcessBulletsHitPlayer();		// i.e. [all invaders are killed]
	void ProcessBulletsHitShield();
	void ProcessBulletsInBound();
	void ProcessSpecialShipCollision();

	bool ProcessSpawnSpecialShip(float _deltaTick);
	void ProcessInvadersReachBottom();

	void EndGameScreen();

    void UpdateScoreText();

	void IncreaseInvaderSpeed();

	void DrawShieldLives();
	void DrawLives();
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
	std::vector<CShield*> m_vecShields;

	CFPSCounter* m_fpsCounter;
	CSpecialShip* m_ssShip;

    int m_iWidth;
    int m_iHeight;

	int m_iNumLivesLeft;

	float m_fInvaderShootCooldown;
	float m_fInvaderShootTimer;

	float m_fSpecialShipSpawnTimer;
	float m_fSpecialShipSpawnCooldown;

	//Variables to be changed in debug window
	float m_fPlayerMoveSpeed;
	float m_fShootCoolDown;

	float m_fInvaderMoveSpeedX;
	float m_fInvaderMoveSpeedY;

	float m_fPlayerBulletSpeed;
	float m_fInvaderBulletSpeed;

	int m_iPointsPerInvaderKill;
	//=======================

	int m_iCurrentScore;
    std::string m_strScore;

private:

};

#endif    // __LEVEL_H__
