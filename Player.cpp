#include "stdafx.h"
#include "Player.h"
#include <algorithm>

Node*					Player::GetNode()		{return mNode;}
Node*					Player::GetNextNode()	{return mNextNode;}
Node**					Player::GetNodePtr()	{return &mNode;}
Node**					Player::GetNextNodePtr(){return &mNextNode;}
D3DXVECTOR3				Player::GetPosition()	{return mPosition;}
D3DXVECTOR3				Player::GetMoveVector() {return mMoveVector;}
Player::PlayerStatus	Player::GetStatus()		{return mStatus;}
D3DXVECTOR3*			Player::GetPositionPtr(){return &mPosition;}
int						Player::GetLives()		{return mLives;}
int						Player::GetFrame()		{return mFrame;}
int						Player::GetMaxFrames()	{return mMaxFrames;}
bool					Player::IsHit()			{return mHit;}

Player::Player(): Billboard(D3DXVECTOR2(9, 9), 1.f) 
{
	mLives = 3;
	mPoints = 0;
}

void Player::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader, LPCSTR texture,D3DXVECTOR3 _pos, Node* _node)
{
	Billboard::Init(device, deviceContext, shader, texture, _pos, D3DXVECTOR3(1.f, 1.f, 1.f));

	mSpawnNode = _node;

	SetInitValues();
}

void Player::SetInitValues()
{
	mStatus = ALIVE;
	mNode = mSpawnNode;
	mNextNode = mSpawnNode;
	mPosition = mNode->GetPosition();
	mDirection = PAUSE;
	mDistanceCovered = 0;
	mSpeed = 35;
	mMoveVector = D3DXVECTOR3(0,0,0);
	mSuperCandy = false;
	mCandy = false;

	mFrame = 0;
	mMaxFrames = 4;
	mAnimationSpeed = 0.05f;
	mAnimationTimer = 0;
	mHit = false;
	mTexture = mAliveTexture;
}

void Player::InitGFX(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	if(FAILED(D3DX11CreateShaderResourceViewFromFile(device, mTexturePath.c_str(), 0, 0, &mAliveTexture, 0 )))
	{
		std::string failed = mTexturePath + " Texture Failed";
		MessageBox(0, failed.c_str(), "Fail!", 0);
	}

	if(FAILED(D3DX11CreateShaderResourceViewFromFile(device, "Content/Img/Pacman_dead_kelka.png", 0, 0, &mKillTexture, 0 )))
	{
		std::string failed = "Pacman Killtexture Failed";
		MessageBox(0, failed.c_str(), "Fail!", 0);
	}

	if(FAILED(D3DX11CreateShaderResourceViewFromFile(device, "Content/Img/smile.png", 0, 0, &mSmileTexture, 0 )))
	{
		std::string failed = "Pacman Smiletexture Failed";
		MessageBox(0, failed.c_str(), "Fail!", 0);
	}

	mTexture = mAliveTexture;
}

Player::~Player(void)
{

}

void Player::Update(const float dt){}

void Player::Update(D3DXVECTOR3 look, const float dt,bool oldSchoolView,LPCSTR dir )
{
	if(!mHit)
	{
		Move(dt);

		if(!oldSchoolView)
			ChangeDirection(look);
		else
			OldSchoolControl(dir);
	}

	Animation(dt);
	
}

void Player::Animation(const float dt)
{

	mAnimationTimer += dt;

	if(mAnimationTimer > mAnimationSpeed)
	{
		mFrame++;
		mAnimationTimer = 0;
	}

	if(mFrame >= mMaxFrames && mHit)
	{
		mFrame = mMaxFrames;
		mStatus = DEAD;
		mLives--;
		return;
	}

	if(mFrame >= mMaxFrames && mSmiling)
	{
		mFrame = 0;
		mTexture = mAliveTexture;
		mSmiling = false;
	}

	if(mFrame >= mMaxFrames)
		mFrame = 0;
}

void Player::OldSchoolControl(LPCSTR dir)
{
	if(mDirection == PAUSE)
	{
		if (dir == "W")
			mDirection = FORWARD;
		else if (dir == "S")
			mDirection = BACKWARD;
		else if	(dir == "A")
			mDirection = LEFT;
		else if (dir == "D")
			mDirection = RIGHT;

		CheckDirections();
		
		//testa  igen om gamla riktningen fungerar
		if (mDirection == PAUSE); 
		{
			mDirection = mLastDirection;
			CheckDirections();
		}
	}
}

void Player::InputDirection(D3DXVECTOR3 look)
{
	//	 NODE STRUCTURE			-- skiljer sig mot deklarationen i Map. z �r motsatt?
	// z
	// ^
	// |      Right
	// |       o				Up		= -x 
	// |       |				Back	= +x 
	// |  Up o-o-o Back			Right	= +z 
	// |       |				Left	= -z 
	// |       o 
	// |     Left
	//-|-----------------> x

	if(look.x < 0 && look.z > -0.5f && look.z < 0.5f)
		mDirection = FORWARD;
	else if (look.x > 0 && look.z < 0.5f && look.z > -0.5f)
		mDirection = BACKWARD;

	else if(look.z < 0 && look.x < 0.5f && look.x > -0.5f)
		mDirection = LEFT;
	else if(look.z > 0 && look.x > -0.5f && look.x < 0.5f)
		mDirection = RIGHT;
}

void Player::ChangeDirection(D3DXVECTOR3 look)
{
	//PAUSE inneb�r att man n�tt en ny nod, d� kan man byta
	if(mDirection == PAUSE)
	{
		D3DXVec3Normalize(&look,&look);

		InputDirection(look);

		CheckDirections();
		
		//testa  igen om gamla riktningen fungerar
		if (mDirection == PAUSE); 
		{
			mDirection = mLastDirection;
			CheckDirections();
		}
	}
	
}

bool Player::IsDead()
{
	if (mStatus == DEAD)
		return true;
	else
		return false;
}

bool Player::HasEatenCandy()
{
	if (mCandy)
	{
		mCandy = false;
		return true;
	}
	else
		return false;
}

bool Player::HasEatenSuperCandy()
{
	if (mSuperCandy)
	{
		mSuperCandy = false;
		return true;
	}
	else
		return false;
}

void Player::SuperCandy()
{
	mSuperCandy = true;
}

void Player::Collision(Node* node)
{
	if (!node->Item->IsEaten())
	{
		AddPoints(node->Item->GetPoints());
		node->Item->Eat();
		if(node->Item->IsSuperCandy())
			SuperCandy();

		mCandy = true;
	}
}

void Player::Move( const float dt)
{
	// �ter Godis
	if (mNode != mNextNode && mNode->Item != NULL && !mNode->Item->IsEaten())
	{
		Collision(mNode);
	}

	//k�r iterationerna s� l�nge man inte n�tt noden -> != PAUSE.
	if(mDirection != PAUSE)
	{
		D3DXVec3Normalize(&mMoveVector,&mMoveVector);
		D3DXVECTOR3 move = mMoveVector*dt*mSpeed;

		mPosition += move;
		mDistanceCovered += D3DXVec3Length(&move);
		mNode = mNextNode; 

		if(mDistanceCovered > mDistance)
		{
			
			mDirection = PAUSE;
			mDistanceCovered =0;
			mPosition = mNode->GetPosition();
		}
	}
}

void Player::CheckDirections()
{
	if (mDirection == FORWARD		&& mNode->Front!=NULL) 
	{
		if (!mNode->Front->GhostNode)
		{NewDirection(mNode->Front);}
	}
	else if (mDirection == BACKWARD	&& mNode->Back!=NULL)
	{
		if (!mNode->Back->GhostNode)
		{NewDirection(mNode->Back);}
	}
	else if (mDirection == LEFT		&& mNode->Left!=NULL)
	{
		if (!mNode->Left->GhostNode)
		{NewDirection(mNode->Left);}
	}
	else if (mDirection == RIGHT	&& mNode->Right!=NULL)
	{
		if (!mNode->Right->GhostNode)
		{NewDirection(mNode->Right);}
	}
	else mDirection = PAUSE;
}

void Player::ReSpawn()
{
	SetInitValues();
}

void Player::Kill()
{
	// Pacman only die once
	if (!mHit)
	{
		mHit = true;
		mFrame = 0;
		mAnimationSpeed = 0.14f;
		mMaxFrames = 10;
		mTexture = mKillTexture;
	}
}

void Player::NewDirection(Node* node)
{
	mNextNode = node;
	mMoveVector = mNextNode->GetPosition() - mPosition;
	mLastDirection = mDirection;
	mDistance = D3DXVec3Length(&mMoveVector);
}

void Player::AddPoints(int points)
{
	mPoints+=points;
}

void Player::Win()
{
	mTexture = mSmileTexture;
	mSmiling = true;
}
