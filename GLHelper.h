#if defined(CONSOLE_MODE)
#pragma comment( linker, "/SUBSYSTEM:CONSOLE" )
#else 
#pragma comment( linker, "/SUBSYSTEM:WINDOWS" )
#pragma comment( linker, "/entry:mainCRTStartup" )
#endif
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3.lib")
#include <math.h>
#include<glad\glad.h>
#include<GLFW\glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include<stb-master\stb_image.h>
#include<glm\glm.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtc\type_ptr.hpp>
#include <time.h> 
#include <vector>
#include <fstream>
#include <string>
#define var static auto 
#define TIME glfwGetTime()
#define NORMAL_COORDINATE false
#define OPENGL_COORDINATE true
const float PI = 22 / 7.0;
#define DEG_TO_RAD(deg) deg * PI / 180
#define RAND_RANGE(Min,Max)((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min
#define RANDOM(Max) RAND_RANGE(0,Max)
#define MAX(val1,val2) (val1 >= val2 ? val1 : val2)
#define MIN(val1,val2) (val1 <= val2 ? val1 : val2)
#define SHAPE (new Shape)
using namespace std;
class Vec2 {
public:
	float x, y;
	Vec2() {
		x = y = 0;
	}
	Vec2(float x, float y) {
		this->x = x;
		this->y = y;
	}
	void set(Vec2 v) {
		this->x = v.x;
		this->y = v.y;
	}
	void set(float x, float y) {
		this->x = x;
		this->y = y;
	}
	void setAngle(float angle) {
		float length = getLength();
		x = cos(angle) * length;
		y = sin(angle) * length;
	}
	float getAngle() {
		return atan2(y, x);
	}
	void setLength(float length) {
		float angle = getAngle();
		x = cos(angle) * length;
		y = sin(angle) * length;
	}
	float getLength() {
		return sqrt(x*x + y*y);
	}
	Vec2 operator+(const Vec2 &v) {
		return Vec2(x + v.x, y + v.y);
	}
	Vec2 operator+=(const Vec2 &v) {
		return Vec2(x + v.x, y + v.y);
	}
	Vec2 operator-(const Vec2 &v) {
		return Vec2(x - v.x, y - v.y);
	}
	Vec2 operator-=(const Vec2 &v) {
		return Vec2(x - v.x, y - v.y);
	}
	Vec2 operator*(const Vec2 &v) {
		return Vec2(x * v.x, y * v.y);
	}
	Vec2 operator*(const double val) {
		return Vec2(x * val, y * val);
	}
	Vec2 operator*=(const Vec2 &v) {
		return Vec2(x * v.x, y * v.y);
	}
	Vec2 operator*=(const double val) {
		return Vec2(x * val, y * val);
	}
	Vec2 operator/(const Vec2 &v) {
		return Vec2(x / v.x, y / v.y);
	}
	Vec2 operator/(const double val) {
		return Vec2(x / val, y / val);
	}
	Vec2 operator/=(const Vec2 &v) {
		return Vec2(x / v.x, y / v.y);
	}
	Vec2 operator/=(const double val) {
		return Vec2(x / val, y / val);
	}
	float dot(Vec2 v) {
		return x * v.x + y * v.y;
	}
	float dist(Vec2 v) {
		float dx = v.x - x;
		float dy = v.y - y;
		return sqrt(dx*dx + dy*dy);
	}
	float unit() {
		return  1.0 / sqrt(x*x + y*y);
	}
	void negative() {
		x = -x;
		y = -y;
	}
	Vec2 getNeg() {
		Vec2 newVec;
		newVec.x = -x;
		newVec.y = -y;
		return newVec;
	}
	void perpendicular(Vec2 v) {
		x = -v.y;
		y = v.x;
	}
	void perpendicular() {
		float temp = x;
		x = -y;
		y = temp;
	}
	Vec2 getPerp(Vec2 v) {
		Vec2 newVec;
		newVec.x = -v.y;
		newVec.y = v.x;
		return newVec;
	}
	Vec2 getPerp() {
		Vec2 newVec;
		newVec.x = -y;
		newVec.y = x;
		return newVec;
	}
	void normalize() {
		float len = unit();
		x = x * len;
		y = y * len;
	}
	Vec2 getNorm() {
		float len = unit();
		x = x * len;
		y = y * len;
		return Vec2(x,y);
	}
};
struct Image
{
	const char * name = "";
	int width;
	int height;
	unsigned char * file;
	void Release() {
		STBI_FREE(file);
	}
};
class Util {
private:
	static Util* instance;
	Util() {
		stbi_set_flip_vertically_on_load(true);
	}
public:
	static Util* getInstance() {
		if (instance == 0) {
			instance = new Util();
		}
		return instance;
	}
	string LoadTextFile(const char * filePath) {
		ifstream file(filePath);
		string data;
		getline(file, data, '\0');
		string d = data.data();
		return d;
	}
	Image LoadImage(const char * imagePath) {
		Image image;
		image.name = imagePath;
		image.file = stbi_load(imagePath, &image.width, &image.height, 0, STBI_rgb);
		stbi_set_flip_vertically_on_load(true);
		return image;
	}
};
Util* Util::instance = 0;
#define Util Util::getInstance()
bool updateViewPort = true;
typedef void(*INPUT_CALLBACK)(int);
INPUT_CALLBACK cbKeyPress;
INPUT_CALLBACK cbKeyRelease;
INPUT_CALLBACK cbKeyIsPress;
bool useCBKeyPress = false;
bool useCBKeyRelease = false;
bool useCBKeyIsPress = false;
bool isPress = false;
bool mouseIsPress = false;
bool isDown = false;
typedef void(*MOUSE_CALLBACK1)(int, double, double);
MOUSE_CALLBACK1 cbMouseDown1;
MOUSE_CALLBACK1 cbMouseUp1;
MOUSE_CALLBACK1 cbMouseIsDown1;
bool useCBMouseDown1 = false;
bool useCBMouseUp1 = false;
bool useCBMouseIsDown1 = false;
typedef void(*MOUSE_CALLBACK2)(double, double);
MOUSE_CALLBACK2 cbMouseDown2;
MOUSE_CALLBACK2 cbMouseUp2;
MOUSE_CALLBACK2 cbMouseIsDown2;
bool useCBMouseDown2 = false;
bool useCBMouseUp2 = false;
bool useCBMouseIsDown2 = false;
typedef void(*MOUSE_CALLBACK3)();
MOUSE_CALLBACK3 cbMouseDown3;
MOUSE_CALLBACK3 cbMouseUp3;
MOUSE_CALLBACK3 cbMouseIsDown3;
bool useCBMouseDown3 = false;
bool useCBMouseUp3 = false;
bool useCBMouseIsDown3 = false;
int mouseIsDownButton;
int funcId;
typedef void(*MOUSE_POS_CALLBACK)(double, double);
MOUSE_POS_CALLBACK cbMouseMove;
MOUSE_POS_CALLBACK cbMouseScroll;
bool useCBMouseMove = false;
bool useCBMouseScroll = false;
typedef void(*MOUSE_ENTER_CALLBACK)();
MOUSE_ENTER_CALLBACK cbMouseEnter;
MOUSE_ENTER_CALLBACK cbMouseLeave;
bool useCBMouseEnter = false;
bool useCBMouseLeave = false;
int mouseCoordinateMode = OPENGL_COORDINATE;
class GLDisplay {
private:
	typedef void(*RENDER_CALLBACK)();
	RENDER_CALLBACK cbRender;
	typedef void(*UPDATE_CALLBACK)(double timeStep);
	UPDATE_CALLBACK cbUpdate;
	static GLDisplay* instance;
	GLDisplay() {};
	GLFWwindow* window;
	float width = 600, height = 600;
	float r = 0, g = 0, b = 0, a = 1;
	float fps = 60;
	double countTime = 0.0, delayTime = 0.0;
	bool useRender = false, useUpdate = false, useDelay = false, isDefualtClose = false;
	void GLInit() {
		srand(time(NULL));
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}
	void makeDisplay() {
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, [](auto windows, auto w, auto h) {
			if (updateViewPort) {
				glViewport(0, 0, w, h);
			}
		});
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (action == GLFW_PRESS) {
				if (useCBKeyPress) {
					cbKeyPress(key);
				}
				if (useCBKeyIsPress) {
					cbKeyIsPress(key);
				}
			}
			if (action == GLFW_REPEAT) {
				if (useCBKeyIsPress) {
					cbKeyIsPress(key);
				}
			}
			if (action == GLFW_RELEASE) {
				if (useCBKeyRelease) {
					cbKeyRelease(key);
				}
			}
		});
		glfwSetCursorEnterCallback(window, [](GLFWwindow* window, int entered) {
			if (entered) {
				if (useCBMouseEnter)
					cbMouseEnter();
			}
			else {
				if (useCBMouseLeave)
					cbMouseLeave();
			}
		});
		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			if (mouseCoordinateMode == OPENGL_COORDINATE) {
				xpos = xpos / 600 * 2 - 1;
				ypos = -ypos / 600 * 2 + 1;
			}
			if (action == GLFW_PRESS) {
				if (useCBMouseDown1) {
					if (!isDown) {
						cbMouseDown1(button, xpos, ypos);
						isDown = true;
					}
				}
				if (useCBMouseDown2) {
					if (!isDown) {
						cbMouseDown2(xpos, ypos);
						isDown = true;
					}
				}
				if (useCBMouseDown3) {
					if (!isDown) {
						cbMouseDown3();
						isDown = true;
					}
				}
				if (useCBMouseIsDown1) {
					funcId = 1;
					mouseIsPress = true;
					mouseIsDownButton = button;
					cbMouseIsDown1(button, xpos, ypos);
				}
				if (useCBMouseIsDown2) {
					funcId = 2;
					mouseIsPress = true;
					mouseIsDownButton = button;
					cbMouseIsDown2(xpos, ypos);
				}
				if (useCBMouseIsDown3) {
					funcId = 3;
					mouseIsPress = true;
					mouseIsDownButton = button;
					cbMouseIsDown3();
				}
			}
			if (action == GLFW_RELEASE) {
				isDown = mouseIsPress = false;
				if (useCBMouseUp1) {
					cbMouseUp1(button, xpos, ypos);
				}
				if (useCBMouseUp2) {
					cbMouseUp2(xpos, ypos);
				}
				if (useCBMouseUp3) {
					cbMouseUp3();
				}
			}
		});
		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
			if (useCBMouseMove) {
				if (mouseCoordinateMode == OPENGL_COORDINATE) {
					xpos = xpos / 600 * 2 - 1;
					ypos = -ypos / 600 * 2 + 1;
				}
				cbMouseMove(xpos, ypos);
			}
			if (mouseIsPress) {
				if (mouseCoordinateMode == OPENGL_COORDINATE) {
					xpos = xpos / 600 * 2 - 1;
					ypos = -ypos / 600 * 2 + 1;
				}
				if (funcId == 1)
					cbMouseIsDown1(mouseIsDownButton, xpos, ypos);
				else if (funcId == 2)
					cbMouseIsDown2(xpos, ypos);
				else if (funcId == 3)
					cbMouseIsDown3();
			}
		});
		glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
			if (useCBMouseScroll) {
				cbMouseScroll(xoffset, yoffset);
			}
		});
	}
public:
	static GLDisplay * getInstance() {
		if (instance == 0) {
			instance = new GLDisplay();
		}
		return instance;
	}
	void DefualtCloseOperation(bool status) {
		isDefualtClose = status;
	}
	void CreateDisplay(const char * title, const int width, const int height) {
		GLInit();
		this->width = width;
		this->height = height;
		this->window = glfwCreateWindow(width, height, title, NULL, NULL);
		makeDisplay();
	}
	void CreateDisplay(const int width, const int height) {
		GLInit();
		this->width = width;
		this->height = height;
		this->window = glfwCreateWindow(width, height, "OpenGL Scene", NULL, NULL);
		makeDisplay();

	}
	void CreateDisplay(const char * title, bool isFullScreen) {
		GLInit();
		auto monitor = glfwGetPrimaryMonitor();
		auto mode = glfwGetVideoMode(monitor);
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		width = mode->width;
		height = mode->height;
		this->window = glfwCreateWindow(width, height, title, monitor, NULL);
		makeDisplay();
	}
	void CreateDisplay(const char * title) {
		GLInit();
		this->window = glfwCreateWindow(width, height, title, NULL, NULL);
		glfwMakeContextCurrent(window);
		makeDisplay();
	}
	void Icon(const char * iconPath) {
		GLFWimage icons[1];
		icons[0].pixels = stbi_load(iconPath, &icons[0].width, &icons[0].height, 0, STBI_rgb_alpha);
		glfwSetWindowIcon(window, 1, icons);
	}
	void Location(int x, int y) {
		glfwSetWindowPos(this->window, x, y);
	}
	void BackgroundColor(float r, float g, float b, float a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	void BackgroundColor(float r, float g, float b) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = 1.0;
	}
	void MouseCoordinate(bool coordinateMode) {
		mouseCoordinateMode = coordinateMode;
	}
	void BackgroundColor(float gray) {
		this->r = gray;
		this->g = gray;
		this->b = gray;
		this->a = 1.0;
	}
	void AutoUpdateViewPort(bool status) {
		updateViewPort = status;
	}
	void Render(RENDER_CALLBACK callback) {
		useRender = true;
		cbRender = callback;
	}
	void FramRate(float fps) {
		this->fps = fps;
	}
	void Delay(double time) {
		this->countTime = 0;
		this->delayTime = time / 1000;
		this->useDelay = true;
	}
	void Update(UPDATE_CALLBACK callbackUpdate) {
		double previous = TIME;
		double lag = 0.0;
		while (!glfwWindowShouldClose(window))
		{
			double current = TIME;
			double elapsed = current - previous;
			previous = current;
			lag += elapsed;
			if (useDelay) {
				countTime += elapsed;
			}
			if (isDefualtClose) {
				if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
					glfwSetWindowShouldClose(window, true);
			}
			while (lag >= 1 / this->fps) {
				if (useDelay) {
					if (countTime >= delayTime) {
						useDelay = false;
						callbackUpdate(elapsed);
					}
				}
				else {
					callbackUpdate(elapsed);
				}
				lag -= 1 / this->fps;
			}
			glClearColor(r, g, b, a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			if (useRender) {
				cbRender();
			}
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		glfwTerminate();
	}
	void SetCursor(int cursorId) {
		GLFWcursor* cursor = glfwCreateStandardCursor(cursorId);
		glfwSetCursor(window, cursor);
	}
	void DisplayClosed() {
		glfwSetWindowShouldClose(window, true);
	}
	void KeyPressed(INPUT_CALLBACK callback) {
		useCBKeyPress = true;
		cbKeyPress = callback;
	}
	void KeyReleased(INPUT_CALLBACK callback) {
		useCBKeyRelease = true;
		cbKeyRelease = callback;
	}
	void KeyIsPressed(INPUT_CALLBACK callback) {
		useCBKeyIsPress = true;
		cbKeyIsPress = callback;
	}
	void MouseDown(MOUSE_CALLBACK1 callback) {
		useCBMouseDown1 = true;
		cbMouseDown1 = callback;
	}
	void MouseUp(MOUSE_CALLBACK1 callback) {
		useCBMouseUp1 = true;
		cbMouseUp1 = callback;
	}
	void MouseIsDown(MOUSE_CALLBACK1 callback) {
		useCBMouseIsDown1 = true;
		cbMouseIsDown1 = callback;
	}
	void MouseDown(MOUSE_CALLBACK2 callback) {
		useCBMouseDown2 = true;
		cbMouseDown2 = callback;
	}
	void MouseUp(MOUSE_CALLBACK2 callback) {
		useCBMouseUp2 = true;
		cbMouseUp2 = callback;
	}
	void MouseIsDown(MOUSE_CALLBACK2 callback) {
		useCBMouseIsDown2 = true;
		cbMouseIsDown2 = callback;
	}
	void MouseDown(MOUSE_CALLBACK3 callback) {
		useCBMouseDown3 = true;
		cbMouseDown3 = callback;
	}
	void MouseUp(MOUSE_CALLBACK3 callback) {
		useCBMouseUp3 = true;
		cbMouseUp3 = callback;
	}
	void MouseIsDown(MOUSE_CALLBACK3 callback) {
		useCBMouseIsDown3 = true;
		cbMouseIsDown3 = callback;
	}
	void MouseMove(MOUSE_POS_CALLBACK callback) {
		useCBMouseMove = true;
		cbMouseMove = callback;
	}
	void MouseScroll(MOUSE_POS_CALLBACK callback) {
		useCBMouseScroll = true;
		cbMouseScroll = callback;
	}
	void MouseEnter(MOUSE_ENTER_CALLBACK callback) {
		useCBMouseEnter = true;
		cbMouseEnter = callback;
	}
	void MouseLeave(MOUSE_ENTER_CALLBACK callback) {
		useCBMouseLeave = true;
		cbMouseLeave = callback;
	}
};
GLDisplay* GLDisplay::instance = 0;
#define Display GLDisplay::getInstance()
#define DELAY(time) Display->Delay(time)
class Shape {
private:
	int program, mode = GL_LINE_LOOP;
	unsigned int texture;
	unsigned int VBO, VAO;
	vector<float> tempVertext, tempColor, tempTexture;
	Vec2 uv;
	float *rgb, r;
	float *vArr, *cArr, *tArr , vArrSize, cArrSize, tArrSize;
	bool useTexture = false,colorOneToAll = false, isStroke = true, isSetColor = false, isRect = false, isPolygon = false,isLinearMode = false;
	Image image;
	float w = 0;
	float h = 0;
	float sx1 = 0;
	float sy1 = 1;
	float sx2 = 1;
	float sy2 = 0;
	float dx = 1;
	float dy = 1;
	const char *vt_data =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"layout (location = 2) in vec2 aTexCoord;\n"
		"out vec3 ourColor;\n"
		"out vec2 TexCoord;\n"
		"uniform mat4 translation;\n"
		"uniform mat4 rotation;\n"
		"uniform mat4 scale;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = translation * rotation * scale * vec4(aPos, 1.0);\n"
		"   ourColor = aColor;\n"
		"   TexCoord  = aTexCoord;\n"
		"}\0"
		;
	const char *fg_data =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform sampler2D ourTexture;\n"
		"in vec3 ourColor;\n"
		"in vec2 TexCoord;\n"
		"uniform vec4 time;\n"
		"uniform float alpha;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(ourColor, alpha);\n"
		"}\n\0"
		;
	const char *fg_data_texture =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform sampler2D ourTexture;\n"
		"in vec3 ourColor;\n"
		"in vec2 TexCoord;\n"
		"uniform vec4 time;\n"
		"uniform float alpha;\n"
		"uniform int mix;\n"
		"void main()\n"
		"{\n"
		"   if(mix == 1)\n"
		"   FragColor = vec4(ourColor, alpha);\n"
		"   else if(mix == 2)\n"
		"   FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, alpha);\n"
		"   else if(mix == 3)\n"
		"   FragColor = texture(ourTexture, TexCoord) * vec4(1.0,1.0,1.0, alpha);\n"
		"}\n\0"
		;
	void initialShader(int vertex, int fragment) {
		program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		texture = 0;
		glGenVertexArrays(1, &VAO);
		glUseProgram(program);
		this->rotation(0, 0, 0, 1.0);
		this->scale(1.0, 1.0, 1.0);
		this->translation(0, 0, 0);
		this->position(0, 0);
		this->setColor(255);
		this->setAlpha(1);
		glUseProgram(0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	void setMixMode(short mode) {
		if (useTexture) {
			glUseProgram(program);
			glUniform1i(getUiniform("mix"), mode);
			glUseProgram(0);
		}
	}
	void initShaderWithImgSrc(Image image, float srcX1, float srcY1, float srcX2, float srcY2) {
		const char * vtString = vt_data;
		int vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vtString, NULL);
		glCompileShader(vertex);
		const char * fgString = fg_data_texture;
		int fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fgString, NULL);
		glCompileShader(fragment);
		initialShader(vertex, fragment);
		this->image = image;
		useTexture = true;
		isStroke = false;
		setMixMode(3);
		w = this->image.width;
		h = this->image.height;
		if (uv.y >= 0) {
			sy1 = srcY1 / h;
			sy2 = srcY2 / h;
		}
		else {
			sy1 = srcY2 / h;
			sy2 = srcY1 / h;
		}
		if (uv.x >= 0) {
			sx1 = srcX1 / w;
			sx2 = srcX2 / w;
		}
		else {
			sx1 = srcX2 / w;
			sx2 = srcX1 / w;
		}
		dx = sx2 - sx1;
		dy = sy2 - sy1;
	}
	void initImageFullSize(Image image) {
		const char * vtString = vt_data;
		int vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vtString, NULL);
		glCompileShader(vertex);
		const char * fgString = fg_data_texture;
		int fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fgString, NULL);
		glCompileShader(fragment);
		initialShader(vertex, fragment);
		this->image = image;
		useTexture = true;
		isStroke = false;
		setMixMode(3);
		if (uv.y >= 0) {
			sy1 = 0;
			sy2 = 1;
		}
		else {
			sy1 = 1;
			sy2 = 0;
		}
		if (uv.x >= 0) {
			sx1 = 0;
			sx2 = 1;
		}
		else {
			sx1 = 1;
			sx2 = 0;
		}
	}
public:
	Shape(string vt_data, string fg_data) {
		const char * vtString = vt_data.c_str();
		int vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vtString, NULL);
		glCompileShader(vertex);
		const char * fgString = fg_data.c_str();
		int fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fgString, NULL);
		glCompileShader(fragment);
		initialShader(vertex, fragment);
	}
	Shape(Image image, float srcX1, float srcY1, float srcX2, float srcY2) {
		initShaderWithImgSrc(image, srcX1, srcY1, srcX2, srcY2);
	}
	Shape(const char * imagePath, float srcX1, float srcY1, float srcX2, float srcY2) {
		initShaderWithImgSrc(Util->LoadImage(imagePath), srcX1, srcY1, srcX2, srcY2);
	}
	Shape(Image image, float srcX1, float srcY1, float srcX2, float srcY2, bool LinearFillterMode) {
		isLinearMode = LinearFillterMode;
		initShaderWithImgSrc(image, srcX1, srcY1, srcX2, srcY2);
	}
	Shape(const char * imagePath, float srcX1, float srcY1, float srcX2, float srcY2, bool LinearFillterMode) {
		isLinearMode = LinearFillterMode;
		initShaderWithImgSrc(Util->LoadImage(imagePath), srcX1, srcY1, srcX2, srcY2);
	}
	Shape(Image image) {
		initImageFullSize(image);
	}
	Shape(Image image,bool LinearFillterMode) {
		isLinearMode = LinearFillterMode;
		initImageFullSize(image);
	}
	Shape(const char * imagePath) {
		image = Util->LoadImage(imagePath);
		initImageFullSize(image);
	}
	Shape(const char * imagePath, bool LinearFillterMode) {
		isLinearMode = LinearFillterMode;
		image = Util->LoadImage(imagePath);
		initImageFullSize(image);
	}
	Shape() {
		const char * vtString = vt_data;
		int vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vtString, NULL);
		glCompileShader(vertex);
		const char * fgString = fg_data;
		int fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fgString, NULL);
		glCompileShader(fragment);
		initialShader(vertex, fragment);
	}
	Shape* setAlpha(float alpha) {
		glUseProgram(program);
		glUniform1f(getUiniform("alpha"), alpha);
		glUseProgram(0);
		return this;
	}
	void beginShape() {
		this->vArr = NULL;
		if (!isSetColor)
			setColor(255);
		if (isStroke)
			this->mode = GL_LINE_LOOP;
		else
			this->mode = GL_LINE_BIT;
	}
	void beginShape(int mode) {
		this->vArr = NULL;
		if (!isSetColor)
			setColor(255);
		this->mode = mode;
	}
	void endShape() {
		this->vArrSize = this->tempVertext.size();
		if (this->tempColor.size() > 0) {
			this->cArrSize = this->tempColor.size();
			this->cArr = new float[this->cArrSize];
			for (int i = 0; i < this->cArrSize; i++) {
				this->cArr[i] = this->tempColor[i];
			}
		}
		else if (this->colorOneToAll) {
			this->cArrSize = this->vArrSize;
			this->cArr = new float[this->cArrSize];
		}
		this->vArr = new float[this->vArrSize];
		for (int i = 0; i < this->vArrSize; i++) {
			this->vArr[i] = this->tempVertext[i];
			if (this->colorOneToAll) {
				this->cArr[i] = this->rgb[i % 3];
			}
		}
		if (useTexture) {
			if (isRect) {
				float textureRectCoord[] = {
					sx1,sy2, // topLeft  0 , 1 
					sx2,sy2, // topRight 1 , 1
					sx1,sy1, // bottomLeft 0 , 0
					sx1,sy1, // buttomLeft 0 , 0  
					sx2,sy2, // topRight 1 , 1 
					sx2,sy1  // bottomRight 1 , 0
				};
				for (int i = 0; i < 12; i++) {
					this->tempTexture.push_back(textureRectCoord[i]);
				}
			}
			this->tArrSize = this->tempTexture.size();
			this->tArr = new float[this->tArrSize];
			for (int i = 0; i < this->tArrSize; i++) {
				this->tArr[i] = this->tempTexture[i];
			}
			this->initTexture(this->image);
		}
		this->initialBuffer();
	}
	void vertext(float x, float y, float z) {
		this->tempVertext.push_back(x);
		this->tempVertext.push_back(y);
		this->tempVertext.push_back(z);
		if (useTexture && !isRect) {
			this->tempTexture.push_back((x + (isPolygon ? this->r / 2 : 0.5))*dx / (isPolygon ? this->r : 1) + sx1);
			this->tempTexture.push_back((y + (isPolygon ? this->r / 2 : 0.5))*dy / (isPolygon ? this->r : 1) + sy1);
		}
	}
	void vertext(float x, float y) {
		this->tempVertext.push_back(x);
		this->tempVertext.push_back(y);
		this->tempVertext.push_back(0.0f);
		if (useTexture && !isRect) {
			this->tempTexture.push_back((x + (isPolygon ? this->r / 2 : 0.5))*dx / (isPolygon ? this->r : 1) + sx1);
			this->tempTexture.push_back((y + (isPolygon ? this->r / 2 : 0.5))*dy / (isPolygon ? this->r : 1) + sy1);
		}
	}
	void color(float r, float g, float b) {
		this->tempColor.push_back(r);
		this->tempColor.push_back(g);
		this->tempColor.push_back(b);
		if (this->colorOneToAll) {
			delete(this->rgb);
			this->colorOneToAll = false;
		}
	}
	template <typename T, int N>
	Shape * fill(T(&color)[N]) {
		setColor(color);
		this->isStroke = false;
		this->mode = GL_LINE_BIT;
		setMixMode(2);
		return this;
	}
	Shape * fill(float r, float g, float b, float alpha) {
		setColor(r, g, b, alpha);
		this->isStroke = false;
		this->mode = GL_LINE_BIT;
		setMixMode(2);
		return this;
	}
	Shape * fill(float r, float g, float b) {
		setColor(r, g, b);
		this->isStroke = false;
		this->mode = GL_LINE_BIT;
		setMixMode(2);
		return this;
	}
	Shape * fill(float gray) {
		setColor(gray, gray, gray);
		this->isStroke = false;
		this->mode = GL_LINE_BIT;
		setMixMode(2);
		return this;
	}
	template <typename T, int N>
	Shape * stroke(T(&color)[N]) {
		setColor(color);
		this->isStroke = !useTexture;
		this->mode = GL_LINE_LOOP;
		setMixMode(3);
		return this;
	}
	Shape * stroke(float r, float g, float b, float alpha) {
		setColor(r, g, b, alpha);
		this->isStroke = !useTexture;
		this->mode = GL_LINE_LOOP;
		setMixMode(3);
		return this;
	}
	Shape * stroke(float r, float g, float b) {
		setColor(r, g, b);
		this->isStroke = !useTexture;
		this->mode = GL_LINE_LOOP;
		setMixMode(3);
		return this;
	}
	Shape * stroke(float gray) {
		setColor(gray, gray, gray);
		this->isStroke = !useTexture;
		this->mode = GL_LINE_LOOP;
		setMixMode(3);
		return this;
	}
	Shape * strokeWeight(float width) {
		glLineWidth(width);
		return this;
	}
	Shape * enableLineSmooth() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		return this;
	}
private:
	template <typename T, int N>
	void setColor(T(&color)[N]) {
		this->cArr = color;
		this->cArrSize = N;
		this->isSetColor = true;
	}
	void setColor(float r, float g, float b, float alpha) {
		this->rgb = new float[3]{ r,g,b };
		this->colorOneToAll = true;
		this->setAlpha(alpha);
		this->isSetColor = true;
	}
	void setColor(float r, float g, float b) {
		this->rgb = new float[3]{ r,g,b };
		this->colorOneToAll = true;
		this->isSetColor = true;
	}
	void setColor(float gray) {
		this->rgb = new float[3]{ gray,gray,gray };
		this->colorOneToAll = true;
		this->isSetColor = true;
	}
public:
	template<typename T, size_t  N>
	void addAllVertext(T(&vertex)[N]) {
		this->vArr = vertex;
		this->vArrSize = N;
	}
	template<typename T, int N>
	void addAllColor(T(&color)[N]) {
		this->cArr = color;
		this->cArrSize = N;
	}
	template<typename T, int N>
	void addAllTexture(T(&textureCoord)[N], const char * imagePath) {
		this->tArr = textureCoord;
		this->tArrSize = N;
		Image image = Util::getInstance()->LoadImage(imagePath);
		initTexture(image);
		this->useTexture = true;
	}
private:
	void initTexture(Image image) {
		glGenTextures(1, &this->texture);
		glBindTexture(GL_TEXTURE_2D, this->texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, isLinearMode? GL_LINEAR: GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.file);
		glGenerateMipmap(GL_TEXTURE_2D);
		image.Release();
	}
public:
	unsigned int createTexture(Image image) {
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, isLinearMode ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.file);
		glGenerateMipmap(GL_TEXTURE_2D);
		image.Release();
		return texture;
	}
	template<typename T, int N>
	void bineArray(T(&Arr)[N], int index, int size, int pointer, int stride) {
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Arr), Arr, GL_STATIC_DRAW);
		glVertexAttribPointer(index, stride - pointer, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)(pointer * sizeof(float)));
		glEnableVertexAttribArray(index);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		this->vArrSize = N;
	}
	template<typename T, int N>
	void bineArray(T(&Arr)[N], int index, int size, int pointer, int stride, const char * imagePath) {
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Arr), Arr, GL_STATIC_DRAW);
		glVertexAttribPointer(index, stride - pointer, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)(pointer * sizeof(float)));
		glEnableVertexAttribArray(index);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
		Image image = Util::getInstance()->LoadImage(imagePath);
		this->texture = this->createTexture(image);
	}
	void initialBuffer() {
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(this->vArr)* this->vArrSize, this->vArr, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(this->cArr)*this->cArrSize, this->cArr, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		if (useTexture) {
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(this->tArr)*this->tArrSize, this->tArr, GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		glBindVertexArray(0);
	}
	void render(int mode) {
		glUseProgram(program);
		if (this->texture) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->texture);
		}
		glBindVertexArray(this->VAO);
		glDrawArrays(mode, 0, vArrSize / 3);
	}
	void render() {
		glUseProgram(program);
		if (this->texture) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->texture);
		}
		glBindVertexArray(this->VAO);
		glDrawArrays(this->mode, 0, vArrSize / 3);
	}
	void bineTexture() {
		glBindTexture(GL_TEXTURE_2D, this->texture);
	}
	void useProgram() {
		glUseProgram(program);
		glBindVertexArray(VAO);
		if (this->texture) {
			this->bineTexture();
		}
	}
	int getProgram() {
		return this->program;
	}
	int getUiniform(const char* uniformName) {
		return glGetUniformLocation(this->program, uniformName);
	}
	Shape * position(float x, float y, float z) {
		glUseProgram(program);
		glm::mat4 position;
		position = glm::translate(position, glm::vec3(x - uv.x, y - uv.y, 0.0));
		glUniformMatrix4fv(getUiniform("translation"), 1, GL_FALSE, glm::value_ptr(position));
		glUseProgram(0);
		return this;
	}
	Shape * position(float x, float y) {
		glUseProgram(program);
		glm::mat4 position;
		position = glm::translate(position, glm::vec3(x - uv.x, y - uv.y, 0.0));
		glUniformMatrix4fv(getUiniform("translation"), 1, GL_FALSE, glm::value_ptr(position));
		glUseProgram(0);
		return this;
	}
	Shape * translation(float x, float y, float z) {
		glUseProgram(program);
		glm::mat4 position;
		position = glm::translate(position, glm::vec3(x, y, z));
		glUniformMatrix4fv(getUiniform("translation"), 1, GL_FALSE, glm::value_ptr(position));
		glUseProgram(0);
		return this;
	}
	Shape * translation(float x, float y) {
		glUseProgram(program);
		glm::mat4 position;
		position = glm::translate(position, glm::vec3(x, y, 0.0));
		glUniformMatrix4fv(getUiniform("translation"), 1, GL_FALSE, glm::value_ptr(position));
		glUseProgram(0);
		return this;
	}
	Shape * rotation(float angle, float x, float y, float z) {
		glUseProgram(program);
		glm::mat4 rotation;
		rotation = glm::rotate(rotation, angle, glm::vec3(x, y, z));
		glUniformMatrix4fv(getUiniform("rotation"), 1, GL_FALSE, glm::value_ptr(rotation));
		glUseProgram(0);
		return this;
	}
	Shape * scale(float x, float y, float z) {
		glUseProgram(program);
		glm::mat4 scale;
		scale = glm::scale(scale, glm::vec3(x, y, z));
		glUniformMatrix4fv(getUiniform("scale"), 1, GL_FALSE, glm::value_ptr(scale));
		glUseProgram(0);
		return this;
	}
	Shape * scale(float x, float y) {
		glUseProgram(program);
		glm::mat4 scale;
		scale = glm::scale(scale, glm::vec3(x, y, 1.0));
		glUniformMatrix4fv(getUiniform("scale"), 1, GL_FALSE, glm::value_ptr(scale));
		glUseProgram(0);
		return this;
	}
	void setMode(int mode) {
		this->mode = mode;
	}
	Shape * circle(float x, float y, float radius) {
		return polygon(x, y, radius, 360);
	}
	Shape * polygon(float x, float y, float radius, float strip) {
		uv.x = 0;
		uv.y = 0;
		this->r = radius;
		isPolygon = true;
		float oldX = 0, oldY = 0;
		beginShape();
		for (float i = 0; i <= 360; i += 360 / strip) {
			float px = cos(DEG_TO_RAD(i))* radius + 0;
			float py = sin(DEG_TO_RAD(i))* radius + 0;
			if (!isStroke) {
				vertext(0, 0);
			}
			vertext(px, py);
			if (!isStroke) {
				vertext(oldX, oldY);
			}
			oldX = px;
			oldY = py;
		}
		endShape();
		position(x, y);
		return this;
	}
	Shape * rectangle(float x, float y, float x2, float y2) {
		float x1 = -x2 / 2, y1 = y2 / 2;
		uv.x = x1;
		uv.y = y1;
		isRect = true;
		beginShape();
		if (isStroke) {
			vertext(x1, y1);
			vertext(x1+x2, y1);
			vertext(x1+x2,y1-y2);
			vertext(x1, y1-y2);
		}
		else {
			vertext(x1, y1);
			vertext(x1+x2, y1);
			vertext(x1, y1-y2);
			vertext(x1, y1-y2);
			vertext(x1+x2, y1);
			vertext(x1+x2, y1-y2);
		}
		endShape();
		position(x,y);
		return this;
	}
	Shape *line(float x1, float y1, float x2, float y2) {
		uv.x = x1;
		uv.y = y1;
		beginShape(GL_LINES);
		vertext(x1, y1);
		vertext(x2, y2);
		endShape();
		return this;
	}
};