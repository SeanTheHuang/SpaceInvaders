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
#include <math.h>
#include <time.h>
#include "Shields.h"
#include "SpecialShip.h"

// This Include
#include "Level.h"

// Static Variables

// Static Function Prototypes

// Implementation

CLevel::CLevel()
: m_iWidth(0)
, m_iHeight(0)
, m_iNumLivesLeft(3)
, m_fpsCounter(0)
, m_pPlayer(0)
, m_fPlayerMoveSpeed(150.0f)
, m_fShootCoolDown(0.5f)
, m_fInvaderShootCooldown(1.5f)
, m_fInvaderShootTimer(0.0f)
, m_fInvaderMoveSpeedX(20.0f)
, m_fInvaderMoveSpeedY(21.0f)
, m_fSpecialShipSpawnCooldown(10.0f)
, m_fSpecialShipSpawnTimer(0.0f)
, m_fPlayerBulletSpeed(500.0f)
, m_fInvaderBulletSpeed(300.0f)
, m_iCurrentScore(0)
, m_iPointsPerInvaderKill(16)
, m_ssShip(nullptr)
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

	while (m_vecBullets.size() > 0)
	{
		CBullet* pBullet = m_vecBullets[m_vecBullets.size() - 1];

		m_vecBullets.pop_back();

		delete pBullet;
	}

	if (m_ssShip != nullptr)
	{
		delete m_ssShip;
		m_ssShip = nullptr;
	}

	delete m_fpsCounter;
	m_fpsCounter = 0;

	delete m_pPlayer;
	m_pPlayer = 0;

}

bool
CLevel::Initialise(int _iWidth, int _iHeight)
{
	srand((unsigned int)time(NULL));

	//Window size
    m_iWidth = _iWidth;
    m_iHeight = _iHeight;

	//Create player cannon
	m_pPlayer = new CPlayer();
	VALIDATE(m_pPlayer->Initialise());
	m_pPlayer->SetX(static_cast<float>(_iWidth)/2);
	m_pPlayer->SetY(static_cast<float>(_iHeight) - 25);
	m_pPlayer->SetMoveSpeed(m_fPlayerMoveSpeed);
	m_pPlayer->SetShotCooldown(m_fShootCoolDown);

	const float kiShieldY = 500;
	const float kiShieldStartX = 90;
	const float kiShieldSpacing = 80;

	float iCurrentX = kiShieldStartX;
	float iCurrentY = kiShieldY;

	//Create the shields
	for (int i = 0; i < 3; i++)
	{
		CShield* pShield = new CShield();
		VALIDATE(pShield->Initialise());
		pShield->SetX(iCurrentX);
		pShield->SetY(iCurrentY);

		m_vecShields.push_back(pShield);

		iCurrentX += pShield->GetWidth() + kiShieldSpacing;
	}

	//Create the invaders
    const int kiInvadersPerRow = 11;
	const int kiRowsOfInvaders = 5;
    const int kiStartX = 57;
	const int kiStartY = 80;
    const int kiGap = 5;

    iCurrentX = kiStartX;
    iCurrentY = kiStartY;
	
	for (int i = 0; i < kiRowsOfInvaders; i++)
	{
		iCurrentY = kiStartY + ((float)i * 21);
		iCurrentX = kiStartX;

		for (int i = 0; i < kiInvadersPerRow; i++)
		{
			CInvader* pInvader = new CInvader();
			VALIDATE(pInvader->Initialise());

			pInvader->SetX(iCurrentX);
			pInvader->SetY(iCurrentY);
			pInvader->SetMoveSpeedX(m_fInvaderMoveSpeedX);
			pInvader->SetMoveStepY(m_fInvaderMoveSpeedY);

			iCurrentX += pInvader->GetWidth() + kiGap;

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

	//Draw shields
	for (unsigned int i = 0; i < m_vecShields.size(); i++)
	{
		m_vecShields[i]->Draw();
	}

	//Draw special ship
	if (m_ssShip != nullptr)
	{
		m_ssShip->Draw();
	}

	DrawShieldLives();
    DrawScore();
	DrawLives();
	DrawFPS();
}

void
CLevel::Process(float _fDeltaTick)
{
	//Process Firing bullets
	ProcessPlayerFiringBullet();
	ProcessInvaderFiringBullets(_fDeltaTick);

	//Bullet process
	for (unsigned int i = 0; i < m_vecBullets.size(); i++)
	{
		m_vecBullets[i]->Process(_fDeltaTick);
	}

	//Player process
	m_pPlayer->Process(_fDeltaTick);

	//Shields process
	for (unsigned int i = 0; i < m_vecShields.size(); i++)
	{
		m_vecShields[i]->Process(_fDeltaTick);
	}

	//Invader Process
	ProcessInvadersInBounds();
	for (unsigned int i = 0; i < m_vecInvaders.size(); ++i)
	{
		m_vecInvaders[i]->Process(_fDeltaTick);
	}

	ProcessSpawnSpecialShip(_fDeltaTick);

	//Process Collisions
	ProcessBulletsHitInvader();
	ProcessBulletsHitPlayer();
	ProcessBulletsInBound();
	ProcessBulletsHitShield();
	ProcessSpecialShipCollision();
	ProcessInvadersReachBottom();
	//======================

	//Special Ship Process
	if (m_ssShip != nullptr)
	{
		m_ssShip->Process(_fDeltaTick);
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
CLevel::DrawShieldLives()
{
	HDC hdc = CGame::GetInstance().GetBackBuffer()->GetBFDC();

	for (unsigned int i = 0; i < m_vecShields.size(); i++)
	{
		std::string output = ToString(m_vecShields[i]->CurrentLivesLeft());
		float iX = m_vecShields[i]->GetX() - 3;
		float iY = m_vecShields[i]->GetY() - 15;
		TextOutA(hdc, (int)iX, (int)iY, output.c_str(), static_cast<int>(output.size()));
	}
}

void
CLevel::DrawLives()
{
	HDC hdc = CGame::GetInstance().GetBackBuffer()->GetBFDC();

	const int kiX = 120;
	const int kiY = m_iHeight - 18;
	SetBkMode(hdc, TRANSPARENT);

	std::string output = "Lives: " + ToString(m_iNumLivesLeft);

	TextOutA(hdc, kiX, kiY, output.c_str(), static_cast<int>(output.size()));
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

void 
CLevel::ProcessInvadersInBounds()
{

	for (unsigned int i = 0; i < m_vecInvaders.size(); i++)
	{

		float leftBound = m_vecInvaders[i]->GetX() - m_vecInvaders[i]->GetWidth() /2;
		float rightBound = m_vecInvaders[i]->GetX() + m_vecInvaders[i]->GetWidth() / 2;
		if ((leftBound <= 0 && m_vecInvaders[i]->MoveSpeedX() < 0)
			|| (rightBound >= m_iWidth) && m_vecInvaders[i]->MoveSpeedX() > 0)
		{
			MoveInvadersDown();	//Hit edge of screen,
			break;				//edit invader's position and movement
		}
		
	}
}

void 
CLevel::MoveInvadersDown()
{
	for (unsigned int i = 0; i < m_vecInvaders.size(); i++)
	{
		m_vecInvaders[i]->SetY(m_vecInvaders[i]->GetY() + m_vecInvaders[i]->MoveStepY());
		m_vecInvaders[i]->SetMoveSpeedX(m_vecInvaders[i]->MoveSpeedX() * -1);
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
		bullet->setBulletSpeed(m_fPlayerBulletSpeed);

		m_vecBullets.push_back(bullet);

		m_pPlayer->setCannonOnCooldown();
	}

	return true;
}

void
CLevel::ProcessBulletsHitInvader()
{
	for (unsigned int i = 0; i < m_vecBullets.size(); i++) //For each bullet
	{
		for (unsigned int j = 0; j < m_vecInvaders.size(); j++) //Check collision with Invaders
		{
			if (m_vecBullets[i]->WhoFiredBullet() == PLAYER)
			{
				float bX = m_vecBullets[i]->GetX();
				float bY = m_vecBullets[i]->GetY();
				float bW = m_vecBullets[i]->GetWidth();
				float bH = m_vecBullets[i]->GetHeight();

				float iX = m_vecInvaders[j]->GetX();
				float iY = m_vecInvaders[j]->GetY();
				float iW = m_vecInvaders[j]->GetWidth();
				float iH = m_vecInvaders[j]->GetHeight();

				if ((bX + bW/2 > iX - iW / 2) &&
					(bX - bW/2 < iX + iW / 2) &&
					(bY + bH/2 > iY - iH / 2) &&
					(bY - bH/2 < iY + iH / 2))
				{

					//Collision happened!
					m_iCurrentScore += m_iPointsPerInvaderKill;

					CBullet* temp = m_vecBullets[i];
					m_vecBullets.erase(m_vecBullets.begin() + i);
					delete temp;
					
					CInvader* temp2 = m_vecInvaders[j];
					m_vecInvaders.erase(m_vecInvaders.begin() + j);
					delete temp2;

					//Increase speed of other invaders
					IncreaseInvaderSpeed();

					//GAME WIN CONDITION
					if (m_vecInvaders.empty())
					{
						CGame::GetInstance().GameEndWin((float)m_iCurrentScore);
					}

					break;

				}
			}
		}
	}
}

void 
CLevel::ProcessBulletsInBound()
{
	for (unsigned int i = 0; i < m_vecBullets.size(); i++)
	{
		if (m_vecBullets[i]->GetY() + m_vecBullets[i]->GetHeight() > m_iHeight+50 ||
			m_vecBullets[i]->GetY() - m_vecBullets[i]->GetHeight() < -50)
		{
			CBullet* temp = m_vecBullets[i];
			m_vecBullets.erase(m_vecBullets.begin() + i);
			delete temp;

			if (i != 0)
			{
				i--;
			}
				
		}
	}
}

void
CLevel::ProcessBulletsHitPlayer()
{
	float pX = m_pPlayer->GetX();
	float pY = m_pPlayer->GetY();
	float pW = m_pPlayer->GetWidth();
	float pH = m_pPlayer->GetHeight();


	for (unsigned int i = 0; i < m_vecBullets.size(); i++) //For each bullet
	{
		if (m_vecBullets[i]->WhoFiredBullet() == INVADER){

			float bX = m_vecBullets[i]->GetX();
			float bY = m_vecBullets[i]->GetY();
			float bW = m_vecBullets[i]->GetWidth();
			float bH = m_vecBullets[i]->GetHeight();

			if ((bX + bW / 2 > pX - pW / 2) &&
				(bX - bW / 2 < pX + pW / 2) &&
				(bY + bH / 2 > pY - pH / 2) &&
				(bY - bH / 2 < pY + pH / 2))
			{

				//Collision happened! Player got hurt!
				m_iNumLivesLeft--;

				CBullet* temp = m_vecBullets[i];
				m_vecBullets.erase(m_vecBullets.begin() + i);
				delete temp;

				if (m_iNumLivesLeft < 1)	//Ran out of lives!
				{
					EndGameScreen();
				}
				break;
			}
		}
		
		
	}
}

void
CLevel::ProcessInvadersReachBottom()
{
	for (unsigned int i = 0; i < m_vecInvaders.size(); i++)
	{
		if (m_vecInvaders[i]->GetY() > m_iHeight)
		{
			EndGameScreen();
		}
	}
}

void
CLevel::EndGameScreen()
{
	CGame::GetInstance().GameEndLost(static_cast<float>(m_iCurrentScore));
}

void
CLevel::ProcessBulletsHitShield()
{
	for (unsigned int i = 0; i < m_vecShields.size(); i++) //For each shield
	{
		for (unsigned int j = 0; j < m_vecBullets.size(); j++) //Check with each bullet
		{
			//See if collision

			float bX = m_vecBullets[j]->GetX();		//Bullet values
			float bY = m_vecBullets[j]->GetY();
			float bW = m_vecBullets[j]->GetWidth();
			float bH = m_vecBullets[j]->GetHeight();

			float sX = m_vecShields[i]->GetX();		//Shield values
			float sY = m_vecShields[i]->GetY();
			float sW = m_vecShields[i]->GetWidth();
			float sH = m_vecShields[i]->GetHeight();

			if ((bX + bW / 2 > sX - sW / 2) &&
				(bX - bW / 2 < sX + sW / 2) &&
				(bY + bH / 2 > sY - sH / 2) &&
				(bY - bH / 2 < sY + sH / 2))
			{

				//Collision happened!

				CBullet* temp = m_vecBullets[j];
				m_vecBullets.erase(m_vecBullets.begin() + j);
				delete temp;


				m_vecShields[i]->ShieldWasHit();

				if (m_vecShields[i]->CurrentLivesLeft() < 1)
				{
					CShield* temp2 = m_vecShields[i];
					m_vecShields.erase(m_vecShields.begin() + i);
					delete temp2;
				}

				break;
			}
		}
	}
}

void 
CLevel::ProcessSpecialShipCollision()
{
	if (m_ssShip == nullptr)	//Do not process if special ship is dead
	{
		return;
	}

	float sX = m_ssShip->GetX();	//Special ship stats
	float sY = m_ssShip->GetY();
	float sW = m_ssShip->GetWidth();
	float sH = m_ssShip->GetHeight();


	for (unsigned int i = 0; i < m_vecBullets.size(); i++) //For each bullet
	{
		if (m_vecBullets[i]->WhoFiredBullet() == PLAYER) {

			float bX = m_vecBullets[i]->GetX();
			float bY = m_vecBullets[i]->GetY();
			float bW = m_vecBullets[i]->GetWidth();
			float bH = m_vecBullets[i]->GetHeight();

			if ((bX + bW / 2 > sX - sW / 2) &&
				(bX - bW / 2 < sX + sW / 2) &&
				(bY + bH / 2 > sY - sH / 2) &&
				(bY - bH / 2 < sY + sH / 2))
			{

				//Collision happened! special ship down!!
				delete m_ssShip;
				m_ssShip = nullptr;

				m_iCurrentScore += (m_iPointsPerInvaderKill * 10);

				CBullet* temp = m_vecBullets[i];
				m_vecBullets.erase(m_vecBullets.begin() + i);
				delete temp;

				return;
			}
		}
	}

	//Check if special ship is out of bounds
	if (m_ssShip->GetX() > m_iWidth + 100)
	{
		delete m_ssShip;
		m_ssShip = nullptr;
	}
}

bool 
CLevel::ProcessSpawnSpecialShip(float _deltaTick)
{
	if (m_ssShip != nullptr)	//Logic does not run if special ship is alive
	{
		return true;
	}

	m_fSpecialShipSpawnTimer += _deltaTick;

	if (m_fSpecialShipSpawnTimer >= m_fSpecialShipSpawnCooldown)
	{
		m_fSpecialShipSpawnTimer = 0.0f;

		m_ssShip = new CSpecialShip();
		VALIDATE(m_ssShip->Initialise());
		m_ssShip->SetX(-100.0f);
		m_ssShip->SetY(40);
	}


	return true;
}

void
CLevel::IncreaseInvaderSpeed()
{
	//Case 1: No more invaders, nothing happens
	if (m_vecInvaders.empty()) { return; }

	float newSpeed = m_vecInvaders[0]->MoveSpeedX();

	//Case 2: One invader left, super fast!
	if (m_vecInvaders.size() == 1)
	{
		newSpeed += (newSpeed > 0) ? 300.0f : -300.0f;
		m_vecInvaders[0]->SetMoveSpeedX(newSpeed);
		return;
	}

	newSpeed += (newSpeed > 0) ? 2.0f : -2.0f;

	for (unsigned int i = 0; i < m_vecInvaders.size(); i++)
	{
		m_vecInvaders[i]->SetMoveSpeedX(newSpeed);
	}
}

bool
CLevel::ProcessInvaderFiringBullets(float _fDeltaTick)
{
	if (m_vecInvaders.empty()) { return true; }

	//Shooting: After firing cooldown, X number of invaders will shoot downwards
	// X = 1 + (1 per every 10 invaders)
	//i.e. 5 invaders left = 1 bullet, 35 invaders left = 4 bullets
	
	//If multiple bullets end up in the same spot, only the first one is created

	m_fInvaderShootTimer += _fDeltaTick;
	if (m_fInvaderShootTimer >= m_fInvaderShootCooldown)
	{
		m_fInvaderShootTimer = 0.0f;	//Reset timer for next shot

		std::vector<CBullet*> tempBulletVec;

		int numShots = 1 + static_cast<int>(m_vecInvaders.size() / 10);

		for (int i = 0; i < numShots; i++)
		{
			//Randomly pick an invader to shoot from
			int randIndex = rand() % m_vecInvaders.size();
			float newX = m_vecInvaders[randIndex]->GetX();
			float newY = m_vecInvaders[randIndex]->GetY();
			

			bool spotTaken = false;
			//Check if bullet position collides with any other
			for (unsigned int j = 0; j < tempBulletVec.size(); j++)
			{
				if (tempBulletVec[j]->GetX() == newX && tempBulletVec[j]->GetY() == newY)
				{
					//Duplicate bullet created
					spotTaken = true;
					break;
				}
			}

			//If bullet was not duplicate, create bullet now
			if (!spotTaken)
			{
				CBullet* newBullet = new CBullet();
				VALIDATE(newBullet->Initialise());
				newBullet->SetX(newX);
				newBullet->SetY(newY);
				newBullet->setBulletSpeed(m_fInvaderBulletSpeed);
				newBullet->setBulletOwner(INVADER);

				tempBulletVec.push_back(newBullet);
			}
		}

		//Once done creating all bullets, put bullets into main bullet vector
		for (unsigned int i = 0; i < tempBulletVec.size(); i++)
		{
			m_vecBullets.push_back(tempBulletVec[i]);
		}


	}

	return true;
}


//GET AND SET FUN TIME FOR EVERYONE
float CLevel::GetPlayerMoveSpeed()
{
	return m_fPlayerMoveSpeed;
}

void CLevel::SetPlayerMoveSpeed(float newVal)
{
	m_fPlayerMoveSpeed = newVal;
	m_pPlayer->SetMoveSpeed(newVal);

}

float CLevel::GetPlayerShootCooldown()
{
	return GetPlayerShootCooldown();
}

void CLevel::SetPlayerShootCooldown(float newVal)
{
	SetPlayerShootCooldown(newVal);
}

float CLevel::GetInvaderShootCooldown()
{
	return m_fInvaderShootCooldown;
}

void CLevel::SetInvaderShootCooldown(float newVal)
{
	m_fInvaderShootCooldown = newVal;
}

float CLevel::GetInvaderMoveSpeedX()
{
	return m_fInvaderMoveSpeedX;
}

float CLevel::GetInvaderMoveSpeedY()
{
	return m_fInvaderMoveSpeedY;
}

void CLevel::SetInvaderMoveSpeedX(float newVal)
{
	m_fInvaderMoveSpeedX = newVal;

	for (unsigned int i = 0; i < m_vecInvaders.size(); i++)
	{
		m_vecInvaders[i]->SetMoveSpeedX(newVal);
	}
}

void CLevel::SetInvaderMoveSpeedY(float newVal)
{
	m_fInvaderMoveSpeedY = newVal;

	for (unsigned int i = 0; i < m_vecInvaders.size(); i++)
	{
		m_vecInvaders[i]->SetMoveStepY(newVal);
	}
}

float CLevel::GetSpecialShipCooldown()
{
	return m_fSpecialShipSpawnCooldown;
}

void CLevel::SetSpecialShipCooldown(float newVal)
{
	m_fSpecialShipSpawnCooldown = newVal;
}

float CLevel::GetPlayerBulletSpeed()
{
	return m_fPlayerBulletSpeed;
}

void CLevel::SetPlayerBulletSpeed(float newVal)
{
	m_fPlayerBulletSpeed = newVal;

	for (unsigned int i = 0; i < m_vecBullets.size(); i++)
	{
		if (m_vecBullets[i]->WhoFiredBullet() == PLAYER)
		{
			m_vecBullets[i]->setBulletSpeed(newVal);
		}
	}
}

float CLevel::GetInvaderBulletSpeed()
{
	return m_fInvaderBulletSpeed;
}

void CLevel::SetInvaderBulletSpeed(float newVal)
{
	m_fInvaderBulletSpeed = newVal;

	for (unsigned int i = 0; i < m_vecBullets.size(); i++)
	{
		if (m_vecBullets[i]->WhoFiredBullet() == INVADER)
		{
			m_vecBullets[i]->setBulletSpeed(newVal);
		}
	}
}