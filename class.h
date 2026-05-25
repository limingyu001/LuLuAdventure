#pragma once
#include <string>
#include <iostream>
#include "globle.h"
#include <graphics.h>
#include <vector>
using namespace std;

//特殊颜色表类
class specialTable {
public:
	int num;
	COLORREF* colors;
	int* tickTable;
public:
	specialTable(int num,int* rtable,int*gtable,int*btable);
};



//图集类
class Atlas {
public:
	int animation_num;
	vector<IMAGE*> frames;

public:
	Atlas(int num,int startID,int w ,int h);
	Atlas(int num, wstring pathV, int w, int h);
};

//动画基类
class Animation {
public:
	Atlas* atlas;
	int animation_index=1;
	int interval_time;
	int timer = 0;
	int frame_num;
public:
	Animation(Atlas* atlas, int t);
	void spplay(int x, int y, int dtime, specialTable* sp);
	void play(int x, int y, int dtime, bool isAttacted);
	void play(int x, int y, int dtime, bool isAttacted, wstring specialImgPath);
	void play(int x, int y, int dtime, bool isAttacted, int specialResourceID);
};

//动画组类
class Animations {
public:
	Animation* front_animation;
	Animation* right_animation;
	Animation* left_animation;
	Animation* behind_animation;
	Animation* static_animation;
public:
	Animations(Animation* front_animation, Animation* right_animation, Animation* left_animation, Animation* behind_animation, Animation* static_animation);
	Animations(Animations* ani);
};

//玩家类
struct persistPlayer {
	int maxHP=5;
	int speed = 5;
	int BoomMaxNum = 5;
	int BoomRecoverSpeed = 4;
	int BoomRecoverProgressMax = 50;
	int putBoomDamage = 51;
	int putBoomRange = 200;
	int killEnemyAddBoomNumPercent = 5;
	int coinNum = 0;
};


//玩家属性结构体
struct playerConfig {
	int HP = 5;
	int maxHP = 5;
	int speed = 5;
	int BoomNum = 5;
	int BoomMaxNum = 5;
	int BoomRecoverSpeed = 4;
	int BoomRecoverProgress = 0;
	int BoomRecoverProgressMax = 50;
	long BoomRecoverTick = 0;
};



class Player {
public:
	playerConfig playerState;
	persistPlayer playerPersistState;
	bool isAttacted= false;
	unsigned short attactedShowTick= 0;
	unsigned short specialShowTick = 0;
	unsigned short killShowTick = 0;
public:
	bool isRightMove = false;
	bool isLeftMove = false;
	bool isUpMove = false;
	bool isDownMove = false;
	specialTable* specialShowTable;
	Animations* animations;
	IMAGE shadow;
	int x;
	int y;
public:
	Player(int, int, Animations* ani, specialTable* sp);
	void draw(int t);
	void drawShadow();
	void shot();
	void putBoom();
	void getAttack();
	void update();
};

//敌人类
class Enemy {
public:
	int HP;
	int speed;
	Animations* animations;
	IMAGE shadow;
	bool isAttacked = false;
	unsigned short attactedShowTick;
public:
	int x;
	int y;
public:
	Enemy(Animations* ani, int hp,int spd,int x,int y);
	Enemy(Animations* ani, int hp, int spd);
	void getDamge(int damage);
	void drawShadow();
	
};

//子弹类
class Bullet {
public:
	int x;
	int y;
	bulletType type;
	Animations* BoomAnimations;
	Animations* BulletAnimations;
	int speed;
	int damageRange;
	int damage;
public:
	Bullet(bulletType t, int x, int y);
	void draw(int time);
};

//道具类
class gameTool {
public:
	int x;
	int y;
	int level;
	toolType type;
	bool isPermanent = false;
	int maintainTime;
	int existTime;
	IMAGE bubbleImg;
	IMAGE gameToolIco;
	IMAGE moreIco;
	long summonTimeStamp;
public:
	gameTool(int level, int type);
	void giveEffect();
	void draw();
};

//效果类
class effect {
public:
	IMAGE bk;
	IMAGE border;
	IMAGE ico;
	IMAGE timebar;
	int level;
	int maintainTime;
	int changeNum;
	long startTimeStamp;
	toolType type;
public:
	effect(toolType type,int level);
};

//按钮类
class Button {
public:
	int x;
	int y;
	int width;
	int height;
	bool isHover = false;
	bool isActive = false;
	IMAGE img;
	IMAGE hoverImg;
	IMAGE activeImg;
	void* onClickFunc;
public:
	//构造和析构函数
	Button(int x, int y, int width, int height, wstring imgPath, wstring hoverImgPath, wstring activeImgPath, void* onClickFunc);
	Button(int x, int y, int width, int height, int imgID,int hoverImgID, int activeImgID, void* onClickFunc);
	Button();
	~Button();
	virtual void draw();
};

//注册表工具类
class Registryer {
public:
	Registryer();
	bool setInt(string key, int value);
	int readInt(string key,int defaultValue);
private:
	string KeyPath;
};



class Game {
public:
	enum gameState {
		menu=1, shop,selectLevel ,playing, gameWin,gameLose,more
	};
	gameState state = menu;
	int currentLevel = 1;
	IMAGE menuImgBK;
	IMAGE levelSelectBK;
	IMAGE playingBKImg;
	IMAGE loseImgBK;
	IMAGE winImgBK;
	IMAGE shopImgBK;
public:
	Game(int menuImgBKID, int levelSelectBKID, int playingBKImgID, int loseImgBKID, int winImgBKID, int shopImgBKID);

};

class LevelBtn :public Button {
public:
	int id;
	bool isLocked = true;
public:
	LevelBtn(int x, int y, int width, int height, int imgID, int hoverImgID, int activeImgID, int lockedImgID, void* onClickFunc,int id);
	void draw();
};

class levelInfor {
public:
	int id;
	int enemyHP;
	int enemyNum;
	int enemySpeed;
	bool isUnlock;
};

class GoodsBtn :public Button {
public:
	int id;
	int currentValue;
	string description;
	int price;
	float priceIncreaseRate;
	int level;
	int maxLevel;
	int baseValue;
	int nextValue;
	float deltaRate;//二选一
	int deltaNum;
	IMAGE unaffordableImg;
	IMAGE maxLevelImg;
public:
	GoodsBtn(int x, int y, int width, int height, void* onClickFunc, int id);
	void draw();
	void buy();
	void update();
	bool isUnaffordable();
	void playerStateUpdate();
};

class Coin {
public:
	int x;
	int y;
	int value;
	Animation* animation;
	IMAGE staticImg;
	IMAGE shadow;
	bool isCollected = false;
public:
	Coin(int x, int y, Animation* animation,int hp);
	Coin(int x, int y, int hp);
	void draw();
	void move();

};