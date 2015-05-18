#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "include\matrix.h"
#include "include\Perceptron.h"
#include "include\kdTree.h"

using namespace std;
using namespace matrixlab;

typedef double  Type;
/* m 算法模式(0:原始, 1:对偶)
   w 算法参数w0
   b 算法参数b0
   r 算法学习速率
   a 算法参数a0(对偶形式)*/
#define DEFAULT_PARAM "-m 0 -w 0 -b 0 -r 1.0 -a 0" 
#define XLEN 500
#define YLEN 500
#define DrawLine(dc,x1,y1,x2,y2,c) \
	do { \
		HPEN hpen = CreatePen(PS_SOLID,0,c); \
		HPEN horig = SelectPen(dc,hpen); \
		MoveToEx(dc,x1,y1,NULL); \
		LineTo(dc,x2,y2); \
		SelectPen(dc,horig); \
		DeletePen(hpen); \
	} while(0)

using namespace std;
// 画刷颜色表
COLORREF colors[] =
{
	RGB(0,0,0),
	RGB(0,120,120),
	RGB(120,120,0),
	RGB(120,0,120),
	RGB(0,200,200),// brush1
	RGB(200,200,0),// brush2
};

HWND main_window;
HBITMAP buffer;
HDC window_dc;
HDC buffer_dc;
HBRUSH brush1, brush2;
HWND edit;
enum {
	ID_BUTTON_CHANGE, ID_BUTTON_RUN, ID_BUTTON_CLEAR, ID_EDIT
};
/// 特征点结构体( 坐标 和 对应的value)
struct point {
	Type x, y;
	signed char value;
};

vector<point> point_v;
/// 特征点默认value为1
int current_value = 1;
/// 回调函数
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
/// 主函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		   PSTR szCmdLine, int iCmdShow)
{
	static char szAppName[] = "SvmToy";
	MSG msg;
	WNDCLASSEX wndclass;
	/// 窗口类参数设置
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc; ///窗口处理函数(回调)的指针
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	
	/// 注册窗口类
	RegisterClassEx(&wndclass);

	/// 创建窗口
	main_window = CreateWindow(szAppName,	// window class name
				    "SVM Toy",	// window caption
				    WS_OVERLAPPEDWINDOW,// window style
				    CW_USEDEFAULT,	// initial x position
				    CW_USEDEFAULT,	// initial y position
				    XLEN,	// initial x size
				    YLEN+60,	// initial y size
				    NULL,	// parent window handle
				    NULL,	// window menu handle
				    hInstance,	// program instance handle
				    NULL);	// creation parameters
	/// 显示更新窗口
	ShowWindow(main_window, iCmdShow);
	UpdateWindow(main_window);
	/// 按钮
	CreateWindow("button", "Change", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		     0, YLEN, 50, 25, main_window, (HMENU) ID_BUTTON_CHANGE, hInstance, NULL);
	CreateWindow("button", "Run", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		     50, YLEN, 50, 25, main_window, (HMENU) ID_BUTTON_RUN, hInstance, NULL);
	CreateWindow("button", "Clear", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		     100, YLEN, 50, 25, main_window, (HMENU) ID_BUTTON_CLEAR, hInstance, NULL);
	edit = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE,
		            150, YLEN, 350, 25, main_window, (HMENU) ID_EDIT, hInstance, NULL);
	/// 编辑框
	Edit_SetText(edit,DEFAULT_PARAM);
	/// 画刷
	brush1 = CreateSolidBrush(colors[4]);
	brush2 = CreateSolidBrush(colors[5]);

	window_dc = GetDC(main_window);
	buffer = CreateCompatibleBitmap(window_dc, XLEN, YLEN);
	buffer_dc = CreateCompatibleDC(window_dc);
	SelectObject(buffer_dc, buffer);
	PatBlt(buffer_dc, 0, 0, XLEN, YLEN, BLACKNESS);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
/// 排序方式
bool SortByPointX1( const point &v1, const point &v2 )
{
	return v1.x < v2.x;
}
/// 清除
void clear_all()
{
	point_v.clear();
	PatBlt(buffer_dc, 0, 0, XLEN, YLEN, BLACKNESS);
	InvalidateRect(main_window, 0, 0);
}
/// \brief 选择画刷颜色
/// \param v 特征点值
/// \return 画刷
HBRUSH choose_brush(int v)
{
	if(v==1) return brush1;
	else if(v==2) return brush2;
}
/// \brief 画一个特征点
/// \param p 特征点信息
void draw_point(const point & p)
{
	RECT rect;
	rect.left = int(p.x*XLEN);
	rect.top = int(p.y*YLEN);
	rect.right = int(p.x*XLEN) + 3;
	rect.bottom = int(p.y*YLEN) + 3;
	FillRect(window_dc, &rect, choose_brush(p.value));
	FillRect(buffer_dc, &rect, choose_brush(p.value));
}
/// \brief 画所有特征点
/// \param p 特征点信息
void draw_all_points()
{
	for(vector<point>::iterator p = point_v.begin(); p != point_v.end(); p++)
		draw_point(*p);
}
void button_run_clicked()
{
	// guard
	if(point_v.empty()) return;

	// parse options参数选择  
	char str[1024];
	Edit_GetLine(edit, 0, str, sizeof(str));
	const char *p = str;
	int tsize = point_v.size();
	bool mode;
	vector<Type> w0(2);
	Type b0;
	Type rate;
	vector<Type> a0( tsize );
	Matrix<Type> xy(tsize, 2);
	vector<int> value(tsize);
	int i,j;
	for( i=0; i<tsize; i++)
	{
		xy[i][0] = point_v.at(i).x;
		xy[i][1] = point_v.at(i).y;
		value[i] = point_v.at(i).value;
	}
	while (1) {
		while (*p && *p != '-')
			p++;
		if (*p == '\0')
			break;
		p++;
		switch (*p++) {
		case 'm':
			mode = atoi(p);
			break;
		case 'w':
			for( i=0; i<2; i++)
				w0[i] = atoi(p);
			break;
		case 'b':
			b0 = atoi(p);
			break;
		case 'r':
			rate = atoi(p);
			break;
		case 'a':
			for( i=0; i<tsize; i++)
				a0[i] = atoi(p);
		break;
		}
	}
	Perceptron<Type> Per( w0, b0, tsize, 2, rate);
	if( mode == 0)
	{
		Per.basic_Perceptron( xy, value);/// 原始感知器
	}else
	{
		Per = Perceptron<Type>( w0, b0, tsize, 2, rate, a0 );/// 对偶感知器
		Per.dual_Perceptron( xy, value, a0);
	}
	w0 = Per.getw();
	b0 = Per.getb();
	vector<point> pointtmp(tsize-1);
	pointtmp = point_v;
	sort( pointtmp.begin(), pointtmp.end(), SortByPointX1 ); 
	double x1,x2,y1,y2;
	x1 = pointtmp.at(0).x;
	y1 = -(x1 * w0[0] + b0) / w0[1];
	x2 = pointtmp.at(tsize-1).x;
	y2 = -(x2 * w0[0] + b0) / w0[1];
	/// 划线
	DrawLine(window_dc, x1 * XLEN ,y1 * YLEN, x2 * XLEN, y2 * YLEN,colors[mode+1]);

	double data[6][2] = {{2,3},{5,4},{9,6},{7,2},{8,1},{4,7}};
	Matrix<Type> x(6,2);
	for( int i=0; i<6; i++)
		for( int j=0; j<2; j++)
			x[i][j] = data[i][j];
	vector<Type> y(6);
	kdTree<Type> kd( x, y);
	kd.CreateKDTree( );

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (iMsg) {
	case WM_LBUTTONDOWN: /// 鼠标左键
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			point p = {(Type)x/XLEN, (Type)y/YLEN, current_value};
			point_v.push_back(p);
			draw_point(p);
		}
		return 0;
	case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			BitBlt(hdc, 0, 0, XLEN, YLEN, buffer_dc, 0, 0, SRCCOPY);
			EndPaint(hwnd, &ps);
		}
		return 0;
	case WM_COMMAND: /// 菜单响应
		{
			int id = LOWORD(wParam);
			switch (id) {
			case ID_BUTTON_CHANGE: /// 更换类别(最多2种)
				if(current_value == 1) current_value = -1;
				else current_value = 1;
				break;
			case ID_BUTTON_RUN: /// 运行
				button_run_clicked();
				break;
			case ID_BUTTON_CLEAR:/// 清除
				clear_all();				
				break;
			}
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	/// 默认的窗口处理函数
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}