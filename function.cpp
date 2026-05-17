#include "class.h"
#include "function.h"
#include "globle.h"
#include <graphics.h>
#include<string>
//0518主要任务:
//1.完善startGame函数和敌人生成机制
//2.完善玩家过关&失败机制
//3.完善注册表数据存储
using namespace std;
extern Player player;
extern Animations* enemy_animation;
extern Animations* avatar_animation;
extern IMAGE* orange;
extern IMAGE* boomImg;
extern IMAGE* boomBkImg;
extern IMAGE* orangeBkImg;
extern IMAGE greenBubble;
extern IMAGE blueBubble;
extern IMAGE purpleBubble;
extern IMAGE sevenBubble;
extern IMAGE boomIco;
extern IMAGE hpIco;
extern IMAGE icecreamIco;
extern IMAGE plusIco;
extern IMAGE boostIco;
extern IMAGE fastIco;
extern IMAGE effect_nmborder;
extern IMAGE effect_ifborder;
extern IMAGE effect_timebar;
extern IMAGE effect_ico_boomnb;
extern IMAGE effect_ico_boomrb;
extern IMAGE effect_ico_iceb;
extern IMAGE effect_ico_orangeb;
extern IMAGE effect_greenbk;
extern IMAGE effect_bluebk;
extern IMAGE effect_purplebk;
extern IMAGE effect_sevenbk;
extern IMAGE* startImg;
extern IMAGE* endImg;
extern Game game;

int history_max_score = 0;
int score = 0;
int toolsSummonDelay = 0;
int killNumCache = 0;
vector<LevelBtn*> levelBtns;
extern levelInfor levelList[16];

const int LEVEL_BUTTON_START_X = 50;
const int LEVEL_BUTTON_START_Y = 200;
const int LEVEL_BUTTON_WIDTH = 100;
const int LEVEL_BUTTON_HEIGHT = 100;
const int LEVEL_BUTTON_MARGIN = 20;
const int RES_LEVEL_BUTTONS_START_ID = 337;
const int RES_LEVEL_BUTTONS_HOVER_START_ID = 353;
const int RES_LEVEL_BUTTONS_ACTIVE_START_ID = 369;
const int RES_LEVEL_BUTTONS_LOCKED_ID = 385; // 假设没有锁定状态的图片，如果有需要可以添加
const int LEVEL_NUM = 16;
//图片加载以及初始化
void init() {
	//创建关卡选择按钮
	int currentLevel = Reg.readInt("currentLevel", 1);
	for (int i = 0; i < 16; i++) {
		int x = LEVEL_BUTTON_START_X + (i%8) * (LEVEL_BUTTON_WIDTH + LEVEL_BUTTON_MARGIN);
		int y = LEVEL_BUTTON_START_Y+(i/8) * (LEVEL_BUTTON_HEIGHT + LEVEL_BUTTON_MARGIN);
		int w = LEVEL_BUTTON_WIDTH;
		int h = LEVEL_BUTTON_HEIGHT;
		int imgID = RES_LEVEL_BUTTONS_START_ID + i;
		int hoverImgID = RES_LEVEL_BUTTONS_HOVER_START_ID + i;
		int activeImgID = RES_LEVEL_BUTTONS_ACTIVE_START_ID + i;
		int lockedImgID = RES_LEVEL_BUTTONS_LOCKED_ID;
		LevelBtn* temp = new LevelBtn(x, y, h, w, imgID , hoverImgID , activeImgID,lockedImgID, nullptr,i+1);
		temp->id = i;
		if (i <= currentLevel - 1) { temp->isLocked = false; }
		levelBtns.push_back(temp);
	}


	initgraph(1280, 720);
	loadimage(orange, _T("PNG"),MAKEINTRESOURCE(RES_ICO_ORANGE_ID), 32, 32);
	loadimage(orangeBkImg, _T("PNG"),MAKEINTRESOURCE(RES_ICO_ORANGEBK_ID), 32, 32);
	loadimage(boomImg, _T("PNG"),MAKEINTRESOURCE(RES_ICO_BOOM_ID), 32, 48);
	loadimage(boomBkImg, _T("PNG"),MAKEINTRESOURCE(RES_ICO_BOOMBK_ID), 32, 48);
	loadimage(&greenBubble, _T("PNG"),MAKEINTRESOURCE(RES_BUBBLE_GREEN_ID));
	loadimage(&blueBubble, _T("PNG"),MAKEINTRESOURCE(RES_BUBBLE_BLUE_ID));
	loadimage(&purpleBubble, _T("PNG"),MAKEINTRESOURCE(RES_BUBBLE_PURPLE_ID));
	loadimage(&sevenBubble, _T("PNG"),MAKEINTRESOURCE(RES_BUBBLE_SEVEN_ID));
	loadimage(&boomIco, _T("PNG"),MAKEINTRESOURCE(RES_BUBBLE_BOOM_ID));
	loadimage(&hpIco, _T("PNG"),MAKEINTRESOURCE(RES_BUBBLE_HP_ID));
	loadimage(&icecreamIco, _T("PNG"),MAKEINTRESOURCE(RES_BUBBLE_ICECREAM_ID));
	loadimage(&boostIco, _T("PNG"),MAKEINTRESOURCE(RES_BUBBLE_BOOST_ID));
	loadimage(&plusIco, _T("PNG"),MAKEINTRESOURCE(RES_BUBBLE_PLUS_ID));
	loadimage(&fastIco, _T("PNG"),MAKEINTRESOURCE(RES_BUBBLE_FAST_ID));
	loadimage(&effect_nmborder, _T("PNG"),MAKEINTRESOURCE(RES_EFFECT_NMBORDER_ID));
	loadimage(&effect_ifborder, _T("PNG"),MAKEINTRESOURCE(RES_EFFECT_IFBORDER_ID));
	loadimage(&effect_timebar, _T("PNG"),MAKEINTRESOURCE(RES_EFFECT_TIMEBAR_ID));
	loadimage(&effect_greenbk, _T("PNG"),MAKEINTRESOURCE(RES_EFFECT_GREENBK_ID));
	loadimage(&effect_bluebk, _T("PNG"),MAKEINTRESOURCE(RES_EFFECT_BLUEBK_ID));
	loadimage(&effect_purplebk, _T("PNG"),MAKEINTRESOURCE(RES_EFFECT_PURPLEBK_ID));
	loadimage(&effect_sevenbk, _T("PNG"),MAKEINTRESOURCE(RES_EFFECT_SEVENBK_ID));
	loadimage(&effect_ico_boomnb, _T("PNG"),MAKEINTRESOURCE(RES_EFFECT_ICO_BOOMNB_ID));
	loadimage(&effect_ico_boomrb, _T("PNG"),MAKEINTRESOURCE(RES_EFFECT_ICO_BOOMRB_ID));
	loadimage(&effect_ico_orangeb, _T("PNG"),MAKEINTRESOURCE(RES_EFFECT_ICO_ORANGEB_ID));
	loadimage(&effect_ico_iceb, _T("PNG"),MAKEINTRESOURCE(RES_EFFECT_ICO_ICEB_ID));
	loadimage(startImg, _T("PNG"), MAKEINTRESOURCE(RES_MENUBK_ID), WINDOW_WIDTH, WINDOW_HEIGHT);
	loadimage(endImg, _T("PNG"), MAKEINTRESOURCE(RES_END_IMG_ID), WINDOW_WIDTH, WINDOW_HEIGHT);



};

#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"MSIMG32.LIB")
ExMessage msg;
vector<Enemy*> enemy_list;
extern vector<Bullet*> boom_list;
extern vector<gameTool*> tools_list;

void putimageAlpha(int x, int y, IMAGE* img) {
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}
void putimageAlpha(int dstX, int dstY, int dstW, int dstH,IMAGE* src, int srcX, int srcY) {
	AlphaBlend(GetImageHDC(NULL), dstX, dstY, dstW, dstH,
		GetImageHDC(src), srcX, srcY, dstW, dstH,
		{ AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

void summonEnemy() {
	static int count = 0;
	count++;
	if (count >= 120) {
		Animations* temp = new Animations(enemy_animation);
		Enemy* t = new Enemy(temp);
		enemy_list.push_back(t);
		count = 0;
	}
}


Button startButton(950, 300, 280, 120, RES_BUTTON_START_ID, RES_BUTTON_START_HOVER_ID, RES_BUTTON_START_ACTIVE_ID, nullptr);
Button exitButton(950, 450, 280, 120, RES_BUTTON_EXIT_ID, RES_BUTTON_EXIT_HOVER_ID, RES_BUTTON_EXIT_ACTIVE_ID, nullptr);
vector<Button*> buttonList = { &startButton, &exitButton };

void startGame(int level) {
	game.currentLevel = level;
	game.state = Game::playing;
}

void drawMenu(int hss) {
	BeginBatchDraw();
	cleardevice();
	putimageAlpha(0, 0, startImg);
	//遍历按钮数组
	for (Button* button : buttonList) {
		button->draw();
	}
	//显示历史最高分
	setbkmode(TRANSPARENT); // 关键代码：设置背景模式为透明
	COLORREF fontbk = RGB(255, 255, 0);
	TCHAR scoreShow[128];
	settextcolor(fontbk);
	settextstyle(45, 22, _T("宋体"));
	_stprintf_s(scoreShow, _T("历史最高分：%d"), hss);
	outtextxy(WINDOW_WIDTH - HISTORY_SCORE_SHOW_MARGIN_RIGHT, HISTORY_SCORE_SHOW_MARGIN_TOP, scoreShow);
	settextstyle(16, 8, _T("宋体"));
	settextcolor(RGB(255, 255, 255));
	outtextxy(1100, 200, _T("Version 1.1.0 (build2)"));
	EndBatchDraw();
}

void drawEnemy(int time,int y) {
	for (Enemy* enemy : enemy_list) {
		if(y==-1&&enemy->y-player.y<=0){
			enemy->drawShadow();
			if (enemy->x - player.x > 0) {
				enemy->animations->left_animation->play(enemy->x - PLAYER_IMG_WIDTH / 2, enemy->y - PLAYER_IMG_HEIGHT / 2, time, enemy->isAttacked);
			}
			else if (enemy->x - player.x < 0) {
				enemy->animations->right_animation->play(enemy->x - PLAYER_IMG_WIDTH / 2, enemy->y - PLAYER_IMG_HEIGHT / 2, time, enemy->isAttacked);
			}
		}
		else if(y==1&&enemy->y-player.y>0){
			enemy->drawShadow();
			if (enemy->x - player.x > 0) {
				enemy->animations->left_animation->play(enemy->x - PLAYER_IMG_WIDTH / 2, enemy->y - PLAYER_IMG_HEIGHT / 2, time, enemy->isAttacked);
			}
			else if (enemy->x - player.x < 0) {
				enemy->animations->right_animation->play(enemy->x - PLAYER_IMG_WIDTH / 2, enemy->y - PLAYER_IMG_HEIGHT / 2, time, enemy->isAttacked);
			}
		}
	}

}

void enemyMove(){
	//敌人移动和受击状态和攻击玩家
	for (Enemy* enemy : enemy_list) {
		int dx = player.x - enemy->x;
		int dy = player.y - enemy->y;
		double len = sqrt(dx * dx + dy * dy);
		enemy->x += (int)(enemy->speed * (dx / len));
		enemy->y += (int)(enemy->speed * (dy / len));

		if (enemy->isAttacked) {
			enemy->attactedShowTick++;
			if (enemy->attactedShowTick % 6 == 0) {
				enemy->isAttacked = false;
			}
		}


	}

	//敌人清除
	for (auto it = enemy_list.begin(); it != enemy_list.end(); ) {
		Enemy* enemy = *it;
		int dx = player.x - enemy->x;
		int dy = enemy->y-player.y;
		double len = sqrt(dx * dx + dy * dy);
		if (abs(dx)<PLAYER_IMG_WIDTH/2&&((dy<PLAYER_IMG_HEIGHT/2&&dy>0)||(dy<0&&abs(dy)<PLAYER_IMG_HEIGHT/8 ))){
			player.specialShowTable->tickTable[0] += 4;
			player.specialShowTick += 16;
			player.getAttack();
			delete enemy;
			it = enemy_list.erase(it);
			
			continue;
		}


		// 删除指针指向的对象并移除vector中的元素
		if (enemy->HP <= 0) {

			player.playerState.BoomNum < player.playerState.BoomMaxNum - 1 ? player.playerState.BoomNum++ : player.playerState.BoomRecoverProgress = player.playerState.BoomRecoverProgressMax - 1;
			delete enemy;
			it = enemy_list.erase(it);
			score++;
			killNumCache++;
			player.killShowTick = 60;
		}
		else {
			++it;
		}
	}
}

void drawBoom(DWORD time) {
	for (Bullet* boom : boom_list) {
		boom->draw(time);
	}

}

void drawHead(int t) {
	//特殊头像显示
	avatar_animation->behind_animation->play(10,10,t,player.specialShowTick,RES_LULUBEATTACTED_ID);



	if (player.specialShowTick) { player.specialShowTick--; }
	for (int i = 0; i < player.playerState.maxHP; i++) {
		putimageAlpha(30 + AVATAR_IMG_WIDTH + (i * (ORANGE_IMG_WIDTH + 10)), 10, orangeBkImg);
	}
	for (int i = 0; i < player.playerState.HP; i++) {
		putimageAlpha(30 + AVATAR_IMG_WIDTH+(i*(ORANGE_IMG_WIDTH+10)), 10, orange);
	}
	int j;
	for (int k = 0; k < player.playerState.BoomMaxNum; k++) {
		putimageAlpha(30 + AVATAR_IMG_WIDTH + (k * (ORANGE_IMG_WIDTH + 10)), 30 + ORANGE_IMG_HEIGHT, boomBkImg);
	}
	for (j = 0; j< player.playerState.BoomNum; j++) {
		putimageAlpha(30 + AVATAR_IMG_WIDTH + (j * (ORANGE_IMG_WIDTH + 10)), 30+ORANGE_IMG_HEIGHT, boomImg);
	}
	//j此时为炸弹数量
		IMAGE s;
		int all = player.playerState.BoomRecoverProgressMax;
		int p = player.playerState.BoomRecoverProgress;
		int width = boomImg->getwidth();
		int height = boomImg->getheight();
		Resize(&s, width, height);//加载创建的空图片为指定大小
		DWORD* raw = GetImageBuffer(boomImg);//获取原图片像素缓冲区
		DWORD* sketch = GetImageBuffer(&s);//获取新建的图片像素缓冲区
		for (int y = 0; y < height; y++) {
			float temp = width * p/all;
			for (int x = 0; x < temp; x++) {//遍历像素
				int idx = y * width + x;
				sketch[idx] = raw[idx];
			}
		}
		putimageAlpha(30 + AVATAR_IMG_WIDTH + (j * (ORANGE_IMG_WIDTH + 10)), 30 + ORANGE_IMG_HEIGHT, &s);
		TCHAR scoreShow[128];
		//得分显示
		static COLORREF fontbk = RGB(255, 255, 255);
		if(player.killShowTick%7==0&&player.killShowTick){fontbk = RGB(255, 255, 255); }
		if(player.killShowTick%14==0&&player.killShowTick){fontbk = RGB(255, 0, 0); }
		if (player.killShowTick) { player.killShowTick--; }
		setbkmode(TRANSPARENT); // 关键代码：设置背景模式为透明
		settextcolor(fontbk);
		settextstyle(30, 15,_T("宋体"));
		_stprintf_s(scoreShow,_T("得分：%d"),score);
		outtextxy(WINDOW_WIDTH - SCORE_SHOW_MARGIN_RIGHT, SCORE_SHOW_MARGIN_TOP, scoreShow);

	}

void drawTools() {
	for (gameTool* tool : tools_list) {
		tool->draw();
	}
}

vector<effect*> effectList;
void drawEffect() {
	int i = 0;
	for (effect* t : effectList) {
		int x = WINDOW_WIDTH - (EFFECT_IMG_WIDTH * 1.5);
		int y = 100 + (EFFECT_IMG_HEIGHT + 10) * i;

		// 绘制背景、边框、图标
		putimageAlpha(x, y, &t->bk);
		putimageAlpha(x, y, &t->border);
		putimageAlpha(x, y, &t->ico);

		float process = (GetTickCount() - t->startTimeStamp) / (float)t->maintainTime;
		if (process < 1.0f) {
			int barH = EFFECT_IMG_HEIGHT * (1.0f - process);      // 剩余高度
			// 用裁剪区域绘制原图的下半部分（或按你的设计调整源坐标）
			putimageAlpha(x, y + EFFECT_IMG_HEIGHT - barH,
				EFFECT_IMG_WIDTH, barH,
				&effect_timebar, 0, EFFECT_IMG_HEIGHT - barH);
		}
		i++;
	}
}

void effectCalc() {
	int i = 0;
	for (auto it = effectList.begin(); it != effectList.end(); ) {
		effect* t = *it;
		if ((GetTickCount() > (t->maintainTime + t->startTimeStamp))&&(t->maintainTime>0)) {
			// 1. 先恢复玩家状态
			if (t->type == boomCapBoost) {
				player.playerState.BoomMaxNum -= t->changeNum;
			}
			else if (t->type == boomRecBoost) {
				player.playerState.BoomRecoverSpeed += t->changeNum;
			}
			else if (t->type == hpCapBoost) {
				player.playerState.maxHP -= t->changeNum;
			}
			else if (t->type == speedNumBoost) {
				player.playerState.speed -= t->changeNum;
			}
			// 2. 释放对象（如果是 new 出来的）
			delete t;
			// 3. 从容器中移除，并让 it 指向下一个元素
			it = effectList.erase(it);
		}
		else {
			++it;
		}
	}
}

void drawPlaying(DWORD time) {
	IMAGE bk;
	loadimage(&bk, _T("PNG"), MAKEINTRESOURCE(RES_BACKGROUND_ID), WINDOW_WIDTH, WINDOW_HEIGHT);
	putimage(0, 0, &bk);
	int t = time;
	drawTools();
	drawBoom(t);
	//绘制玩家后方敌人(y-方向)
	drawEnemy(t,-1);
	player.draw(t);
	//绘制玩家前方敌人(y+方向)
	drawEnemy(t,1);
	drawHead(t);
	drawEffect();
};

void drawLevelSelect() {
	IMAGE bk;
	loadimage(&bk, _T("PNG"), MAKEINTRESOURCE(RES_LEVEL_BK_ID), WINDOW_WIDTH, WINDOW_HEIGHT);
	putimage(0, 0, &bk);
	for (LevelBtn* btn : levelBtns) {
		btn->draw();
	}
}

void operate() {
	while (peekmessage(&msg)) {
		if (msg.message == WM_KEYDOWN) {
			switch (msg.vkcode) {
			case 'W':
				player.isUpMove = true;
				break;
			case 'A':
				player.isLeftMove = true;
				break;
			case 'S':
				player.isDownMove = true;
				break;
			case 'D':
				player.isRightMove = true;
				break;
			case 'J':
				player.putBoom();
				break;
			}
		}
		else if (msg.message == WM_KEYUP) {
			switch (msg.vkcode) {
			case 'W':
				player.isUpMove = false;
				break;
			case 'A':
				player.isLeftMove = false;
				break;
			case 'S':
				player.isDownMove = false;
				break;
			case 'D':
				player.isRightMove = false;
				break;
			}

		}
}
};

void boomBoom() {
	for (auto it = boom_list.begin(); it != boom_list.end(); ) {
		Bullet* boom = *it;
		boom->speed--;
		// 删除指针指向的对象并移除vector中的元素
		if (boom->speed < 0) {
			delete boom;
			it = boom_list.erase(it);
		}else if(boom->speed==12){
			//遍历敌人列表，判断哪些敌人收到攻击
			for (Enemy* enemy : enemy_list) {
				int dx = boom->x - enemy->x;
				int dy = boom->y - enemy->y;
				int distance = sqrt(dx * dx + dy * dy);
				if (distance < boom->damageRange) {
					enemy->isAttacked = true;
					enemy->getDamge(boom->damage);
				}
			}
		}
		else {
			++it;
		}
	}
}

void boomRecover() {
	if (player.playerState.BoomRecoverSpeed <= 0) { player.playerState.BoomRecoverSpeed = 1; }
	if (player.playerState.BoomMaxNum <= player.playerState.BoomNum) { return; }
	if (player.playerState.BoomRecoverProgress == player.playerState.BoomRecoverProgressMax) {
		player.playerState.BoomNum++;
		player.playerState.BoomRecoverProgress = 0;
	};
	player.playerState.BoomRecoverTick++;
	if (player.playerState.BoomRecoverTick % player.playerState.BoomRecoverSpeed == 0) {
		player.playerState.BoomRecoverProgress++;
	}
}

void summonTool() {
	if (killNumCache >= 3) {//#####此处更改
		killNumCache = 0;
		toolsSummonDelay = rand() % 300+1;
	}
	if (toolsSummonDelay) {
		toolsSummonDelay--;
	}
	if (toolsSummonDelay == 1) {
		int type = rand() % 6 +1;
		int levelProbability = rand() % 100 +1;
		int level = 0;
		if (levelProbability<20) {
			level = 1;
		}
		else if (levelProbability >= 20 && levelProbability < 50) {
			level = 2;
		}
		else if (levelProbability >= 50 && levelProbability < 70) {
			level = 3;
		}
		else {
			level = 4;
		}
		gameTool* t = new gameTool(level,type);
		tools_list.push_back(t);
	}
}

double len(int x1, int y1, int x2, int y2) {
	int dx = x1 - x2;
	int dy = y1 - y2;
	double len = sqrt(dx * dx + dy * dy);
	return len;

}

void addEffect(gameTool* t) {

	int List[4] = { 1,2,3,4 };
	if (t->type == boomNumBoost) { player.playerState.BoomNum += List[t->level-1]; return; }
	if (t->type == hpNumBoost) { player.playerState.HP += List[t->level-1]; return; }
	effect* after = new effect(t->type, t->level);
	if (after->type == boomCapBoost) { player.playerState.BoomMaxNum += List[t->level - 1]; }
	else if (after->type == boomRecBoost) { player.playerState.BoomRecoverSpeed -= 2; }
	else if (after->type == hpCapBoost) { player.playerState.maxHP += List[t->level - 1]; }
	else if (after->type == speedNumBoost) { player.playerState.speed += 2; }
	effectList.push_back(after);
}

void toolCalc() {
	//遍历泡泡链表
	for (auto it = tools_list.begin(); it != tools_list.end();) {
		gameTool* t = *it;
		//清除超时的泡泡
		if (GetTickCount() > t->summonTimeStamp + t->existTime) {
			it = tools_list.erase(it);
			delete t;
			continue;
		}
		else if(len(t->x,t->y,player.x,player.y+PLAYER_IMG_HEIGHT/2)<=20){
			//玩家碰到泡泡
			player.specialShowTable->tickTable[t->level] += 4;
			addEffect(t);
			it = tools_list.erase(it);
			delete t;
			continue;
		
		}
		else {
			it++;
		}
	}
		
}

void beyondControl() {
	if (player.playerState.HP < 0) { player.playerState.HP = 0; }
	if (player.playerState.HP > player.playerState.maxHP) { player.playerState.HP = player.playerState.maxHP; }
}

void calcPlayerMove(){
	int player_dir_x = player.isRightMove - player.isLeftMove;
	int player_dir_y = player.isDownMove - player.isUpMove;
	double xxx = player_dir_x / sqrt(player_dir_x * player_dir_x + player_dir_y * player_dir_y);
	double yyy = player_dir_y / sqrt(player_dir_x * player_dir_x + player_dir_y * player_dir_y);
	if (player_dir_x * player_dir_x + player_dir_y * player_dir_y != 0) {
		player.x += (int)(xxx * player.playerState.speed);
		player.y += (int)(yyy * player.playerState.speed);
	}
	if (player.x < PLAYER_IMG_WIDTH / 2) { player.x = PLAYER_IMG_WIDTH / 2; }
	else if (player.x > WINDOW_WIDTH - PLAYER_IMG_WIDTH / 2) { player.x = WINDOW_WIDTH - PLAYER_IMG_WIDTH / 2; }
	if (player.y < PLAYER_IMG_HEIGHT / 2) { player.y = PLAYER_IMG_HEIGHT / 2; }
	else if (player.y > WINDOW_HEIGHT - PLAYER_IMG_HEIGHT / 2) {
		player.y = WINDOW_HEIGHT - PLAYER_IMG_HEIGHT / 2;
	}
}

void resetGame() {
	//重置玩家状态
	player.playerState.HP = 5;
	player.playerState.maxHP = 5;
	player.playerState.speed = 5;
	player.playerState.BoomNum = 5;
	player.playerState.BoomMaxNum = 5;
	player.playerState.BoomRecoverSpeed = 4;
	player.playerState.BoomRecoverProgress = 0;
	player.playerState.BoomRecoverProgressMax = 50;
	player.playerState.BoomRecoverTick = 0;
	//清除敌人
	for (Enemy* enemy : enemy_list) {
		delete enemy;
	}
	enemy_list.clear();
	//清除炸弹
	for (Bullet* boom : boom_list) {
		delete boom;
	}
	boom_list.clear();
	//清除道具
	for (gameTool* tool : tools_list) {
		delete tool;
	}
	tools_list.clear();
	//清除效果
	for (effect* t : effectList) {
		delete t;
	}
	effectList.clear();
	score = 0;
	//重置玩家位置
	player.x = 300;
	player.y = 300;

}

void calcLose() {
	if (GetAsyncKeyState('R')) { game.state = Game::playing; }
	if (GetAsyncKeyState(VK_ESCAPE)) { game.state = Game::menu; }
}

void temp_isGameEnd() {
	if (player.playerState.HP <= 0) {
		if (score > history_max_score) { history_max_score = score; }
		game.state = Game::gameover;
		resetGame();
	}
}

void calcPlaying() {
	summonEnemy();
	summonTool();
	toolCalc();
	boomBoom();
	enemyMove();
	boomRecover();
	effectCalc();
	beyondControl();
	calcPlayerMove();
	temp_isGameEnd();
};

void calcMenu() {
	while (peekmessage(&msg)) {
		// 获取鼠标位置
		if (msg.message == WM_MOUSEMOVE || msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONUP) {
			int mx = msg.x;
			int my = msg.y;
			for (Button* button : buttonList) {
				// 判断鼠标是否在按钮上
				if (mx >= button->x && mx <= button->x + button->width && my >= button->y && my <= button->y + button->height) {
					button->isHover = true;
					// 处理点击逻辑
					if (msg.message == WM_LBUTTONDOWN) {
						button->isActive = true;
					}
					else if (msg.message == WM_LBUTTONUP) {
						if (button->isActive) {
							if (button == &startButton) {
								game.state = Game::selectLevel;
							}
							else if (button == &exitButton) {
								exit(0);
							}
						}
						button->isActive = false;
					}
				}
				else {
					button->isHover = false;
					button->isActive = false;
				}
			}
		}
	}
}

void calcSelectLevel() {
	while (peekmessage(&msg)) {
		if (msg.message == WM_MOUSEMOVE || msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONUP) {
			int mx = msg.x;
			int my = msg.y;
			for (LevelBtn* btn : levelBtns) {
				if (mx >= btn->x && mx <= btn->x + btn->width && my >= btn->y && my <= btn->y + btn->height) {
					btn->isHover = true;
					if (msg.message == WM_LBUTTONDOWN) {
						btn->isActive = true;
					}
					else if (msg.message == WM_LBUTTONUP) {
						if (btn->isActive && !btn->isLocked) {
							//开始关卡传参
							startGame(btn->id);
						}
						btn->isActive = false;
					}
				}
				else {
					btn->isHover = false;
					btn->isActive = false;
				}
			}
		}
	}
}

void calc() {
	if (game.state == Game::menu) {
		calcMenu();
	}
	else if (game.state == Game::selectLevel) {
		calcSelectLevel();
	}
	else if (game.state == Game::playing) {
		operate();
		calcPlaying();
	}
	else if (game.state == Game::gameover) {
		calcLose();
	}
}

void drawGameover() {
	Sleep(100);

	putimageAlpha(0, 0, endImg);
	setbkmode(TRANSPARENT); // 关键代码：设置背景模式为透明
	COLORREF fontbk = RGB(255, 0, 0);
	TCHAR scoreShow[128];
	TCHAR scoreShowH[128];
	settextcolor(fontbk);
	settextstyle(100, 50, _T("宋体"));
	_stprintf_s(scoreShow, _T("你的得分：%d"), score);
	_stprintf_s(scoreShowH, _T("历史最高分：%d"), history_max_score);
	outtextxy(200, 200, scoreShow);
	outtextxy(200, 300, scoreShowH);
}

void draw(DWORD time) {
	BeginBatchDraw();
	cleardevice();
	if (game.state == Game::menu) {
		drawMenu(history_max_score);
	}
	else if (game.state == Game::playing) {
		drawPlaying(time);
		cout <<"draw playing" << endl;
	}
	else if (game.state == Game::selectLevel) {
		drawLevelSelect();
	}
	else if (game.state == Game::gameover) {
		drawGameover();
	}


	EndBatchDraw();
}
