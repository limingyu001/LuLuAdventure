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


//玩家属性类
struct playerConfig {
	int HP = 5;
	int maxHP = 5;
	int speed = 5;
	int BoomNum=5;
	int BoomMaxNum = 5;
	int BoomRecoverSpeed = 4;
	int BoomRecoverProgress = 0;
	int BoomRecoverProgressMax = 50;
	long BoomRecoverTick = 0;
};

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
class Player {
public:
	playerConfig playerState;
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
	Player(int, int, Animations* ani, specialTable* sp) {
		x = 300;
		y = 300;
		specialShowTable = sp;
		animations = ani;
		loadimage(&shadow,_T("PNG"), MAKEINTRESOURCE(RES_SHADOW_ID), PLAYER_SHADOW_WIDTH, PLAYER_SHADOW_HEIGHT);
	}
	void draw(int t);
	void drawShadow();
	void shot();
	void putBoom();
	void getAttack();

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
	Enemy(Animations* ani);
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