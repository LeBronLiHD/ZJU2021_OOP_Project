
// HaveFunDrivingPlaneView.cpp: CHaveFunDrivingPlaneView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "HaveFunDrivingPlane.h"
#endif

#include "HaveFunDrivingPlaneDoc.h"
#include "HaveFunDrivingPlaneView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHaveFunDrivingPlaneView

IMPLEMENT_DYNCREATE(CHaveFunDrivingPlaneView, CView)

BEGIN_MESSAGE_MAP(CHaveFunDrivingPlaneView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_GAMEOPTION_NAVIGATION, &CHaveFunDrivingPlaneView::OnGameoptionNavigation)
	ON_COMMAND(ID_GAMEOPTION_NAVIGATION, &CHaveFunDrivingPlaneView::OnGameoptionNavigation)
	ON_COMMAND(ID_GAMEOPTION_NAVIGATIONMAP, &CHaveFunDrivingPlaneView::OnGameoptionNavigationmap)
END_MESSAGE_MAP()

// CHaveFunDrivingPlaneView 构造/析构

CHaveFunDrivingPlaneView::CHaveFunDrivingPlaneView() noexcept
{
	// TODO: 在此处添加构造代码
	xtexa1 = xtexa2 = 0;
	m_xAngle = m_yAngle = 0;
	fowardSpeed = 0;
	//backwardSpeed = 0;
	l_r_Speed = 0;
	moveForward = false;
	moveLeft = false;
	moveRight = false;
	moveBackward = false;
	altDown == false;
	moveSky = 0;
	a = 0;
}

CHaveFunDrivingPlaneView::~CHaveFunDrivingPlaneView()
{
}

BOOL CHaveFunDrivingPlaneView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CHaveFunDrivingPlaneView 绘图

void CHaveFunDrivingPlaneView::OnDraw(CDC* /*pDC*/)
{
	CHaveFunDrivingPlaneDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	RenderScene();
}


// CHaveFunDrivingPlaneView 打印

BOOL CHaveFunDrivingPlaneView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CHaveFunDrivingPlaneView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CHaveFunDrivingPlaneView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CHaveFunDrivingPlaneView 诊断

#ifdef _DEBUG
void CHaveFunDrivingPlaneView::AssertValid() const
{
	CView::AssertValid();
}

void CHaveFunDrivingPlaneView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHaveFunDrivingPlaneDoc* CHaveFunDrivingPlaneView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHaveFunDrivingPlaneDoc)));
	return (CHaveFunDrivingPlaneDoc*)m_pDocument;
}
#endif //_DEBUG


// CHaveFunDrivingPlaneView 消息处理程序


// 初始化OpenGL
BOOL CHaveFunDrivingPlaneView::InitializeOpenGL()
{
	//SetWindowText(reinterpret_cast<LPCTSTR>("A Simple Plane Game"));
	PIXELFORMATDESCRIPTOR pfd;
	int n;
	m_pDC = new CClientDC(this);
	ASSERT(m_pDC != NULL);
	// 设置当前的绘图像素格式  
	if (!SetupPixelFormat())
	{
		return FALSE;
	}

	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);
	// 创建绘图描述表  
	m_hRC = wglCreateContext(m_pDC->GetSafeHdc());
	if (m_hRC == NULL)
	{
		return FALSE;
	}
	// 使绘图描述表为当前调用现程的当前绘图描述表  
	if (wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC) == FALSE)
	{
		return FALSE;
	}
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	listSky = sky.ValueOfSkyEstablishment (720, 30, 32);//经度最好是能被360整除，纬度最好能被半径整除
	listTerrain = terrain.ValueOfGlobalMapEstablishment();//在最开始创建窗口的时候就先将列表创建好，在下面直接用
	listLight = LightModle();//将光照模型列表创建好，不要再rendersence中调用此行，不然没有提高任何效率

	listGrid = terrain.ValueOfGridEstablishment();
	listPlane = Plane.CreatPlane();
	listHouse = house.CreatHouse();
	//listWater = water.CreatWater();
	return TRUE;
}


// 设置像素格式
BOOL CHaveFunDrivingPlaneView::SetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // pfd结构的大小   
		1,                              // 版本号   
		PFD_DRAW_TO_WINDOW |            // 支持在窗口中绘图   
		PFD_SUPPORT_OPENGL |            // 支持 OpenGL   
		PFD_DOUBLEBUFFER,               // 双缓存模式   
		PFD_TYPE_RGBA,                  // RGBA 颜色模式   
		32,                             // 24 位颜色深度   
		0, 0, 0, 0, 0, 0,               // 忽略颜色位   
		0,                              // 没有非透明度缓存   
		0,                              // 忽略移位位   
		0,                              // 无累计缓存   
		0, 0, 0, 0,                     // 忽略累计位   
		32,                             // 32 位深度缓存       
		0,                              // 无模板缓存   
		0,                              // 无辅助缓存   
		PFD_MAIN_PLANE,                 // 主层   
		0,                              // 保留   
		0, 0, 0                         // 忽略层,可见性和损毁掩模   

	};
	int pixelFormat;
	// 为设备描述表得到最匹配的像素格式   
	if ((pixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0)
	{
		MessageBox(_T("ChoosePixelFormat failed"));
		return FALSE;
	}
	// 设置最匹配的像素格式为当前的像素格式   
	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd) == FALSE)
	{
		MessageBox(_T("SetPixelFormat failed"));
		return FALSE;
	}
	return TRUE;
}


int CHaveFunDrivingPlaneView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetTimer(1, 50, NULL);
	//SetTimer(2, 50, NULL);

	if (InitializeOpenGL())
	{
		return 0;
	}
	return 0;
}

BOOL CHaveFunDrivingPlaneView::OnEraseBkgnd(CDC* pDC)//避免windows自己的窗口刷新闪烁
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}


void CHaveFunDrivingPlaneView::OnDestroy()
{
	CView::OnDestroy();

	// TODO:  在此处添加消息处理程序代码
	m_hRC = ::wglGetCurrentContext();
	if (::wglMakeCurrent(0, 0) == FALSE)
	{
		MessageBox(_T("Could not make RC non-current"));
	}

	if (m_hRC)
	{
		if (::wglDeleteContext(m_hRC) == FALSE)
		{
			MessageBox(_T("Could not delete RC"));
		}
	}

	if (m_pDC)
	{
		delete m_pDC;
	}
	glDeleteLists(1, 2);//删除从1开始数，数量个2数的显示列表（即删除1，2两个显示列表）
	m_pDC = NULL;
	KillTimer(1);
}

void CHaveFunDrivingPlaneView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	m_wide = cx; m_height = cy;
	// TODO:  在此处添加消息处理程序代码
	//避免除数为0  
	if (cy == 0)
	{
		cy = 1;
	}
	//设置视口与窗口的大小  
	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)cx / (GLfloat)cy, 0.9f, 500000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int CHaveFunDrivingPlaneView::LightModle()
{
	//光源
	GLuint List = glGenLists(1);
	glNewList(List, GL_COMPILE);
	GLfloat lgtposition[] = { 500, 200, 0, 0 };
	GLfloat ambi[] = { 0.1, 0.1, 0.1, 1 };
	GLfloat diff[] = { 1, 1, 1, 1 };
	GLfloat spec[] = { 1, 1, 1, 1 };
	GLfloat _SPOT_DIRECTION[] = { -50.0f, -20.0f, 0.0f };//表示光的方向，原点到改点为方向
	GLfloat _GL_SPOT_EXPONENT[] = { 30.0f };//聚合度，越大越聚，对位置光源才有效
	GLfloat _GL_SPOT_CUTOFF[] = { 30.0f };//手电发光角度的一半，对位置光源才有效

	glLightfv(GL_LIGHT0, GL_POSITION, lgtposition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, _SPOT_DIRECTION);
	glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, _GL_SPOT_EXPONENT);
	glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, _GL_SPOT_CUTOFF);

	GLfloat ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuse[] = { 1, 1, 1, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat emission[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //自身发微弱该光，此处没让他发光
	GLfloat shininess = 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_COLOR_MATERIAL);
	glEndList();
	return List;
}

void CHaveFunDrivingPlaneView::FogModle()
{
	// 设置雾化效果
	float fog_r = 211.f / 255.f;
	float fog_g = 237.f / 255.f;
	float fog_b = 254.f / 255.f;
	GLuint	fogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR };	// 保存三种雾的模式
	GLuint	fogfilter = 0;								// 当前使用的雾的模式 
	GLfloat	fogColor[4] = { fog_r, fog_g, fog_b, 1 };		// 雾的颜色

	glFogi(GL_FOG_MODE, fogMode[2]);			        // 设置雾的模式
	glFogfv(GL_FOG_COLOR, fogColor);					// 设置雾的颜色
	glFogf(GL_FOG_DENSITY, 0.295f);						// 设置雾的浓度
	glHint(GL_FOG_HINT, GL_NICEST);					    // Fog Hint Value
	glFogf(GL_FOG_START, 200.0f);						// 设置雾的开始深度
	glFogf(GL_FOG_END, 700);							// 设置雾的结束深度
	glEnable(GL_FOG);									// 使用雾
}

//整个过程飞机和视野gllookat坐标一直没变，键盘移动是控制的地形移动
//看起来像飞机在动，这样做的目的是为了鼠标旋转是绕着这飞机转的
void CHaveFunDrivingPlaneView::RenderScene()
{
	//设置清屏颜色 
	glClearColor(211.f / 255.f, 237.f / 255.f, 254.f / 255.f, 1);
	//清除颜色缓冲区和深度缓冲区 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//透视投影变换  
	glEnable(GL_TEXTURE_2D);
	//启用纹理，不能掉
	//使用光照,放在这里光源会随视野移动而移动
	glCallList(listLight);;
	//视角变换  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0, 5, 30, 0, 5, -30, 0, 1, 0);
	//5是为了视野比飞机稍高（飞机在原点）30是为了视野在飞机后面点
	//gluLookAt(0, 500, 1800, 0, 0, 0, 0, 1, 0);	//观全景用这行代码
	//使用光照，放这里光源不会随着视野移动而移动，所以旋转时物体的影子会变化
	glCallList(listLight);	
	//listLight在initialopengl函数中被初始化
	//sky.ValueOfSunEstablishment();//Error
	//使用雾
	FogModle();

	//alt没按时控制鼠标移动
	if (altDown == false)
	{
		glRotatef(m_xAngle, 0, 1, 0);//左右转
		glRotatef(m_yAngle, cos(m_xAngle*c), 0, sin(m_xAngle*c));//上下转
	}

	//绘制飞机
	glPushMatrix();				//进栈的好处是好处是对模型进行平移和旋转不会影响其他模型
	glRotated(-90, 1, 0, 0);	//旋转后z轴向上
	glRotated(180, 0, 0, 1);	//摆正飞机
	glCallList(listPlane);
	glPopMatrix();

	glTranslated(l_r_Speed, 0, -fowardSpeed);					//下移坐标，为绘制地形做准备
	glRotatef(m_xAngle, 0, 1, 0);								//左右转
	glRotatef(m_yAngle, cos(m_xAngle*c), 0, sin(m_xAngle*c));	//上下转
	//绘制地形（房子相对地形静止，所以可放在同一个栈）
	glPushMatrix();//进栈的好处是好处是对模型进行平移和旋转不会影响其他模型

	glTranslated(0, -40, -0);						//下移坐标，为绘制地形做准备

	glCallList(listTerrain);
	//绘制房子
	glRotated(-90, 1, 0, 0);		//旋转使房子正立，此房子z轴向上
	glTranslated(20, 0, 6);			//绕z轴上升6，大概和地面接触，z轴不是地图的z轴，是房子的z轴
	glCallList(listHouse);
	//创造第二个房子
	glTranslated(10, 150, 0);		//第一个房子已经下移了坐标轴，所以画第二个不必移动
	glRotated(90, 0, 0, 1);
	glCallList(listHouse);
	glPopMatrix();

	//glCallList(listGrid);//显示网格模型，需注释glCallList(listTerrain);
	//绘制天空
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glRotatef(moveSky, 0, 1, 0);	//球体随时间旋转，看起来云在动
	glTranslated(0, -150, 0);
	glCallList(listSky);
	glPopMatrix();

	//glCallList(listWater);
	glPushMatrix();
	glTranslated(-255, -33, 0);
	water.CreatWater(a);
	glPopMatrix();

	glFinish();
	SwapBuffers(wglGetCurrentDC());
}

void CHaveFunDrivingPlaneView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	mousePoint = point;
	CView::OnLButtonDown(nFlags, point);
}

void CHaveFunDrivingPlaneView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nFlags&MK_LBUTTON) {
		m_yAngle += (point.y - mousePoint.y) / 3.6;
		m_xAngle += (point.x - mousePoint.x) / 3.6;
		//Set the mouse point
		mousePoint = point;
		InvalidateRect(NULL, FALSE);

	}
	CView::OnMouseMove(nFlags, point);
}


void CHaveFunDrivingPlaneView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (moveForward == true)
		fowardSpeed -= 2;	 //speed不是速度，是每单位时间移动的距离，值越大，
							 //看起来速度越快,用-=是因为gllookat是从z的正半轴看向负半轴
	if (moveLeft == true)
		l_r_Speed += 2;
	if (moveRight == true)
		l_r_Speed -= 2;
	if (moveBackward == true)
		fowardSpeed += 2;

	moveSky += 0.04;		//运动的云朵是旋转球体实现的
	if (moveSky >= 360)
		moveSky = 0;

	a++;
	if (a > 8)a = 0;

	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}


void CHaveFunDrivingPlaneView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nChar == 87 || nChar == 119)		// 按下'W'键
		moveForward = true;					//启动时间，接着见OnTimer函数
	if (nChar == 65 || nChar == 97)
		moveLeft = true;
	if (nChar == 68 || nChar == 100)
		moveRight = true;
	if (nChar == 83 || nChar == 115)
		moveBackward = true;
	if (nChar == 32)//alt按下
		altDown == true;

	//按下ESC切换全屏显示
	if (nChar == 27)
	{
		CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
		pMain->FullScreenModeSwitch();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CHaveFunDrivingPlaneView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nChar == 87 || nChar == 119)		// 'W'键松开
		moveForward = false;//启动时间，接着见OnTimer函数
	if (nChar == 83 || nChar == 115)
		moveBackward = false;
	if (nChar == 65 || nChar == 97)
		moveLeft = false;
	if (nChar == 68 || nChar == 100)
		moveRight = false;
	if (nChar == 32)//alt松开
		altDown == false;
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}



void CHaveFunDrivingPlaneView::OnGameoptionNavigation()
{
	// TODO: 在此添加命令处理程序代码
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2f(-0.5, 0.5);
	glVertex2f(-0.5, -0.5);
	glColor3f(1.0, 1.0, 1.0);
	glEnd();

	glPopMatrix();
	// 交换缓冲区  
	SwapBuffers(wglGetCurrentDC());
}


void CHaveFunDrivingPlaneView::OnGameoptionNavigationmap()
{
	// TODO: 在此添加命令处理程序代码
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2f(-0.5, 0.5);
	glVertex2f(-0.5, -0.5);
	glColor3f(1.0, 1.0, 1.0);
	glEnd();
	SetTimer(1, 5000, NULL);
	glPopMatrix();
	// 交换缓冲区  
	//SwapBuffers(wglGetCurrentDC());
}
