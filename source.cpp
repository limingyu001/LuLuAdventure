#include <iostream>
#include <graphics.h>
#include "class.h"
#include "globle.h"
#include "function.h"
#include "ctime"
#include "resource.h"
using namespace std;

//最后两个参数是win图片和shop图片，暂时用menu图片占位
Game game(RES_MENUBK_ID, RES_LEVEL_BK_ID,RES_BACKGROUND_ID, RES_END_IMG_ID, RES_WIN_BK_ID, RES_MENUBK_ID);
IMAGE* orange = new IMAGE;
IMAGE* orangeBkImg = new IMAGE;
IMAGE* boomImg = new IMAGE;
IMAGE* boomBkImg = new IMAGE;
Atlas* player_static_atlas = new Atlas(20, RES_PLAYER_STATIC_START, PLAYER_IMG_WIDTH, PLAYER_IMG_HEIGHT);
Atlas* player_front_atlas = new Atlas(12, RES_PLAYER_FRONT_START, PLAYER_IMG_WIDTH, PLAYER_IMG_HEIGHT);
Atlas* player_right_atlas = new Atlas(16, RES_PLAYER_RIGHT_START, PLAYER_IMG_WIDTH, PLAYER_IMG_HEIGHT);
Atlas* player_left_atlas = new Atlas(16, RES_PLAYER_LEFT_START, PLAYER_IMG_WIDTH, PLAYER_IMG_HEIGHT);
Atlas* player_behind_atlas = new Atlas(12, RES_PLAYER_BEHIND_START, PLAYER_IMG_WIDTH, PLAYER_IMG_HEIGHT);
Atlas* enemy_right_atlas = new Atlas(24, RES_ENEMY_RIGHT_START, ENEMY_IMG_WIDTH, ENEMY_IMG_HEIGHT);
Atlas* enemy_left_atlas = new Atlas(24, RES_ENEMY_LEFT_START, ENEMY_IMG_WIDTH, ENEMY_IMG_HEIGHT);
Atlas* enemy_right_died_atlas = new Atlas(50, RES_ENEMY_DIED_RIGHT_START_ID, ENEMY_IMG_WIDTH, ENEMY_IMG_HEIGHT);
Atlas* enemy_left_died_atlas = new Atlas(50, RES_ENEMY_DIED_LEFT_START_ID, ENEMY_IMG_WIDTH, ENEMY_IMG_HEIGHT);
Atlas* bomm_atlas = new Atlas(42, RES_BOOM_START, BOOM_IMG_WIDTH, BOOM_IMG_HEIGHT);
Atlas* lulu_circle_atlas = new Atlas(20, RES_PLAYER_CIRCLE_START, AVATAR_IMG_WIDTH, AVATAR_IMG_HEIGHT);
Animation* s = new Animation(player_static_atlas, 50);
Animation* f = new Animation(player_front_atlas, 30);
Animation* r = new Animation(player_right_atlas, 30);
Animation* l = new Animation(player_left_atlas, 30);
Animation* b = new Animation(player_behind_atlas, 30);
Animation* er = new Animation(enemy_right_atlas, 30);
Animation* el = new Animation(enemy_left_atlas, 30);
Animation* boom_animation = new Animation(bomm_atlas, 30);
Animation* l_c_a = new Animation(lulu_circle_atlas, 50);
Animation* er_died_animation = new Animation(enemy_right_died_atlas, 20);
Animation* el_died_animation = new Animation(enemy_left_died_atlas, 20);
Animations* avatar_animation = new Animations(l_c_a, l_c_a, l_c_a, l_c_a, l_c_a);
Animations* boom_animations = new Animations(boom_animation, boom_animation, boom_animation, boom_animation, boom_animation);
Animations* player_animation = new Animations(f, r, l, b, s);
Animations* enemy_animation = new Animations(er, er, el, el, el);
Animations* enemy_died_right_animations = new Animations(er_died_animation, er_died_animation, er_died_animation, er_died_animation, er_died_animation);
Animations* enemy_died_left_animations = new Animations(el_died_animation, el_died_animation, el_died_animation, el_died_animation, el_died_animation);
int rt[5] = { 255,148,103,152,242 };
int gt[5] = { 255,228,157,82,255 };
int bt[5] = { 255,68,255,179,0 };
specialTable* sp = new specialTable(4, rt, gt, bt);
Player player(5, 5, player_animation, sp);

Registryer Reg;

//id//敌人血量//敌人数量//敌人速度//是否解锁
levelInfor levelList[16] = {
levelInfor{ 1, 50, 3, 2, true},
levelInfor{ 2, 50, 5, 2, false },
levelInfor{ 3, 100, 5, 2, false },
levelInfor{ 4, 100, 7, 3, false },
levelInfor{ 5, 150, 7, 3, false },
levelInfor{ 6, 150, 10, 3, false },
levelInfor{ 7, 150, 15, 3, false },
levelInfor{ 8, 200, 10, 3, false },
levelInfor{ 9, 250, 10, 3, false },
levelInfor{ 10, 300, 10, 3, false },
levelInfor{ 11, 500, 15, 4, false },
levelInfor{ 12, 600, 10, 4, false },
levelInfor{ 13, 700, 10, 5, false },
levelInfor{ 14, 800, 15, 6, false },
levelInfor{ 15, 900, 20, 6, false },
levelInfor{ 16, 1000, 20, 6, false },

};


int main(){
	srand(static_cast<unsigned int>(time(0)));
	init();
	resetGame();
	while (TRUE) {
		DWORD startTick = GetTickCount();//获取开始时间戳
		static int gameTick = 0;
		calc();
		gameTick++;
		DWORD endTick = GetTickCount();//获取一轮的结束时间戳
		draw((1000 / 60) - endTick + startTick);
		if (endTick - startTick < 1000 / 60) { Sleep((1000 / 60) - endTick + startTick); }
		else {cout << "FPS low" << endl; }//如果1/FPS秒内执行完成就休眠直到1/120秒结束
	}
	return 0;
}