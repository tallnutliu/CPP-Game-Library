#ifndef _CGAME_H_
#define _CGAME_H_

#include <atlimage.h>
#include <ctime>
typedef unsigned short USI;
class cgame
{
public:
	class cgame_character;
	class cgame_complex_matrix
	{
	public:
		friend class cgame_character;
		//默认构造函数
		cgame_complex_matrix();
		//生成一个矩阵（分别是宽和高）
		bool create(USI, USI);
		//摧毁原矩阵，重新生成新矩阵
		bool recreate(USI, USI);
		//摧毁原矩阵【警告：调用create()或recreate()且使用完毕后必须调用一次此函数】
		void destroy();
		//使用 RGB 值初始化矩阵【警告：未调用此函数时内存中值将是不确定的】
		void init(BYTE, BYTE, BYTE);
		//设置某一个像素的 RGB 值
		bool set_pixel(USI, USI, BYTE, BYTE, BYTE);
		//附加一个正方形到矩阵，第一个参数是边长，后两个是左上角坐标，最后是 RGB 颜色值
		bool attach_square(USI, USI, USI, BYTE, BYTE, BYTE);
		//附加一个长方形到矩阵，第一二个参数是宽和高
		bool attach_rect(USI, USI, USI, USI, BYTE, BYTE, BYTE);
	private:
		struct RGB { BYTE r; BYTE g; BYTE b; };
		USI width, height;
		RGB** buffer;
	};
	class cgame_character
	{
	public:
		/*构造函数*/
		//默认构造函数（这样的话最多一个造型）
		cgame_character();
		//BYTE构造函数，传入的参数表示最多有几个造型（不能超过 255）
		cgame_character(BYTE);
		//复制构造函数
		cgame_character(const cgame_character&);
		//移动构造函数
		cgame_character(cgame_character&&);

		/*赋值函数*/
		const cgame_character& operator=(const cgame_character&);

		/*添加特殊形状的造型函数，必须和cgame_complex_matrix配合使用*/
		bool load_from_complex_matrix(const cgame_complex_matrix&);

		/*图像载入及显示函数，返回值表示是否成功*/
		//加载图片，输入图片名，要求图片位于项目目录下。
		bool load(const char*);
		//显示
		bool show() const;
		//显示，设置不透明度（从 0 到 255，数值越小越透明）
		bool show_transparent(BYTE) const;
		//隐藏
		bool hide() const;

		/*设置其他指标函数，返回值表示是否成功（除 next_app() 之外，它没有参数）*/
		//设置x坐标，左上角为 0，越往右数值越大
		bool set_x(USI);
		//设置y坐标，左上角为 0，越往下数值越大
		bool set_y(USI);
		//同时设置xy坐标
		bool set_posi(USI, USI);
		//设置角色造型，从 0 开始
		bool set_app(BYTE);
		//切换至下一个造型
		void next_app();

		/*随机设置函数，返回值表示是否成功（除 rand_app() 之外，它没有参数）*/
		//移到随机 x 坐标，省略所有参数则全屏幕随机，两参数表示取值范围
		bool rand_x(USI start_x = 0, USI end_x = 1892);
		//移到随机 y 坐标，省略所有参数则全屏幕随机，两参数表示取值范围
		bool rand_y(USI start_y = 0, USI end_y = 985);
		//移到随机位置，省略所有参数则全屏幕随机，前两参数表示区域左上角 x、y，后两参数表示右下角 x、y，移到该长方区域内的随机位置
		bool rand_posi(USI start_x = 0, USI start_y = 0, USI end_x = 1892, USI end_y = 985);
		//设置为随机造型
		void rand_app();

		/*获取指标函数，返回值表示指标值*/
		//获取 x 坐标
		USI get_x() const;
		//获取 y 坐标
		USI get_y() const;
		//获取当前造型编号
		BYTE get_cur_app() const;

		/*移动和旋转函数，返回值表示是否成功【警告：内部移动后在屏幕上并不会移动，需先调用 hide()，在调用完此函数后再调用 show()】*/
		//向右移动
		bool move_right(short);
		//向上移动
		bool move_up(short);
		//移到光标【警告：必须在最大化状态下使用！否则坐标会有大幅度偏差】
		void move_to_cursor();
		//移到另一个角色
		void move_to_character(const cgame_character&);

		/*侦测函数，返回值表示侦测值是否为真（除最后一个外，它用于获取信息）*/
		//传入另一个 cgame_character 类，侦测是否相碰
		bool bump(const cgame_character&) const;
		//侦测是否在舞台边缘上
		bool is_side_of_screen() const;
		//侦测是否在舞台上边缘
		bool is_up_side_screen() const;
		//侦测是否在舞台下边缘
		bool is_down_side_screen() const;
		//侦测是否在舞台左边缘
		bool is_left_side_screen() const;
		//侦测是否在舞台右边缘
		bool is_right_side_screen() const;
		//侦测碰到鼠标【警告：必须在最大化状态下使用！否则坐标会有大幅度偏差】
		bool touch_mouse() const;
		//侦测被鼠标左键点击【警告：必须在最大化状态下使用！否则坐标会有大幅度偏差】
		bool clicked_left() const;
		//侦测被鼠标右键点击【警告：必须在最大化状态下使用！否则坐标会有大幅度偏差】
		bool clicked_right() const;
		//侦测和本角色中心点和另一个角色中心点的距离
		float distance(const cgame_character&) const;
		//侦测本角色中心点和光标的距离【警告：必须在全屏状态下使用！否则坐标会有大幅度偏差】
		float distance_to_cursor() const;

		/*析构函数*/
		~cgame_character();
	private:
		USI x, y;
		BYTE max_app_num, cur_app_num, cur_app;
		CImage* img;
		//私有方法，获取[a,b]内的随机数
		USI randint(USI, USI);
	};
	class cgame_clock
	{
	public:
		//开始计时
		void start();
		//获取计时开始后经过的秒数
		float second() const;
	private:
		time_t clocks;
	};
	//初始化随机数函数【警告：调用 cgame_character 的 rand 系列函数之前必须调用一次此函数】
	static void init_rand();
	//移除快速编辑模式函数【警告：对于Win10，使用 cgame_character::clicked_left() 和 cgame::clicked_right() 前必须调用一次此函数】
	static void disable_quick_edit_mode();
	//设置全舞台为某个 RGB 颜色
	static void init_bg_rgb(BYTE, BYTE, BYTE, HWND);
	//清屏
	static void clear_bg();
	//播放音乐，参数要求同 cgame_character::load()（可用于特殊音效，如收集金币）
	static void play_music(const char*);
	//重复播放音乐，参数要求同 cgame_character::load()（可用于背景音乐）
	static void play_music_repeat(const char*);
	//侦测是否按下某键
	static bool press_key(char);
	//退出游戏，传入的参数为错误消息
	static void game_exit(const char*);
private:
	static BYTE R, G, B;
};

#endif
