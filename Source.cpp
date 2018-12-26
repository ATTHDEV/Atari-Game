#include "GLHelper.h"
var speedX = 0.0, rBall = 0.05, paddleWidth = 0.5;
struct Box {
	Shape * shape;
	Vec2 p;
	float w, h;
};
BOOL collisionPaddleBall(Vec2 *p, Vec2 *pBall,float width,float height) {
	float dx = pBall->x - MAX(p->x, MIN(pBall->x, p->x + width));
	float dy = pBall->y - MAX(p->y - height, MIN(pBall->y, p->y));
	return dx * dx + dy * dy < rBall * rBall;
}
int collisionBoxBall(vector<Box> boxs, Vec2 *pBall) {
	for(int i = 0 ; i < boxs.size();i++)
		if (collisionPaddleBall(&boxs[i].p, pBall, boxs[i].w, boxs[i].h))return i;
	return -1;
}
void main() {
	Display->CreateDisplay("Atari Breakout Game", 800, 600);
	Display->Location(1800, 0);
	Display->Icon("game.png");
	static vector<Box> boxs;
	static Vec2 p(-0.2, -0.9), vBall(0, -0.02), pBall(0, -0.2);
	var paddle = SHAPE->fill(1,1,0)->rectangle(p.x, p.y, paddleWidth, 0.05);
	var ball = SHAPE->fill(0.7)->circle(0, -0.2, rBall);
	for (float i = 0.8; i >= 0.1; i -= 0.16) {
		for (float j = -1.0; j <= 0.9; j += 0.165) {
			Box b;
			b.p = Vec2(j+0.015, i-0.01);
			b.w = 0.155;
			b.h = 0.15;
			b.shape = SHAPE
					->fill(RAND_RANGE(0.3,1), RAND_RANGE(0.3, 1), RAND_RANGE(0.3, 1))
					->rectangle(j+0.015, i-0.01, 0.155, 0.15);
			boxs.push_back(b);
		}
	}
	Display->KeyPressed([](int key) {
		if (key == GLFW_KEY_A) speedX = -0.02;
		else if (key == GLFW_KEY_D) speedX = 0.02;
		if (key == GLFW_KEY_LEFT)speedX = -0.02;
		else if (key == GLFW_KEY_RIGHT) speedX = 0.02;
	});
	Display->KeyReleased([](int key) {
		if (key == GLFW_KEY_A) speedX = 0;
		else if (key == GLFW_KEY_D) speedX = 0;
		if (key == GLFW_KEY_LEFT) speedX = 0;
		else if (key == GLFW_KEY_RIGHT)speedX = 0;
	});
	Display->Render([] {
		for (auto b : boxs) b.shape->render();
		paddle->render();
		ball->render();
	});
	Display->Update([](double timeStep) {
		p.x = p.x + speedX;
		if (p.x <= -1)p.x = -1;
		else if (p.x + paddleWidth >= 1)p.x = 1 - paddleWidth;
		paddle->position(p.x, p.y);
		pBall = pBall + vBall;
		ball->position(pBall.x, pBall.y);
		if (pBall.x - rBall < -1) {
			pBall.x = -1 + rBall;
			vBall.x = -vBall.x;
		}
		else if (pBall.x + rBall > 1) {
			pBall.x = 1 - rBall;
			vBall.x = -vBall.x;
		}
		else if (pBall.y + rBall > 1) {
			pBall.y = 1 - rBall;
			vBall.y = -vBall.y;
		}
		if (collisionPaddleBall(&p, &pBall,paddleWidth,0.05)) {
			Vec2 cPaddle(p.x + paddleWidth/2, p.y - 0.025);
			vBall.setAngle(atan2(pBall.y - cPaddle.y, pBall.x - cPaddle.x));
		}
		float i = collisionBoxBall(boxs, &pBall);
		if (i != -1) {
			//Vec2 cBox(boxs[i].p.x + boxs[i].w, boxs[i].p.y + boxs[i].h);
			//vBall.setAngle(atan2(pBall.y - cBox.y, pBall.x - cBox.x));
			//boxs.erase(boxs.begin() + i);
	
			Vec2 compass[] = {
				Vec2(0.0f, 1.0f),
				Vec2(1.0f, 0.0f),
				Vec2(0.0f, -1.0f),
				Vec2(-1.0f, 0.0f),
			};

			auto max = 0.0f;
			auto dir = -1;
			for (int i = 0; i < 4; i++)
			{
				float dot_product = compass[i].dot(boxs[i].p.getNorm());
				if (dot_product > max)
				{
					max = dot_product;
					dir = i;
				}
			}
			if (dir == 1 || dir == 2) {
				vBall.x = -vBall.x;
				pBall.x = pBall.x + (dir == 1?rBall:-rBall);
			}
			else {
				vBall.y = -vBall.y;
				pBall.y = pBall.y + (dir == 0 ? -rBall : rBall);
			}
			boxs.erase(boxs.begin() + i);
		}
	});
}