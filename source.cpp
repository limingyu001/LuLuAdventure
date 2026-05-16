#include <iostream>
#include <graphics.h>
#include "class.h"
#include "globle.h"
#include "function.h"
#include "ctime"
#include "resource.h"
using namespace std;
IMAGE* orange = new IMAGE;
IMAGE* orangeBkImg = new IMAGE;
IMAGE* boomImg = new IMAGE;
IMAGE* boomBkImg = new IMAGE;
IMAGE* startImg = new IMAGE;
IMAGE* endImg = new IMAGE;
Atlas* player_static_atlas = new Atlas(20, RES_PLAYER_STATIC_START, PLAYER_IMG_WIDTH, PLAYER_IMG_HEIGHT);
Atlas* player_front_atlas = new Atlas(12, RES_PLAYER_FRONT_START, PLAYER_IMG_WIDTH, PLAYER_IMG_HEIGHT);
Atlas* player_right_atlas = new Atlas(16, RES_PLAYER_RIGHT_START, PLAYER_IMG_WIDTH, PLAYER_IMG_HEIGHT);
Atlas* player_left_atlas = new Atlas(16, RES_PLAYER_LEFT_START, PLAYER_IMG_WIDTH, PLAYER_IMG_HEIGHT);
Atlas* player_behind_atlas = new Atlas(12, RES_PLAYER_BEHIND_START, PLAYER_IMG_WIDTH, PLAYER_IMG_HEIGHT);
Atlas* enemy_right_atlas = new Atlas(24, RES_ENEMY_RIGHT_START, ENEMY_IMG_WIDTH, ENEMY_IMG_HEIGHT);
Atlas* enemy_left_atlas = new Atlas(24, RES_ENEMY_LEFT_START, ENEMY_IMG_WIDTH, ENEMY_IMG_HEIGHT);
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
Animations* avatar_animation = new Animations(l_c_a, l_c_a, l_c_a, l_c_a, l_c_a);
Animations* boom_animations = new Animations(boom_animation, boom_animation, boom_animation, boom_animation, boom_animation);
Animations* player_animation = new Animations(f, r, l, b, s);
Animations* enemy_animation = new Animations(er, er, el, el, el);
int rt[5] = { 255,148,103,152,242 };
int gt[5] = { 255,228,157,82,255 };
int bt[5] = { 255,68,255,179,0 };
specialTable* sp = new specialTable(4, rt, gt, bt);
Player player(5, 5, player_animation, sp);

bool FLAG1 = true;
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	init();
	Registryer Reg;
	while (TRUE) {
		srand(static_cast<unsigned int>(time(0)));
		//开始界面
		extern int history_max_score;
		history_max_score = Reg.readInt("historyMaxScore", 0);
		while (FLAG1) {
			if (GetAsyncKeyState(VK_RETURN)) { FLAG1 = false; }
			calcMenu();
			drawMenu(history_max_score);
			//绘制开始界面
			//cleardevice();
			//putimageAlpha(0, 0, startImg);
			//settextcolor(RGB(255, 255, 255));
			//settextstyle(50, 0, _T("Arial"));
			//outtextxy(200, 200, _T("Press Enter to Start"));
			Sleep(100);
		}
	
		resetGame();
		while (player.playerState.HP) {
			DWORD startTick = GetTickCount();//获取开始时间戳
			static int gameTick = 0;
			operate();
			calc();
			gameTick++;
			DWORD endTick = GetTickCount();//获取一轮的结束时间戳
			draw((1000 / 60) - endTick + startTick);
			if (endTick - startTick < 1000 / 60) { Sleep((1000 / 60) - endTick + startTick); }//如果1/FPS秒内执行完成就休眠直到1/120秒结束
		}
		extern int score;
		if(score > history_max_score) {
			history_max_score = score;
			Reg.setInt("historyMaxScore", history_max_score);
		}
		bool FLAG2 = true;
		//结束界面
		//按下R键重新开始，按下ESC键退出
		while (FLAG2) {
			if (GetAsyncKeyState('R')) { FLAG2 = false; }
			if (GetAsyncKeyState(VK_ESCAPE)) { exit(0); }
			endPage();
			
		}

		
	}
	return 0;
}