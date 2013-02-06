#include "stdafx.h"
#include "Player.h"
#include <algorithm>

Node*					Player::GetNode()		{return mNode;}
Node*					Player::GetNextNode()	{return mNextNode;}
D3DXVECTOR3				Player::GetPosition()	{return mPosition;}
Player::PlayerStatus	Player::GetStatus()		{return mStatus;}

Player::Player(){}

Player::Player(D3DXVECTOR3 _pos, Node* _node)
{
	mStatus = ALIVE;
	mPosition = _pos;
	mNode = _node;
	mNextNode = _node;
	mPosition = mNode->GetPosition();
	mDirection = PAUSE;
	mMoveIterations = 0;
	mMaxIterations = 15;
	mMoveVector = D3DXVECTOR3(0,0,0);
	
}


Player::~Player(void)
{

}

void Player::Update(D3DXVECTOR3 look)
{
	Move();
	ChangeDirection(look);
}

void Player::OldSchoolControl(LPCSTR dir)
{
	Move();
	if(mDirection == PAUSE)
	{
		if (dir == "UP")
			mDirection = FORWARD;
		else if (dir == "DOWN")
			mDirection = BACKWARD;
		else if	(dir == "LEFT")
			mDirection = LEFT;
		else if (dir == "RIGHT")
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


void Player::Move()
{
	// �ter Godis
	if (mNode != mNextNode && mNode->Item != NULL)
	{
		mNode->Item->Eat();
	}

	//k�r iterationerna s� l�nge man inte n�tt noden -> != PAUSE.
	if(mDirection != PAUSE)
	{
		mPosition += mMoveVector/mMaxIterations;

		mNode = mNextNode; 

		mMoveIterations++;

		if(mMoveIterations > mMaxIterations)
		{
			mDirection = PAUSE;
			mMoveIterations =0;
			mPosition = mNode->GetPosition();
		}
	}
}

void Player::CheckDirections()
{
	if (mDirection == FORWARD		&& mNode->Front!=NULL) 
	{
		if (!mNode->Front->GhostNode)
		{mNextNode = mNode->Front;	mMoveVector = mNextNode->GetPosition() - mPosition;mLastDirection = mDirection;}
	}
	else if (mDirection == BACKWARD	&& mNode->Back!=NULL)
	{
		if (!mNode->Back->GhostNode)
		{mNextNode = mNode->Back;	mMoveVector = mNextNode->GetPosition() - mPosition;mLastDirection = mDirection;}
	}
	else if (mDirection == LEFT		&& mNode->Left!=NULL)
	{
		if (!mNode->Left->GhostNode)
		{mNextNode = mNode->Left;	mMoveVector = mNextNode->GetPosition() - mPosition;mLastDirection = mDirection;}
	}
	else if (mDirection == RIGHT	&& mNode->Right!=NULL)
	{
		if (!mNode->Right->GhostNode)
		{mNextNode = mNode->Right;	mMoveVector = mNextNode->GetPosition() - mPosition;mLastDirection = mDirection;}
	}
	else mDirection = PAUSE;
}

D3DXVECTOR3* Player::GetPositionPtr()
{
	return &mPosition;
}