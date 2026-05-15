#include "class.h"
#include "function.h"
#include "globle.h"
#include "resource.h"
#include <graphics.h>
vector<Bullet*> boom_list;
vector<gameTool*> tools_list;
extern Player player;
extern Animations* boom_animations;
IMAGE greenBubble;
IMAGE blueBubble;
IMAGE purpleBubble;
IMAGE sevenBubble;
IMAGE boomIco;
IMAGE hpIco;
IMAGE icecreamIco;
IMAGE plusIco;
IMAGE boostIco;
IMAGE fastIco;
IMAGE effect_nmborder;
IMAGE effect_ifborder;
IMAGE effect_timebar;
IMAGE effect_ico_boomnb;
IMAGE effect_ico_boomrb;
IMAGE effect_ico_iceb;
IMAGE effect_ico_orangeb;
IMAGE effect_greenbk;
IMAGE effect_bluebk;
IMAGE effect_purplebk;
IMAGE effect_sevenbk;


specialTable::specialTable(int num, int* rtable, int* gtable, int* btable) {
	this->num = num;
	colors = new COLORREF[num];
	tickTable = new int[num];
	for (int i = 0; i < num; i++) {
		tickTable[i] = 0;
		COLORREF temp = RGB(rtable[i], gtable[i], btable[i]);
		colors[i] = temp;
	}

}

//图集类
Atlas::Atlas(int num,int startID,int w,int h) {
	animation_num = num;
	for (int i = 1; i <= num; i++) {
		IMAGE* t = new IMAGE();
		//wstring t_path = pathV+to_wstring(i)+L".png";
		loadimage(t, _T("PNG"),MAKEINTRESOURCE(startID+i-1), w, h);
		frames.push_back(t);
	}
}

Atlas::Atlas(int num, wstring pathV, int w, int h) {
	animation_num = num;
	for (int i = 1; i <= num; i++) {
		IMAGE* t = new IMAGE();
		wstring t_path = pathV + to_wstring(i) + L".png";
		loadimage(t, t_path.c_str(), w, h);
		frames.push_back(t);
	}
}




//动画类
Animation::Animation(Atlas* atlas,int t){
	this->atlas = atlas;
	interval_time = t;
	frame_num = atlas->animation_num;
}

void Animation::spplay(int x, int y, int dtime, specialTable* sp) {
	timer += dtime;
	if (timer >= interval_time) {
		timer = 0;
		animation_index++;
		if (animation_index >= frame_num) { animation_index = 0; }
	}
	int i = 0;
	for (i ; i < sp->num; i++) {
		if (sp->tickTable[i]) {
			break;
		}
	}
	if(sp->tickTable[i]>0){//绘制特殊图像
		sp->tickTable[i]--;
		IMAGE white;
		int width = atlas->frames[animation_index]->getwidth();
		int height = atlas->frames[animation_index]->getheight();
		Resize(&white, width, height);//加载创建的空图片为指定大小
		DWORD* raw = GetImageBuffer(atlas->frames[animation_index]);//获取原图片像素缓冲区
		DWORD* sketch = GetImageBuffer(&white);//获取新建的图片像素缓冲区
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {//遍历每一个像素
				int idx = y * width + x;
				if ((raw[idx] & 0xFF000000) >> 24) {//取出alpha通道，如果不为0
					sketch[idx] = BGR(sp->colors[i]) | (((DWORD)(BYTE)(255)) << 24);//将像素点颜色变为指定颜色
				}
			}
		}
		putimageAlpha(x, y, &white);
	}
	else{
		putimageAlpha(x, y, atlas->frames[animation_index]);
		//绘制原始图像
	}

}


void Animation::play(int x, int y, int dtime,bool isAttacted) {
	timer += dtime;
	if (timer >= interval_time) {
		timer = 0;
		animation_index++;
		if (animation_index >= frame_num) { animation_index = 0; }
	}
	if (isAttacted) {
		IMAGE white;
		int width = atlas->frames[animation_index]->getwidth();
		int height = atlas->frames[animation_index]->getheight();
		Resize(&white, width,height);//加载创建的空图片为指定大小
		DWORD* raw = GetImageBuffer(atlas->frames[animation_index]);//获取原图片像素缓冲区
		DWORD* sketch = GetImageBuffer(&white);//获取新建的图片像素缓冲区
		for(int y =0;y<height;y++){
			for (int x = 0; x < width; x++) {//遍历每一个像素
				int idx = y * width + x;
				if ((raw[idx] & 0xFF000000) >> 24) {//取出alpha通道，如果不为0
					sketch[idx] = BGR(RGB(255, 255, 255)) | (((DWORD)(BYTE)(255)) << 24);//将像素点颜色变为白色
				}
			}
		}
		putimageAlpha(x, y, &white);
	}
	else{ putimageAlpha(x, y, atlas->frames[animation_index]); }
}

void Animation::play(int x, int y, int dtime, bool isAttacted,wstring specialImgPath) {
	timer += dtime;
	if (timer >= interval_time) {
		timer = 0;
		animation_index++;
		if (animation_index >= frame_num) { animation_index = 0; }
	}
	if (isAttacted) {
		IMAGE special;
		int width = atlas->frames[animation_index]->getwidth();
		int height = atlas->frames[animation_index]->getheight();
		loadimage(&special, specialImgPath.c_str(), width, height);
		putimageAlpha(x, y, &special);
	}
	else { putimageAlpha(x, y, atlas->frames[animation_index]); }
}

void Animation::play(int x, int y, int dtime, bool isAttacted, int specialResourceID) {
	timer += dtime;
	if (timer >= interval_time) {
		timer = 0;
		animation_index++;
		if (animation_index >= frame_num) { animation_index = 0; }
	}
	if (isAttacted) {
		IMAGE special;
		int width = atlas->frames[animation_index]->getwidth();
		int height = atlas->frames[animation_index]->getheight();
		loadimage(&special, _T("PNG"), MAKEINTRESOURCE(specialResourceID), width, height);
		putimageAlpha(x, y, &special);
	}
	else { putimageAlpha(x, y, atlas->frames[animation_index]); }
}




//动画集类
Animations::Animations(Animation* front_animation, Animation* right_animation, Animation* left_animation, Animation* behind_animation,Animation* static_animation) {
	Animation* t1 = new Animation(*front_animation);
	Animation* t2 = new Animation(*right_animation);
	Animation* t3 = new Animation(*left_animation);
	Animation* t4 = new Animation(*behind_animation);
	Animation* t5 = new Animation(*static_animation);
	this->front_animation = t1;
	this->right_animation = t2;
	this->left_animation = t3;
	this->behind_animation = t4;
	this->static_animation = t5;
}
Animations::Animations(Animations*ani){
	this->front_animation = new Animation(*ani->front_animation);
	this->right_animation = new Animation(*ani->right_animation);
	this->left_animation = new Animation(*ani->left_animation);
	this->behind_animation = new Animation(*ani->behind_animation);
	this->static_animation = new Animation(*ani->static_animation);
}
//子弹类
void Bullet::draw(int time) {
	if (type == boom) {
		BoomAnimations->behind_animation->play(x - BOOM_IMG_WIDTH / 2, y - BOOM_IMG_HEIGHT / 2, time, false);
	}

}
Bullet::Bullet(bulletType t, int x, int y) {
	type = t;
	this->x = x;
	this->y = y;
	this->BoomAnimations = new Animations(boom_animations);
	if (t == boom) {
		damageRange = 200;
		speed = 82;
		damage = 51;
	}
}
Bullet* createBoom() {
	Bullet* temp = new Bullet(boom, player.x, player.y);
	return temp;
}
//玩家类
void Player::drawShadow() {
	putimageAlpha(x-PLAYER_SHADOW_WIDTH/2, y+PLAYER_IMG_HEIGHT/3, &shadow);
}
void Player::putBoom() {
	if (playerState.BoomNum <= 0) { return; }
	playerState.BoomNum--;
	boom_list.push_back(createBoom());
}
void Player::getAttack() {
	playerState.HP -= 1;
}
void Player::draw(int time) {
	this->drawShadow();
	int player_dir_x = isLeftMove - isRightMove;
	int player_dir_y = isDownMove-isUpMove;

	if (player_dir_x==-1) {
		animations->right_animation->spplay(x - PLAYER_IMG_WIDTH/2, y - PLAYER_IMG_HEIGHT/2,time,player.specialShowTable);
		return;
	}
	else if (player_dir_x == 1) {
		animations->left_animation->spplay(x - PLAYER_IMG_WIDTH / 2, y - PLAYER_IMG_HEIGHT / 2, time, player.specialShowTable);
		return;
	}
	else if (player_dir_y==-1) {
		animations->behind_animation->spplay(x - PLAYER_IMG_WIDTH / 2, y - PLAYER_IMG_HEIGHT / 2,time, player.specialShowTable);
		return;
	}
	else if(player_dir_y == 1){
		animations->front_animation->spplay(x - PLAYER_IMG_WIDTH / 2, y - PLAYER_IMG_HEIGHT / 2, time, player.specialShowTable);
		return;
	}
	else {
		animations->static_animation->spplay(x - PLAYER_IMG_WIDTH / 2, y - PLAYER_IMG_HEIGHT / 2, time, player.specialShowTable);
		return;
	}
}
//敌人类
Enemy::Enemy(Animations* ani) {
	Animations* temp = new Animations(*ani);
	HP = 100;
	speed = 3;
	attactedShowTick = 0;
	animations = temp;
	loadimage(&shadow,_T("PNG"), MAKEINTRESOURCE(RES_SHADOW_ID), PLAYER_SHADOW_WIDTH, PLAYER_SHADOW_HEIGHT);
	enum edge {
		UP =0,
		LEFT,
		DOWN,
		RIGHT,
	};
	edge e = (edge)(rand() % 4);
	if (e == UP) {
		y = -5;
		x = rand() % WINDOW_WIDTH;
	}
	else if (e == DOWN) {
		y = WINDOW_HEIGHT + 5;
		x = rand() % WINDOW_WIDTH;
	}
	else if (e == LEFT) {
		y =rand()%WINDOW_HEIGHT;
		x = -5;
	}
	else if (e == RIGHT) {
		y = rand()%WINDOW_HEIGHT;
		x = WINDOW_WIDTH+5;
	}
}
void Enemy::drawShadow() {
	putimageAlpha(x - ENEMY_SHADOW_WIDTH / 2, y - ENEMY_IMG_HEIGHT/5, &shadow);

}
void Enemy::getDamge(int damage) {
	HP -= damage;
}


//游戏道具类
gameTool::gameTool(int level, int type) {

	x = rand() % (WINDOW_WIDTH - 100)+100;
	y = rand() % (WINDOW_HEIGHT - 100)+100;
	this->level = level;
	toolType list[] = { boomNumBoost ,boomCapBoost,boomRecBoost,hpNumBoost,hpCapBoost,speedNumBoost };
	int timeList[5] = {0,10000,15000,30000,-1};
	int existTimeList[5] = { 0,30000,15000,10000,5000 };
	IMAGE imageList[5] = { NULL,greenBubble,blueBubble,purpleBubble,sevenBubble };
	bubbleImg = imageList[level];
	maintainTime = timeList[level];
	existTime = existTimeList[level];
	if (level >= 4) { isPermanent = true; }
	this->type = list[type-1];
	if (type == boomNumBoost ||type== boomCapBoost ||type== boomRecBoost) {
		gameToolIco = boomIco;
	}
	else if (type == hpNumBoost ||type  == hpCapBoost) {
		gameToolIco = hpIco;
	}
	else { gameToolIco = icecreamIco; }
	if (type == boomCapBoost || type == hpCapBoost) {
		moreIco = boostIco;
	}
	else if (type == boomNumBoost || type == hpNumBoost || type == speedNumBoost) {
		moreIco = plusIco;
	}
	else { moreIco = fastIco; }
	summonTimeStamp = GetTickCount();

}
void gameTool::draw() {
	putimageAlpha(x - TOOL_ICO_WIDTH / 2, y - TOOL_ICO_HEIGHT / 2, &bubbleImg);
	putimageAlpha(x - TOOL_ICO_WIDTH / 2, y - TOOL_ICO_HEIGHT / 2, &moreIco);
	putimageAlpha(x - TOOL_ICO_WIDTH / 2, y - TOOL_ICO_HEIGHT / 2, &gameToolIco);
}
void gameTool::giveEffect() {
	cout << "玩家获得效果";
}

//效果类
effect::effect(toolType type, int level) {
	this->type = type;
	//图标
	if (type == boomCapBoost) { ico = effect_ico_boomnb; }
	else if (type == boomRecBoost) { ico = effect_ico_boomrb; }
	else if (type == hpCapBoost) { ico = effect_ico_orangeb; }
	else if (type == speedNumBoost) { ico = effect_ico_iceb; }
	IMAGE list[] = { NULL,effect_greenbk,effect_bluebk,effect_purplebk,effect_sevenbk };
	int timeList[5] = { 0,10000,15000,30000,-1 };
	int list2[] = { 0,1,2,3,3 };
	//背景颜色
	bk = list[level];
	//持续时间
	maintainTime = timeList[level];

	//边框
	if (level == 4) { border = effect_ifborder; }
	else { border = effect_nmborder; }

	timebar = effect_timebar;
	startTimeStamp = GetTickCount();
	changeNum = list2[level];
}