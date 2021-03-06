#ifndef _GAME_H_
#define _GAME_H_

#include "d3dApp.h"
#include "Camera.h"
#include "Candy.h"
#include "Fruit.h"
#include "Map.h"
#include "Player.h"
#include "Ghost.h"
#include "ShaderManager.h"
#include "AudioEngine.h"
#include "LightManager.h"
#include "GUIManager.h"
#include "HighScore.h"

class Game : public d3dApp
{
public:
	enum GameType
	{
		FIRST_PERSON,
		OLD_SCHOOL,
		NO_CLIP,
	};
	GameType gameType;


	Game(void);
	~Game(void);

	void Init(HINSTANCE, HWND, bool, bool, float, float);
	void Update(const float dt);

	void FreezeGame(float sec);
	bool GameFreezed( const float dt );

	void PlayerDead();

	void UpdateAudio();
	
	void SwitchGameType( const float dt );

	void PlayerUpdate( const float dt );

	void ChangeView();

	void PacManRampage();

	void CountEatenCandy();

	void RemoveExpiredFruit();

	void SpawnFruit();

	void Draw();
private:
	void initShaders(void);
	void initLevel(void);

	void NextLevel(void);
	void ChangeLevel(int);
	void NewLife(void);

	std::vector<Obj3D *> mObjList;
	Map m_map;
	Player mPlayer;
	ShaderManager m_shaders;
	AudioEngine m_audio;
	LightManager m_lights;
	GUIManager m_GUIManager;

	Node* m_fruitNode;
	int m_eatenCandy;
	int m_totalCandy;

	void CameraFollowPlayer();
	void DebugCam(const float dt );
	void OldSchool();
	void PlaySound(int index, bool loop);

	void PlayerCollisionGhost();
	void PlayerCollisionFruit();

	LPCTSTR mLastKey;

	int m_ghostsEaten;
	int m_level;
	float m_ghostblueTime;
	float m_ghostweakTime;

	float m_freezeTime;
	float freezeTimer;
};

#endif

