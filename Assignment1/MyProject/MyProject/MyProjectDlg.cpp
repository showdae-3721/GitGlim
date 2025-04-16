// MyProjectDlg.cpp : 구현 파일

#include "stdafx.h"
#include "MyProject.h"
#include "MyProjectDlg.h"
#include "afxdialogex.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <random>

using namespace chrono;

#ifdef _DEBUG
#define new DEBUG_NEW
	#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CMyProjectDlg 대화 상자
CMyProjectDlg::CMyProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MYPROJECT_DIALOG, pParent)
	, m_radius(12)
	, m_thickness(5)
	, m_dragIndex(-1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CMyProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_radius);
	DDV_MinMaxInt(pDX, m_radius, 1, 20);
	DDX_Text(pDX, IDC_EDIT_THICKNESS, m_thickness);
	DDV_MinMaxInt(pDX, m_thickness, 1, 10);
}


BEGIN_MESSAGE_MAP(CMyProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_INIT, &CMyProjectDlg::OnBnClickedBtnInit)
	ON_BN_CLICKED(IDC_BTN_RANDOM, &CMyProjectDlg::OnBnClickedBtnRandom)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_USER_DRAW_DONE, &CMyProjectDlg::OnDrawDone)
END_MESSAGE_MAP()


// CMyProjectDlg 메시지 처리기
BOOL CMyProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	InitImgBuffer();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


BOOL CMyProjectDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_image.Destroy();

	return CDialogEx::DestroyWindow();
}


void CMyProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.
void CMyProjectDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if (m_image)
			m_image.Draw(dc, 0, 0);

		CDialogEx::OnPaint();
	}
}


// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMyProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMyProjectDlg::InitImgBuffer()
{
	// 이미지 포멧
	int nWidth	= 500;
	int nHeight	= 500;
	int nBpp	= 8;

	m_image.Create(nWidth, -nHeight, nBpp); // 이미지 생성
	if (nBpp == 8) // 모노의 경우 색상 테이블 처리 필요
	{
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		m_image.SetColorTable(0, 256, rgb);
	}

	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits(); // 이미지 첫번째 위치 포인터 얻기

	memset(fm, 0xff, nWidth * nHeight); // 이미지 메모리 초기화

	Invalidate(); // OnPaint() 호출
}


void CMyProjectDlg::DrawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray, bool fill)
{
	int nCenterX = x + nRadius;
	int nCenterY = y + nRadius;
	int nPitch = m_image.GetPitch(); // 열의 개수 얻기

	for (int j = y; j < y + nRadius * 2; j++) // 행
	{
		for (int i = x; i < x + nRadius * 2; i++) // 열
		{
			// 메모리를 벗어나면 터지는 경우 예외 처리
			if (ValidImgPos(i, j))
				if (IsInCircle(i, j, nCenterX, nCenterY, nRadius, fill)) // 마우스 좌표, 센터값, 반지름
					fm[j * nPitch + i] = nGray; // 원에 해당되는 픽셀값만 변경
		}
	}

	// 스레드에서 Invalidate() 사용시 충돌/오작동 발생
	// Invalidate();

	// 스레드에서 화면 갱신 요청 메시지를 보내고 메인 스레드에서 처리
	::PostMessage(GetSafeHwnd(), WM_USER_DRAW_DONE, 0, 0);
}


bool CMyProjectDlg::IsInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius, bool fill)
{
	// 점이랑 원의 중심 사이의 거리를 계산
	// 거리가 반지름보다 짧으면 -> 원 안에 있다 -> true
	// 거리가 반지름보다 길면 -> 원 밖에 있다 -> false

	bool bRet = false;

	double dX = i - nCenterX; // 점과 원의 중심 간의 x축 거리
	double dY = j - nCenterY; // 점과 원의 중심 간의 y축 거리
	double dDist = dX * dX + dY * dY; // 거리의 제곱값

	if (fill == true)
	{
		// 거리의 제곱이 반지름의 제곱보다 작다면 [즉 점이 원의 내부에 있다면]
		if (dDist < nRadius * nRadius)
			bRet = true;

		return bRet;
	}
	else
	{
		// 외곽선만 리턴
		double outerRadius = nRadius;
		double innerRadius = nRadius - m_thickness; // 외곽선 두께 조절

		//if (dDist >= (nRadius - 1.5) * (nRadius - 1.5) && dDist <= nRadius * nRadius)
		if (dDist >= innerRadius * innerRadius && dDist <= outerRadius * outerRadius)
			bRet = true;

		return bRet;
	}
}


void CMyProjectDlg::SumCircle(int nGray)
{
	////////////////////////////////////////////////////////////////////
	// 3점을 지나는 외접원의 중심점 구하기
	////////////////////////////////////////////////////////////////////
	if (m_clickPoints.size() == CLICKCNT)
	{
		// CPoint[int] -> double 형변환
		double x1 = m_clickPoints[0].x, y1 = m_clickPoints[0].y;
		double x2 = m_clickPoints[1].x, y2 = m_clickPoints[1].y;
		double x3 = m_clickPoints[2].x, y3 = m_clickPoints[2].y;

		// 세 점이 일직선인지 확인하는 판별식
		double ret = x1 * (y2 - y3) - y1 * (x2 - x3) + x2 * y3 - x3 * y2;

		// 허용 오차 [1e-2 = 0.01]
		if (fabs(ret) < 1500)
		{
			cout << "SumCircle(): 직선에 가까움" << fabs(ret) << endl;
			return;
		}

		double A1 = x1 * x1 + y1 * y1;
		double B1 = x2 * x2 + y2 * y2;
		double C1 = x3 * x3 + y3 * y3;

		double D = 2 * (x1 * (y2 - y3) + x2 *(y3 - y1) + x3 * (y1 - y2));
		double cx = (A1 * (y2 - y3) + B1 * (y3 - y1) + C1 * (y1 - y2)) / D;
		double cy = (A1 * (x3 - x2) + B1 * (x1 - x3) + C1 * (x2 - x1)) / D;

		CPoint center((int)cx, (int)cy);

		// 반지름 계산 (임의 점 A와 중심 거리)
		double dx = x1 - cx;
		double dy = y1 - cy;
		int exactRadius = (int)sqrt(dx * dx + dy * dy);

		unsigned char* fm = (unsigned char*)m_image.GetBits();

		// 3개의 지점을 지나는 정원 그리기
		DrawCircle(fm, center.x - exactRadius, center.y - exactRadius, exactRadius, nGray, false);

#if 0
		// 3개의 지점을 지나는 정원 센터 그리기
		DrawCircle(fm, center.x - m_radius, center.y - m_radius, m_radius, 128, true);
#endif
	}
}


bool CMyProjectDlg::ValidImgPos(int x, int y)
{
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	CRect rect(0, 0, nWidth, nHeight);

	return rect.PtInRect(CPoint(x, y));
}


void CMyProjectDlg::RandPos()
{
	// 랜덤하게 벡터 좌표 변경

	// random_device: 운영체제나 하드웨어가 제공하는 난수
	// mt19937: C++11부터 도입된 난수 생성 엔진 [Mersenne Twister 알고리즘 기반]
	std::random_device rd;	// 하드웨어 기반 랜덤 시드
	std::mt19937 gen(rd()); // 고유한 시드 설정
	// 랜덤값 범위 설정
	std::uniform_int_distribution<> width(0, m_image.GetWidth());
	std::uniform_int_distribution<> height(0, m_image.GetHeight());

	// 랜덤하게 벡터 좌표 변경
	m_clickPoints[0].x = width(gen);
	m_clickPoints[0].y = height(gen);
	m_clickPoints[1].x = width(gen);
	m_clickPoints[1].y = height(gen);
	m_clickPoints[2].x = width(gen);
	m_clickPoints[2].y = height(gen);

	// CPoint[int] -> double 형변환
	double x1 = m_clickPoints[0].x, y1 = m_clickPoints[0].y;
	double x2 = m_clickPoints[1].x, y2 = m_clickPoints[1].y;
	double x3 = m_clickPoints[2].x, y3 = m_clickPoints[2].y;

	// 세 점이 일직선인지 확인하는 판별식
	double ret = x1 * (y2 - y3) - y1 * (x2 - x3) + x2 * y3 - x3 * y2;

	// 허용 오차 [1e-2 = 0.01]
	if (fabs(ret) < 5000)
	{
		cout << "RandPos(): 직선에 가까움" << fabs(ret) << endl;
		RandPos();
	}
}


void CMyProjectDlg::OnBnClickedBtnInit()
{
	m_clickPoints.clear();

	m_image.Destroy();

	if (m_image == NULL)
		InitImgBuffer();
	else
		AfxMessageBox(_T("이미지 파괴 에러"));
}


int CMyProjectDlg::ThreadWork()
{
	static int num = 0;

	num++;
	cout << "CMyProjectDlg::ThreadWork(): Start" << endl;

	auto start = system_clock::now();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 여기다 일 시키기
	cout << "CMyProjectDlg::ThreadWork(): Work!!! " << endl;
	
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	///////////////////////////////////////////////////////////////////
	// 지우기
	///////////////////////////////////////////////////////////////////
	SumCircle(255);

	for (int i = 0; i < m_clickPoints.size(); ++i)
	{
		DrawCircle(fm, m_clickPoints[i].x - m_radius, m_clickPoints[i].y - m_radius, m_radius, 255, true); // 클릭지점 그리기
		DrawCircle(fm, m_clickPoints[i].x - m_radius / 4, m_clickPoints[i].y - m_radius / 4, m_radius / 4, 255, true); // 클릭지점 센터 그리기
	}

	///////////////////////////////////////////////////////////////////
	// 다시 그리기
	///////////////////////////////////////////////////////////////////
	RandPos();

	SumCircle(0);

	for (int i = 0; i < m_clickPoints.size(); ++i)
	{
		DrawCircle(fm, m_clickPoints[i].x - m_radius, m_clickPoints[i].y - m_radius, m_radius, 0, true); // 클릭지점 그리기
		DrawCircle(fm, m_clickPoints[i].x - m_radius / 4, m_clickPoints[i].y - m_radius / 4, m_radius / 4, 128, true); // 클릭지점 센터 그리기
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	auto end = system_clock::now();
	auto millisec = duration_cast<milliseconds>(end - start);

	cout << "CMyProjectDlg::ThreadWork(): Time = " << millisec.count() << " ms" << endl;

	cout << "CMyProjectDlg::ThreadWork(): End" << endl;

	return num;
}


LRESULT CMyProjectDlg::OnDrawDone(WPARAM wParam, LPARAM lParam)
{
	// 이 함수는 메인 스레드에서 호출되며, Invalidate()로 화면을 갱신
	// Invalidate()는 GDI/화면을 건드리는 함수
	// MFC에서는 GDI는 메인 스레드(UI 스레드)에서만 안전하게 사용 가능
	Invalidate();

	// LRESULT 반환할 경우 0은 메시지 처리 완료를 뜻 함
	return 0;
}


CEvent g_event(TRUE, FALSE); // 스레드 동기화 객체 [신호 / 리셋]
UINT ThreadEvent(LPVOID pParam)
{
	CMyProjectDlg *pDlg = (CMyProjectDlg*)pParam;
	if (!pDlg)
		return 1; // 실패

	static int cnt = 0;

	g_event.Lock(); // 스레드 잠금
	g_event.ResetEvent(); // 이벤트 비신호 상태
	cnt++;

	cout << "ThreadEvent(): Start: " << cnt << endl;

	pDlg->ThreadWork();

	cout << "ThreadEvent(): End: " << cnt<< endl;

	//Sleep(500);
	this_thread::sleep_for(milliseconds(500));

	g_event.SetEvent(); // 이벤트 신호 상태

	return 0; // 성공
}


void CMyProjectDlg::OnBnClickedBtnRandom()
{
	if (m_clickPoints.size() != CLICKCNT)
	{
		AfxMessageBox(_T("클릭 지점 3개가 없음"));
		return;
	}

	cout << "CMyProjectDlg::OnBnClickedBtnRandom(): Start" << endl;

	UpdateData(true);

	for (int i = 0; i < REPEATCNT; i++)
		AfxBeginThread(ThreadEvent, this); // 스레드 시작

	cout << "CMyProjectDlg::OnBnClickedBtnRandom(): End" << endl;
}


void CMyProjectDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 드래그 시작
	if (m_clickPoints.size() == CLICKCNT)
	{
		UpdateData(true);

		for (int i = 0; i < m_clickPoints.size(); ++i)
		{
			// 벡터에 저장된 좌표에 반지름을 더함
			CRect rc(m_clickPoints[i].x - m_radius, m_clickPoints[i].y - m_radius,
				m_clickPoints[i].x + m_radius, m_clickPoints[i].y + m_radius);

			// 현재 마우스가 벡터에 저장된 좌표안에 있으면
			if (rc.PtInRect(point))
			{
				m_dragIndex = i; // 벡터에 저장된 요소
				
				SetCapture(); // 클라언트 윈도우를 벗어나도 좌표를 받음
			}
		}

		return; // 네 번째 클릭부터는 클릭 지점 원을 그리지 않음
	}

	// 클릭 지점 그리기
	m_clickPoints.push_back(point);

	UpdateData(true);
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	DrawCircle(fm, point.x - m_radius, point.y - m_radius, m_radius, 0, true); // 클릭지점 그리기
	DrawCircle(fm, point.x - m_radius/4, point.y - m_radius/4, m_radius/4, 128, true); // 클릭지점 센터 그리기

	SumCircle(0); // 3점을 지나는 외접원 값 얻기

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMyProjectDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 마우스 이동
	if (m_dragIndex != -1 && (nFlags & MK_LBUTTON))
	{
		UpdateData(true);
		unsigned char* fm = (unsigned char*)m_image.GetBits();

		///////////////////////////////////////////////////////////////////
		// 지우기
		///////////////////////////////////////////////////////////////////
		for (int i = 0; i < m_clickPoints.size(); ++i)
		{
			DrawCircle(fm, m_clickPoints[i].x - m_radius, m_clickPoints[i].y - m_radius, m_radius, 255, true); // 클릭지점 그리기
			DrawCircle(fm, m_clickPoints[i].x - m_radius / 4, m_clickPoints[i].y - m_radius / 4, m_radius / 4, 255, true); // 클릭지점 센터 그리기
		}

		SumCircle(255); // 3점을 지나는 외접원 값 얻기

		///////////////////////////////////////////////////////////////////
		// 다시 그리기
		///////////////////////////////////////////////////////////////////
		// 벡터 좌표 변경
		m_clickPoints[m_dragIndex] = point;

		for (int i = 0; i < m_clickPoints.size(); ++i)
		{
			DrawCircle(fm, m_clickPoints[i].x - m_radius, m_clickPoints[i].y - m_radius, m_radius, 0, true); // 클릭지점 그리기
			DrawCircle(fm, m_clickPoints[i].x - m_radius / 4, m_clickPoints[i].y - m_radius / 4, m_radius / 4, 128, true); // 클릭지점 센터 그리기
		}

		SumCircle(0); // 3점을 지나는 외접원 값 얻기
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CMyProjectDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 드래그 종료
	if (m_dragIndex != -1)
	{
		m_dragIndex = -1;
		ReleaseCapture(); // 좌표 받기 종료
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}


BOOL CMyProjectDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// 화면 깜빡임 개선
	//return CDialogEx::OnEraseBkgnd(pDC);
	return TRUE;
}


HBRUSH CMyProjectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// 스테틱 컨트롤 깜빡임 개선
	int nRet = pWnd->GetDlgCtrlID();
	if (nRet == IDC_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}
