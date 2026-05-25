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
double len(int x1, int y1, int x2, int y2);

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
	if(sp->tickTable[i]>0&&i<sp->num){//绘制特殊图像
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
		damageRange = 150;
		speed = 82;
		damage = player.playerPersistState.putBoomDamage;
	}
}
Bullet* createBoom() {
	Bullet* temp = new Bullet(boom, player.x, player.y);
	return temp;
}
//玩家类
Player::Player(int, int, Animations* ani, specialTable* sp) {
	x = 300;
	y = 300;
	specialShowTable = sp;
	animations = ani;
	loadimage(&shadow, _T("PNG"), MAKEINTRESOURCE(RES_SHADOW_ID), PLAYER_SHADOW_WIDTH, PLAYER_SHADOW_HEIGHT);
	this->playerPersistState.coinNum = Reg.readInt("coinNum", 0);
}

void Player::update() {
	this->playerState.maxHP = this->playerPersistState.maxHP;
	this->playerState.BoomMaxNum = this->playerPersistState.BoomMaxNum;
	this->playerState.BoomNum = this->playerState.BoomMaxNum;
	this->playerState.BoomRecoverSpeed = this->playerPersistState.BoomRecoverSpeed;
	this->playerState.BoomRecoverProgressMax = this->playerPersistState.BoomRecoverProgressMax;
	this->playerState.speed = this->playerPersistState.speed;
	this->playerPersistState.coinNum = Reg.readInt("coinNum", 0);
}

void Player::drawShadow() {
	putimageAlpha(x-PLAYER_SHADOW_WIDTH/2, y+PLAYER_IMG_HEIGHT/3, &shadow);
}
void Player::putBoom() {
	if (playerState.BoomNum <= 0) { return; }
	//遍历boom_list，判断是否有炸弹在爆炸过程中，如果有则不能放置炸弹
	for (auto& boom : boom_list) {
		float length = len(boom->x, boom->y, player.x, player.y);
		if (length < BOOM_SIZE) {
			return;
		}
	}
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
Enemy::Enemy(Animations* ani, int hp, int spd) {
	Animations* temp = new Animations(*ani);
	HP = hp;
	speed = spd;
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

Enemy::Enemy(Animations* ani, int hp, int spd,int x,int y) {
	Animations* temp = new Animations(*ani);
	HP = hp;
	speed = spd;
	this->x = x;
	this->y = y;
	animations = temp;
	loadimage(&shadow, _T("PNG"), MAKEINTRESOURCE(RES_SHADOW_ID), PLAYER_SHADOW_WIDTH, PLAYER_SHADOW_HEIGHT);
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

//按钮类
Button::Button(int x, int y, int width, int height, wstring imgPath, wstring hoverImgPath, wstring activeImgPath, void* onClickFunc){
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	IMAGE img, hoverImg, activeImg;
	loadimage(&img, imgPath.c_str(), width, height);
	loadimage(&hoverImg, hoverImgPath.c_str(), width, height);
	loadimage(&activeImg, activeImgPath.c_str(), width, height);
	this->img = img;
	this->hoverImg = hoverImg;
	this->activeImg = activeImg;
}
//Button无参构造
Button::Button() {
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	onClickFunc = nullptr;
}

Button::Button(int x, int y, int width, int height, int imgID, int hoverImgID, int activeImgID, void* onClickFunc) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	IMAGE img, hoverImg, activeImg;
	loadimage(&img, _T("PNG"), MAKEINTRESOURCE(imgID), width, height);
	loadimage(&hoverImg, _T("PNG"), MAKEINTRESOURCE(hoverImgID), width, height);
	loadimage(&activeImg, _T("PNG"), MAKEINTRESOURCE(activeImgID), width, height);
	this->img = img;
	this->hoverImg = hoverImg;
	this->activeImg = activeImg;
}
Button::~Button() {
	//析构函数，释放图片资源
	img.~IMAGE();
	hoverImg.~IMAGE();
	activeImg.~IMAGE();
}
void  Button::draw() {
	if (isActive) {
		putimageAlpha(x, y, &activeImg);
	}
	else if (isHover) {
		putimageAlpha(x, y, &hoverImg);
	}
	else {
		putimageAlpha(x, y, &img);
	}
}

Registryer::Registryer() {
	KeyPath = "Software\\LuLuAdventure";
}
//向注册表写入int
bool Registryer::setInt(string key, int value) {
	HKEY hKey;
	DWORD disposition;
	// 创建或打开注册表键
	LONG result = RegCreateKeyExA(
		HKEY_CURRENT_USER,
		KeyPath.c_str(),
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&hKey,
		&disposition
	);

	if (result != ERROR_SUCCESS) {
		return false;
	}

	// 写入整数值
	result = RegSetValueExA(
		hKey,
		key.c_str(),
		0,
		REG_DWORD,
		(const BYTE*)&value,
		sizeof(value)
	);
	RegCloseKey(hKey);
	return result == ERROR_SUCCESS;
}

// 从注册表读取整数
int Registryer::readInt(string valueName, int defaultValue = 0) {
	HKEY hKey;
	DWORD value;
	DWORD valueSize = sizeof(value);
	DWORD type = REG_DWORD;
	// 打开注册表键
	LONG result = RegOpenKeyExA(
		HKEY_CURRENT_USER,
		KeyPath.c_str(),
		0,
		KEY_READ,
		&hKey
	);

	if (result != ERROR_SUCCESS) {
		return defaultValue;
	}
	// 读取整数值
	result = RegQueryValueExA(
		hKey,
		valueName.c_str(),
		NULL,
		&type,
		(LPBYTE)&value,
		&valueSize
	);

	RegCloseKey(hKey);
	if (result != ERROR_SUCCESS) {
		return defaultValue;
	}
	return (int)value;
};




LevelBtn::LevelBtn(int x, int y, int width, int height, int imgID, int hoverImgID, int activeImgID, int lockedImgID, void* onClickFunc, int id) :Button(x, y, width, height, imgID, hoverImgID, activeImgID, onClickFunc) {
	this->id = id;
};

void LevelBtn::draw() {
	if(isLocked){
		IMAGE lockedImg;
		loadimage(&lockedImg, _T("PNG"), MAKEINTRESOURCE(RES_LEVEL_BUTTONS_LOCKED_ID), width, height);
		putimageAlpha(x, y, &lockedImg);
		return;
	}

	if (isActive) {
		putimageAlpha(x, y, &activeImg);
	}
	else if (isHover) {
		putimageAlpha(x, y, &hoverImg);
	}
	else {
		putimageAlpha(x, y, &img);
	}
}

Game::Game(int menuImgBKID, int levelSelectBKID, int playingBKImgID, int loseImgBKID, int winImgBKID, int shopImgBKID) {
	loadimage(&menuImgBK, _T("PNG"), MAKEINTRESOURCE(menuImgBKID), WINDOW_WIDTH, WINDOW_HEIGHT);
	loadimage(&levelSelectBK, _T("PNG"), MAKEINTRESOURCE(levelSelectBKID), WINDOW_WIDTH, WINDOW_HEIGHT);
	loadimage(&playingBKImg, _T("PNG"), MAKEINTRESOURCE(playingBKImgID), WINDOW_WIDTH, WINDOW_HEIGHT);
	loadimage(&loseImgBK, _T("PNG"), MAKEINTRESOURCE(loseImgBKID), WINDOW_WIDTH, WINDOW_HEIGHT);
	loadimage(&winImgBK, _T("PNG"), MAKEINTRESOURCE(winImgBKID), WINDOW_WIDTH, WINDOW_HEIGHT);
	loadimage(&shopImgBK, _T("PNG"), MAKEINTRESOURCE(shopImgBKID), WINDOW_WIDTH, WINDOW_HEIGHT);
	currentLevel = Registryer().readInt("currentLevel", 1);
	

}

bool GoodsBtn::isUnaffordable() {
	if (player.playerPersistState.coinNum < price) { return true; }
	else { return false; }
}

GoodsBtn::GoodsBtn(int x, int y, int width, int height, void* onClickFunc, int id){
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->id = id;
	switch (id) {
	case 1:
		description = "增加最大炸弹数";
		this->price = 200;
		this->priceIncreaseRate = 1.5;
		this->level = Reg.readInt("up_1", 0);
		this->maxLevel = 10;
		this->baseValue = 5;
		this->deltaRate = 1;
		this->deltaNum = 1;
		this->priceIncreaseRate = 1.5;
		this->price = this->price * pow(this->priceIncreaseRate, this->level);
		this->currentValue = this->baseValue + this->deltaNum * this->level;
		this->nextValue = this->baseValue + this->deltaNum * (this->level+1);
		break;
	case 2:
		description = "增加炸弹伤害";
		this->price = 200;
		this->priceIncreaseRate = 1.5;
		this->level = Reg.readInt("up_2", 0);
		this->maxLevel = 9999;
		this->baseValue = 51;
		this->deltaRate = 1.1;
		this->deltaNum = 0;
		this->priceIncreaseRate = 1.5;
		this->price = this->price * pow(this->priceIncreaseRate, this->level);
		this->currentValue = this->baseValue * pow(this->deltaRate, this->level);
		this->nextValue = this->baseValue * pow(this->deltaRate, this->level+1);
		break;
	case 3:
		description = "增加炸弹恢复速度";
		this->price = 500;
		this->priceIncreaseRate = 1.5;
		this->level = Reg.readInt("up_3", 0);
		this->maxLevel = 3;
		this->baseValue = 4;
		this->deltaRate = 1;
		this->deltaNum = 1;
		this->priceIncreaseRate = 1.5;
		this->price = this->price * pow(this->priceIncreaseRate, this->level);
		this->currentValue = this->baseValue - this->deltaNum * this->level;
		this->nextValue =this->baseValue- (this->level+1);
		break;
	case 4:
		this->description = "增加最大HP";
		this->price = 500;
		this->priceIncreaseRate = 1.5;
		this->level = Reg.readInt("up_4", 0);
		this->maxLevel = 5;
		this->baseValue = 5;
		this->deltaNum = 1;
		this->price = this->price * pow(this->priceIncreaseRate, this->level);
		this->currentValue = this->baseValue + this->deltaNum * this->level;
		this->nextValue = this->baseValue + this->deltaNum * (this->level+1);
		break;
	case 5:
		this->description = "增加移动速度";
		this->price = 1000;
		this->priceIncreaseRate = 1.5;
		this->level = Reg.readInt("up_5", 0);
		this->maxLevel = 5;
		this->baseValue = 5;
		this->deltaNum = 1;
		this->price = this->price * pow(this->priceIncreaseRate, this->level);
		this->currentValue = this->baseValue + this->deltaNum * this->level;
		this->nextValue = this->baseValue + this->deltaNum * (this->level+1);
		break;
	}
	IMAGE normalImg;
	IMAGE hoverImg;
	IMAGE activeImg;
	IMAGE maxlevelImg;
	IMAGE unafordableImg;
	loadimage(&normalImg, _T("PNG"), MAKEINTRESOURCE(RES_GOODS_BTN_START_ID+id-1), width, height);
	loadimage(&hoverImg, _T("PNG"), MAKEINTRESOURCE(RES_GOODS_BTN_HOVER_START_ID + id - 1), width, height);
	loadimage(&activeImg, _T("PNG"), MAKEINTRESOURCE(RES_GOODS_BTN_ACTIVE_START_ID + id - 1), width, height);
	loadimage(&maxlevelImg, _T("PNG"), MAKEINTRESOURCE(RES_GOODS_BTN_MAXLEVEL_START_ID + id - 1), width, height);
	loadimage(&unafordableImg, _T("PNG"), MAKEINTRESOURCE(RES_GOODS_BTN_UNAFORDABLE_START_ID + id - 1), width, height);
	this->img = normalImg;
	this->hoverImg = hoverImg;
	this->activeImg = activeImg;
	this->maxLevelImg = maxlevelImg;
	this->unaffordableImg = unafordableImg;
	update();
	playerStateUpdate();
};

void GoodsBtn::draw() {
	if (this->level >= this->maxLevel) {
		putimageAlpha(x, y, &maxLevelImg);
		return;
	}
	else if (isUnaffordable()) {//资金不足
		putimageAlpha(x, y, &unaffordableImg);
		setbkmode(TRANSPARENT); // 关键代码：设置背景模式为透明
		COLORREF fontbk = RGB(77, 46, 26);
		settextcolor(fontbk);
		TCHAR text[128];
		TCHAR text2[128];
		TCHAR text3[128];
		settextstyle(30, 0, _T("宋体"));
		_stprintf_s(text, _T("当前等级：LV%d"), this->level);
		outtextxy(x + 30, y + 20, text);
		settextstyle(24, 0, _T("宋体"));
		_stprintf_s(text2, _T("升级花费：%d"), this->price);
		outtextxy(x + 30, y + BTN_GOODS_HEIGHT - 95, text2);
		_stprintf_s(text3, _T("当前值：%d  -->%d"), this->currentValue, this->nextValue);
		outtextxy(x + 30, y + BTN_GOODS_HEIGHT - 70, text3);
		return;
	}
	else {
		if (isActive) {
			putimageAlpha(x, y, &activeImg);
		}
		else if (isHover) {
			putimageAlpha(x, y, &hoverImg);
		}
		else {
			putimageAlpha(x, y, &img);
		}
		//输出文字
		setbkmode(TRANSPARENT); // 关键代码：设置背景模式为透明
		COLORREF fontbk = RGB(77, 46, 26);
		TCHAR text[128];
		TCHAR text2[128];
		TCHAR text3[128];
		settextcolor(fontbk);
		settextstyle(30, 0, _T("宋体"));
		_stprintf_s(text, _T("当前等级：LV%d"), this->level);
		outtextxy(x+30, y+20, text);
		settextstyle(24, 0, _T("宋体"));
		_stprintf_s(text2, _T("升级花费：%d"), this->price);
		outtextxy(x+30, y + BTN_GOODS_HEIGHT - 95, text2);
		_stprintf_s(text3, _T("当前值：%d  -->%d"), this->currentValue, this->nextValue);
		outtextxy(x+30, y + BTN_GOODS_HEIGHT - 70, text3);
	}
}

void GoodsBtn::buy() {
	if (isUnaffordable() || level >= maxLevel) { return; }
	player.playerPersistState.coinNum -= price;
	level++;
	Reg.setInt("up_" + to_string(id), level);
	Reg.setInt("coinNum", player.playerPersistState.coinNum);
	update();//还需要处理玩家属性的同步
	playerStateUpdate();
}

void GoodsBtn::playerStateUpdate() {
	player.playerState.BoomMaxNum = player.playerPersistState.BoomMaxNum;
	player.playerState.HP = player.playerPersistState.maxHP;
	player.playerState.BoomRecoverSpeed = player.playerPersistState.BoomRecoverSpeed;
	player.playerState.BoomNum = player.playerState.BoomMaxNum;
	player.playerState.maxHP = player.playerPersistState.maxHP;
	player.playerState.speed = player.playerPersistState.speed;

}


void GoodsBtn::update() {
	switch (id) {
	case 1:
		player.playerPersistState.BoomMaxNum = baseValue + deltaNum * level;
		this->currentValue = this->baseValue + this->deltaNum * this->level;
		this->nextValue = this->baseValue + this->deltaNum * (this->level + 1);
		this->price = 200 * pow(this->priceIncreaseRate, this->level);
		player.playerState.BoomMaxNum = player.playerPersistState.BoomMaxNum;
		break;
	case 2:
		player.playerPersistState.putBoomDamage = baseValue * pow(deltaRate, level);
		this->currentValue = this->baseValue * pow(this->deltaRate, this->level);
		this->nextValue = this->baseValue * pow(this->deltaRate, this->level + 1);
		this->price = 500 * pow(this->priceIncreaseRate, this->level);
		break;
	case 3:
		player.playerPersistState.BoomRecoverSpeed = baseValue - deltaNum * level;
		this->currentValue = this->baseValue - this->deltaNum * this->level;
		this->nextValue = this->baseValue - this->deltaNum * (this->level + 1);
		this->price = 1000 * pow(this->priceIncreaseRate, this->level);
		break;
	case 4:
		player.playerPersistState.maxHP = baseValue + deltaNum * level;
		this->currentValue = this->baseValue + this->deltaNum * this->level;
		this->nextValue = this->baseValue + this->deltaNum * (this->level + 1);
		this->price = 2000 * pow(this->priceIncreaseRate, this->level);
		break;
	case 5:
		player.playerPersistState.speed = baseValue + deltaNum * level;
		this->currentValue = this->baseValue + this->deltaNum * this->level;
		this->nextValue = this->baseValue + this->deltaNum * (this->level + 1);
		this->price = 5000 * pow(this->priceIncreaseRate, this->level);
		break;
	}
}

//金币静态图片的构造函数，以后可能会增加动画效果
Coin::Coin(int x, int y,int hp) {
	IMAGE coinImg;
	loadimage(&coinImg, _T("PNG"), MAKEINTRESOURCE(RES_COIN_IMG_ID), COIN_IMG_WIDTH, COIN_IMG_HEIGHT);
	this->staticImg = coinImg;
	this->x = x;
	this->y = y;
	//生成0.15-0.25的随机倍率
	float rate = (rand() % 11 + 15) / 100.0f;
	this->value = floor(hp * rate);
}

void Coin::draw() {
	putimageAlpha(x-COIN_IMG_WIDTH/2, y-COIN_IMG_HEIGHT/2+PLAYER_IMG_HEIGHT / 3, &staticImg);
}