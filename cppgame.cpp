#include "cgame.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <atlimage.h>
#include <mmsystem.h>
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
#pragma comment(lib,"winmm.lib")
BYTE cgame::R = 0;
BYTE cgame::G = 0;
BYTE cgame::B = 0;
cgame::cgame_complex_matrix::cgame_complex_matrix()
{
	width = 0;
	height = 0;
}
bool cgame::cgame_complex_matrix::create(USI _width, USI _height)
{
	if ((_width <= 1892) && (_height <= 985))
	{
		buffer = new RGB * [_height];
		for (USI i = 0; i < _height; i++)
			buffer[i] = new RGB[_width];
		width = _width;
		height = _height;
		return true;
	}
	return false;
}
bool cgame::cgame_complex_matrix::recreate(USI _width, USI _height)
{
	destroy();
	return create(_width, _height);
}
void cgame::cgame_complex_matrix::destroy()
{
	for (USI i = 0; i < height; i++)
		delete[] buffer[i];
	delete[] buffer;
}
void cgame::cgame_complex_matrix::init(BYTE R, BYTE G, BYTE B)
{
	for (USI i = 0; i < width; i++)
		for (USI j = 0; j < height; j++)
		{
			buffer[i][j].r = R;
			buffer[i][j].g = G;
			buffer[i][j].b = B;
		}
}
bool cgame::cgame_complex_matrix::set_pixel(USI x, USI y, BYTE R, BYTE G, BYTE B)
{
	if ((x < width) && (y < height))
	{
		RGB rgb = { R,G,B };
		buffer[x][y] = rgb;
		return true;
	}
	return false;
}
bool cgame::cgame_complex_matrix::attach_square(USI len, USI x, USI y, BYTE R, BYTE G, BYTE B)
{
	if ((len <= 985) && ((len + x) <= width) && ((len + y) <= height))
	{
		RGB rgb = { R,G,B };
		//上
		for (USI i = 0; i < len; i++)
			buffer[x + i][y] = rgb;
		//右
		for (USI i = 0; i < len; i++)
			buffer[x + (len - 1)][y + i] = rgb;
		//下
		for (USI i = 0; i < len; i++)
			buffer[x + i][y + (len - 1)] = rgb;
		//左
		for (USI i = 0; i < len; i++)
			buffer[x][y + i] = rgb;
		return true;
	}
	return false;
}
bool cgame::cgame_complex_matrix::attach_rect(USI rect_width, USI rect_height, USI x, USI y, BYTE R, BYTE G, BYTE B)
{
	if ((rect_width <= 1892) && (rect_height <= 985) && ((rect_width + x) <= width) && ((rect_height + y) <= height))
	{
		RGB rgb = { R,G,B };
		//上
		for (USI i = 0; i < rect_width; i++)
			buffer[x + i][y] = rgb;
		//右
		for (USI i = 0; i < rect_height; i++)
			buffer[x + (rect_width - 1)][y + i] = rgb;
		//下
		for (USI i = 0; i < rect_width; i++)
			buffer[x + i][y + (rect_height - 1)] = rgb;
		//左
		for (USI i = 0; i < rect_height; i++)
			buffer[x][y + i] = rgb;
		return true;
	}
	return false;
}
cgame::cgame_character::cgame_character()
{
	x = 0.0f;
	y = 0.0f;
	max_app_num = 1;
	cur_app_num = 0;
	cur_app = 0;
	img = new CImage[1];
}
cgame::cgame_character::cgame_character(BYTE _max_app_num)
{
	x = 0.0f;
	y = 0.0f;
	max_app_num = _max_app_num;
	cur_app_num = 0;
	cur_app = 0;
	img = new CImage[_max_app_num];
}
cgame::cgame_character::cgame_character(const cgame_character& char1)
{
	x = char1.x;
	y = char1.y;
	max_app_num = char1.max_app_num;
	cur_app_num = char1.cur_app_num;
	cur_app = char1.cur_app;
	img = new CImage[max_app_num];
	for (BYTE i = 0; i < cur_app_num; i++)
		img[i] = char1.img[i];
}
cgame::cgame_character::cgame_character(cgame_character&& char1)
{
	x = char1.x;
	y = char1.y;
	max_app_num = char1.max_app_num;
	cur_app_num = char1.cur_app_num;
	cur_app = char1.cur_app;
	img = char1.img;
	char1.img = nullptr;
}
const cgame::cgame_character& cgame::cgame_character::operator=(const cgame::cgame_character& char1)
{
	delete[] img;
	x = char1.x;
	y = char1.y;
	max_app_num = char1.max_app_num;
	cur_app_num = char1.cur_app_num;
	cur_app = char1.cur_app;
	img = new CImage[max_app_num];
	for (BYTE i = 0; i < cur_app_num; i++)
		img[i] = char1.img[i];
	return char1;
}
bool cgame::cgame_character::load_from_complex_matrix(const cgame::cgame_complex_matrix& shape)
{
	if (cur_app_num < max_app_num)
	{
		img[cur_app_num].Create(shape.width, shape.height, 32);
		for (USI i = 0; i < shape.width; i++)
			for (USI j = 0; j < shape.height; j++)
				img[cur_app_num].SetPixelRGB(i, j, (shape.buffer)[i][j].r, (shape.buffer)[i][j].g, (shape.buffer)[i][j].b);
		cur_app_num++;
		return true;
	}
	return false;
}
bool cgame::cgame_character::load(const char* filename)
{
	if (cur_app_num < max_app_num)
	{
		CString st = filename;
		img[cur_app_num].Load((LPCTSTR)st);
		if (!(img[cur_app_num].IsNull()))
		{
			cur_app_num++;
			return true;
		}
	}
	return false;
}
bool cgame::cgame_character::show() const
{
	HDC hdc = GetDC(GetConsoleWindow());
	if (hdc)
	{
		USI x1 = x, y1 = y;
		img[cur_app].Draw(hdc, x1, y1);
		return true;
	}
	return false;
}
bool cgame::cgame_character::show_transparent(BYTE opaque_val) const
{
	HDC hdc = GetDC(GetConsoleWindow());
	if (hdc)
	{
		USI x1 = x, y1 = y;
		img[cur_app].AlphaBlend(hdc, x1, y1, opaque_val);
		return true;
	}
	return false;
}
bool cgame::cgame_character::hide() const
{
	HDC hdc = GetDC(GetConsoleWindow());
	if (hdc)
	{
		//将角色所在位置全部设置为背景色
		USI x1 = x, y1 = y;
		USI xlimit = x1 + img[cur_app].GetWidth();
		USI ylimit = y1 + img[cur_app].GetHeight();
		CImage newimg;
		newimg.Create(25, 25, 32);
		for (BYTE i = 0; i < 25; i++)
			for (BYTE j = 0; j < 25; j++)
				newimg.SetPixelRGB(i, j, cgame::R, cgame::G, cgame::B);
		for (USI i = x1; i <= xlimit; i += 25)
			for (USI j = y1; j <= ylimit; j += 25)
				newimg.Draw(hdc, i, j);
		return true;
	}
	return false;
}
bool cgame::cgame_character::set_x(USI x1)
{
	if ((x1 >= 0) && (x1 <= 1892))
	{
		x = x1;
		return true;
	}
	return false;
}
bool cgame::cgame_character::set_y(USI y1)
{
	if ((y1 >= 0) && (y1 <= 985))
	{
		y = y1;
		return true;
	}
	return false;
}
bool cgame::cgame_character::set_posi(USI x1, USI y1)
{
	if ((x1 >= 0) && (x1 <= 1892) && (y1 >= 0) && (y1 <= 985))
	{
		x = x1;
		y = y1;
		return true;
	}
	return false;
}
bool cgame::cgame_character::set_app(BYTE app)
{
	if (app < cur_app_num)
	{
		cur_app = app;
		return true;
	}
	return false;
}
void cgame::cgame_character::next_app() { cur_app++; cur_app %= cur_app_num; }
bool cgame::cgame_character::rand_x(USI start_x, USI end_x)
{
	if ((start_x >= 0) && (start_x <= 1892) && (end_x >= 0) && (end_x <= 1892) && (start_x < end_x))
	{
		x = randint(start_x, end_x);
		return true;
	}
	return false;
}
bool cgame::cgame_character::rand_y(USI start_y, USI end_y)
{
	if ((start_y >= 0) && (start_y <= 985) && (end_y >= 0) && (end_y <= 985) && (start_y < end_y))
	{
		y = randint(start_y, end_y);
		return true;
	}
	return false;
}
bool cgame::cgame_character::rand_posi(USI start_x, USI start_y, USI end_x, USI end_y) { return (rand_x() && rand_y()); }
void cgame::cgame_character::rand_app() { cur_app = randint(0, cur_app_num - 1); }
USI cgame::cgame_character::get_x() const { return x; }
USI cgame::cgame_character::get_y() const { return y; }
BYTE cgame::cgame_character::get_cur_app() const { return cur_app; }
bool cgame::cgame_character::move_right(short steps)
{
	if (((x + steps) >= 0) && ((x + steps) <= 1892))
	{
		x += steps;
		return true;
	}
	return false;
}
bool cgame::cgame_character::move_up(short steps)
{
	if ((y >= steps) && ((y - steps) <= 985))
	{
		y -= steps;
		return true;
	}
	return false;
}
void cgame::cgame_character::move_to_cursor()
{
	POINT pt;
	GetCursorPos(&pt);
	//将POINT坐标转为CImage坐标
	x = float(pt.x) * 3 / 2;
	y = float(pt.y) * 3 / 2 - 30;
}
void cgame::cgame_character::move_to_character(const cgame_character& char1) { x = char1.x; y = char1.y; }
bool cgame::cgame_character::bump(const cgame_character& char1) const
{
	float charachalfwidth = float(img[cur_app].GetWidth()) / 2;
	float charachalfheight = float(img[cur_app].GetHeight()) / 2;
	float charac2halfwidth = float(((char1.img)[char1.cur_app]).GetWidth()) / 2;
	float charac2halfheight = float(((char1.img)[char1.cur_app]).GetHeight()) / 2;
	float charac1centrex = x + charachalfwidth;
	float charac1centrey = y + charachalfheight;
	float charac2centrex = char1.x + charac2halfwidth;
	float charac2centrey = char1.y + charac2halfheight;
	if (abs(charac1centrex - charac2centrex) <= (charachalfwidth + charac2halfwidth))
	{
		if (abs(charac1centrey - charac2centrey) <= (charachalfheight + charac2halfheight))
		{
			return true;
		}
	}
	return false;
}
bool cgame::cgame_character::is_side_of_screen() const { return ((x == 0) || ((x + img[cur_app].GetWidth()) == 1892) || (y == 0) || ((y + img[cur_app].GetHeight()) == 985)); }
bool cgame::cgame_character::is_up_side_screen() const { return (y == 0); }
bool cgame::cgame_character::is_down_side_screen() const { return ((y + img[cur_app].GetHeight()) == 985); }
bool cgame::cgame_character::is_left_side_screen() const { return (x == 0); }
bool cgame::cgame_character::is_right_side_screen() const { return ((x + img[cur_app].GetWidth()) == 1892); }
bool cgame::cgame_character::touch_mouse() const
{
	//侦测是否碰到鼠标
	POINT pt;
	GetCursorPos(&pt);
	float tmpx = float(x) * 2 / 3;
	float tmpy = float(y + 30) * 2 / 3;
	return ((pt.x >= tmpx) && (pt.x <= (tmpx + img[cur_app].GetWidth() * 2 / 3)) && (pt.x >= tmpy) && (pt.y <= (tmpy + (img[cur_app].GetHeight() + 30) * 2 / 3)));
}
bool cgame::cgame_character::clicked_left() const { return (touch_mouse() && (KEY_DOWN(VK_LBUTTON))); }
bool cgame::cgame_character::clicked_right() const { return (touch_mouse() && (KEY_DOWN(VK_RBUTTON))); }
float cgame::cgame_character::distance(const cgame::cgame_character& char1) const
{
	float charac1centrex = x + float(img[cur_app].GetWidth()) / 2;
	float charac1centrey = y + float(img[cur_app].GetHeight()) / 2;
	float charac2centrex = char1.x + float(((char1.img)[char1.cur_app]).GetWidth()) / 2;
	float charac2centrey = char1.y + float(((char1.img)[char1.cur_app]).GetHeight()) / 2;
	float xminus = abs(charac1centrex - charac2centrex);
	float yminus = abs(charac1centrey - charac2centrey);
	return sqrt(xminus * xminus + yminus * yminus);
}
float cgame::cgame_character::distance_to_cursor() const
{
	float characcentrex = x + float(img[cur_app].GetWidth()) / 2;
	float characcentrey = y + float(img[cur_app].GetHeight()) / 2;
	POINT pt;
	GetCursorPos(&pt);
	float tmpx = float(pt.x) * 3 / 2;
	float tmpy = float(pt.y) * 3 / 2 - 30;
	float xminus = abs(characcentrex - tmpx);
	float yminus = abs(characcentrey - tmpy);
	return sqrt(xminus * xminus + yminus * yminus);
}
cgame::cgame_character::~cgame_character() { delete[] img; }
USI cgame::cgame_character::randint(USI x, USI y) { return (rand() % (y - x + 1)) + x; }
void cgame::cgame_clock::start() { clocks = clock(); }
float cgame::cgame_clock::second() const { return float(clock() - clocks) / CLOCKS_PER_SEC; }
void cgame::init_rand() { srand(time(0)); }
void cgame::disable_quick_edit_mode()
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(hStdin, mode);
}
void cgame::init_bg_rgb(BYTE r, BYTE g, BYTE b, HWND hwnd)
{
	cgame::R = r; cgame::G = g; cgame::B = b;
	CImage newimg;
	HDC hdc = GetDC(hwnd);
	newimg.Create(25, 25, 32);
	for (BYTE i = 0; i < 25; i++)
		for (BYTE j = 0; j < 25; j++)
			newimg.SetPixelRGB(i, j, r, g, b);
	for (USI i = 0; i < 1892; i += 25)
		for (USI j = 0; j < 985; j += 25)
			newimg.Draw(hdc, i, j);
}
void cgame::clear_bg()
{ 
	system("cls");
	cgame::R = 0;
	cgame::G = 0;
	cgame::B = 0;
}
void cgame::play_music(const char* filename) { CString st = filename; PlaySound((LPCTSTR)st, NULL, SND_FILENAME); }
void cgame::play_music_repeat(const char* filename) { CString st = filename; PlaySound((LPCTSTR)st, NULL, SND_FILENAME | SND_LOOP | SND_ASYNC); }
bool cgame::press_key(char c) { return (KEY_DOWN(c)); }
void cgame::game_exit(const char* error_message)
{
	CString st = error_message;
	MessageBox(GetConsoleWindow(), LPCTSTR(st), _T("Error"), MB_OK);
	exit(EXIT_FAILURE);
}
