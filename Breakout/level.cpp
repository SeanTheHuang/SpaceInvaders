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
#include "Game.h"
#include "Invader.h"
#include "Player.h"
#include "utils.h"
#include "backbuffer.h"
#include "framecounter.h"
#include "background.h"
#include "Bullet.h"

// This Include
#include "Level.h"

// Static Variables

// Static Function Prototypes

// Implementation

//#define CHEAT_BOUNCE_ON_BACK_WALL

CLevel::CLevel()
: m_iWidth(0)
, m_iHeight(0)
, m_fpsCounter(0)
, m_pPlayer(0)
, m_fInvaderMoveSpeed(30.0f)
, m_fInvaderDropStep(21.0f)
, m_fPlayerBulletSpeed(180.0f)
, m_fInvaderBulletSpeed(180.0f)
, m_iCurrentScore(0)
{

}

CLevel::~CLevel()
{
    while (m_vecInvaders.size() > 0)
    {
        CInvader* pInvader = m_vecInvaders[m_vecInvaders.size() - 1];

		m_vecInvaders.pop_back();

        delete pInvader;
    }

	delete m_fpsCounter;
	m_fpsCounter = 0;

	delete m_pPlayer;
	m_pPlayer = 0;

}

bool
CLevel::Initialise(int _iWidth, int _iHeight)
{
	//Window size
    m_iWidth = _iWidth;
    m_iHeight = _iHeight;

	//Create player cannon
	m_pPlayer = new CPlayer();
	VALIDATE(m_pPlayer->Initialise());
	m_pPlayer->SetX(static_cast<float>(_iWidth)/2);
	m_pPlayer->SetY(static_cast<float>(_iHeight) - 25);

	//Create the invaders
    const int kiInvadersPerRow = 11;
	const int kiRowsOfInvaders = 5;
    const int kiStartX = 57;
	const int kiStartY = 80;
    const int kiGap = 5;

    int iCurrentX = kiStartX;
    int iCurrentY = kiStartY;

	
	for (int i = 0; i < kiRowsOfInvaders; i++)
	{
		iCurrentY = kiStartY + (i * 21);
		iCurrentX = kiStartX;

		for (int i = 0; i < kiInvadersPerRow; i++)
		{
			CInvader* pInvader = new CInvader();
			VALIDATE(pInvader->Initialise());

			pInvader->SetX(static_cast<float>(iCurrentX));
			pInvader->SetY(static_cast<float>(iCurrentY));
			pInvader->SetMoveSpeedX(m_fInvaderMoveSpeed);
			pInvader->SetMoveStepY(m_fInvaderDropStep);

			iCurrentX += static_cast<int>(pInvader->GetWidth()) + kiGap;

			m_vecInvaders.push_back(pInvader);
		}
	}

	//Create FPS counter
	m_fpsCounter = new CFPSCounter();
	VALIDATE(m_fpsCounter->Initialise());

    return (true);
}

void
CLevel::Draw()
{
	//Draw Background (TODO??)

	//Draw Bullets
	for (unsigned int i = 0; i < m_vecBullets.size(); i++)
	{
		m_vecBullets[i]->Draw();
	}

	//Draw Player
	m_pPlayer->Draw();

	//Draw Invaders
	for (unsigned int i = 0; i < m_vecInvaders.size(); ++i)
	{
		m_vecInvaders[i]->Draw();
	}

    DrawScore();
	DrawFPS();
}

void
CLevel::Process(float _fDeltaTick)
{
	//Process Collisions

	//Process Firing bullets
	ProcessPlayerFiringBullet();

	//Bullet process
	for (unsigned int i = 0; i < m_vecBullets.size(); i++)
	{
		m_vecBullets[i]->Process(_fDeltaTick);
	}

	//Player process
	m_pPlayer->Process(_fDeltaTick);

	//Invader Process
	ProcessInvadersInBounds();
	for (unsigned int i = 0; i < m_vecInvaders.size(); ++i)
	{
		m_vecInvaders[i]->Process(_fDeltaTick);
	}

   
	UpdateScoreText();
	m_fpsCounter->CountFramesPerSecond(_fDeltaTick);
}

void
CLevel::DrawScore()
{
    HDC hdc = CGame::GetInstance().GetBackBuffer()->GetBFDC();

    const int kiX = 5;
    const int kiY = m_iHeight - 18;
	SetBkMode(hdc, TRANSPARENT);
    
    TextOutA(hdc, kiX, kiY, m_strScore.c_str(), static_cast<int>(m_strScore.size()));
}

void 
CLevel::UpdateScoreText()
{
    m_strScore = "Score: ";

    m_strScore += ToString(m_iCurrentScore);
}


void 
CLevel::DrawFPS()
{
	HDC hdc = CGame::GetInstance().GetBackBuffer()->GetBFDC(); 

	m_fpsCounter->DrawFPSText(hdc, m_iWidth, m_iHeight);

}

void CLevel::ProcessInvadersInBounds()
{

	for (unsigned int i = 0; i < m_vecInvaders.size(); i++)
	{
		if (m_vecInvaders[i]->IsHit() == false)
		{
			int leftBound = m_vecInvaders[i]->GetX() - m_vecInvaders[i]->GetWidth() /2;
			int rightBound =m_vecInvaders[i]->GetX() + m_vecInvaders[i]->GetWidth() / 2;
			if ((leftBound <= 0 && m_vecInvaders[i]->MoveSpeedX() < 0)
				|| (rightBound >= m_iWidth) && m_vecInvaders[i]->MoveSpeedX() > 0)
			{
				MoveInvadersDown();	//Hit edge of screen,
				break;				//edit invader's position and movement
			}
		}
	}
}

void CLevel::MoveInvadersDown()
{
	for (unsigned int i = 0; i < m_vecInvaders.size(); i++)
	{
		if (m_vecInvaders[i]->IsHit() == false) 
		{
			m_vecInvaders[i]->SetY(m_vecInvaders[i]->GetY() + m_vecInvaders[i]->MoveStepY());
			m_vecInvaders[i]->SetMoveSpeedX(m_vecInvaders[i]->MoveSpeedX() * -1);
		}
	}
}


bool
CLevel::ProcessPlayerFiringBullet()
{
	if (GetAsyncKeyState(VK_UP) && m_pPlayer->CannonOffCooldown())
	{
		//Create a bullet here, make it go up
		CBullet* bullet = new CBullet();
		VALIDATE(bullet->Initialise());

		bullet->SetX(m_pPlayer->GetX());
		bullet->SetY(m_pPlayer->GetY() - bullet->GetHeight() / 2);
		bullet->setBulletOwner(PLAYER);
		bullet->setBulletSpeed(m_fPlayerBulletSpeed * -1.0f); //Make sure bullet going up

		m_vecBullets.push_back(bullet);

		m_pPlayer->setCannonOnCooldown();
	}
}