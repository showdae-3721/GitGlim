// MyProjectDlg.cpp : ���� ����

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

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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

// CMyProjectDlg ��ȭ ����
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


// CMyProjectDlg �޽��� ó����
BOOL CMyProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	InitImgBuffer();

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}


BOOL CMyProjectDlg::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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


// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.
void CMyProjectDlg::OnPaint()
{
	CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if (m_image)
			m_image.Draw(dc, 0, 0);

		CDialogEx::OnPaint();
	}
}


// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CMyProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMyProjectDlg::InitImgBuffer()
{
	// �̹��� ����
	int nWidth	= 500;
	int nHeight	= 500;
	int nBpp	= 8;

	m_image.Create(nWidth, -nHeight, nBpp); // �̹��� ����
	if (nBpp == 8) // ����� ��� ���� ���̺� ó�� �ʿ�
	{
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		m_image.SetColorTable(0, 256, rgb);
	}

	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits(); // �̹��� ù��° ��ġ ������ ���

	memset(fm, 0xff, nWidth * nHeight); // �̹��� �޸� �ʱ�ȭ

	Invalidate(); // OnPaint() ȣ��
}


void CMyProjectDlg::DrawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray, bool fill)
{
	int nCenterX = x + nRadius;
	int nCenterY = y + nRadius;
	int nPitch = m_image.GetPitch(); // ���� ���� ���

	for (int j = y; j < y + nRadius * 2; j++) // ��
	{
		for (int i = x; i < x + nRadius * 2; i++) // ��
		{
			// �޸𸮸� ����� ������ ��� ���� ó��
			if (ValidImgPos(i, j))
				if (IsInCircle(i, j, nCenterX, nCenterY, nRadius, fill)) // ���콺 ��ǥ, ���Ͱ�, ������
					fm[j * nPitch + i] = nGray; // ���� �ش�Ǵ� �ȼ����� ����
		}
	}

	// �����忡�� Invalidate() ���� �浹/���۵� �߻�
	// Invalidate();

	// �����忡�� ȭ�� ���� ��û �޽����� ������ ���� �����忡�� ó��
	::PostMessage(GetSafeHwnd(), WM_USER_DRAW_DONE, 0, 0);
}


bool CMyProjectDlg::IsInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius, bool fill)
{
	// ���̶� ���� �߽� ������ �Ÿ��� ���
	// �Ÿ��� ���������� ª���� -> �� �ȿ� �ִ� -> true
	// �Ÿ��� ���������� ��� -> �� �ۿ� �ִ� -> false

	bool bRet = false;

	double dX = i - nCenterX; // ���� ���� �߽� ���� x�� �Ÿ�
	double dY = j - nCenterY; // ���� ���� �߽� ���� y�� �Ÿ�
	double dDist = dX * dX + dY * dY; // �Ÿ��� ������

	if (fill == true)
	{
		// �Ÿ��� ������ �������� �������� �۴ٸ� [�� ���� ���� ���ο� �ִٸ�]
		if (dDist < nRadius * nRadius)
			bRet = true;

		return bRet;
	}
	else
	{
		// �ܰ����� ����
		double outerRadius = nRadius;
		double innerRadius = nRadius - m_thickness; // �ܰ��� �β� ����

		//if (dDist >= (nRadius - 1.5) * (nRadius - 1.5) && dDist <= nRadius * nRadius)
		if (dDist >= innerRadius * innerRadius && dDist <= outerRadius * outerRadius)
			bRet = true;

		return bRet;
	}
}


void CMyProjectDlg::SumCircle(int nGray)
{
	////////////////////////////////////////////////////////////////////
	// 3���� ������ �������� �߽��� ���ϱ�
	////////////////////////////////////////////////////////////////////
	if (m_clickPoints.size() == CLICKCNT)
	{
		// CPoint[int] -> double ����ȯ
		double x1 = m_clickPoints[0].x, y1 = m_clickPoints[0].y;
		double x2 = m_clickPoints[1].x, y2 = m_clickPoints[1].y;
		double x3 = m_clickPoints[2].x, y3 = m_clickPoints[2].y;

		// �� ���� ���������� Ȯ���ϴ� �Ǻ���
		double ret = x1 * (y2 - y3) - y1 * (x2 - x3) + x2 * y3 - x3 * y2;

		// ��� ���� [1e-2 = 0.01]
		if (fabs(ret) < 1500)
		{
			cout << "SumCircle(): ������ �����" << fabs(ret) << endl;
			return;
		}

		double A1 = x1 * x1 + y1 * y1;
		double B1 = x2 * x2 + y2 * y2;
		double C1 = x3 * x3 + y3 * y3;

		double D = 2 * (x1 * (y2 - y3) + x2 *(y3 - y1) + x3 * (y1 - y2));
		double cx = (A1 * (y2 - y3) + B1 * (y3 - y1) + C1 * (y1 - y2)) / D;
		double cy = (A1 * (x3 - x2) + B1 * (x1 - x3) + C1 * (x2 - x1)) / D;

		CPoint center((int)cx, (int)cy);

		// ������ ��� (���� �� A�� �߽� �Ÿ�)
		double dx = x1 - cx;
		double dy = y1 - cy;
		int exactRadius = (int)sqrt(dx * dx + dy * dy);

		unsigned char* fm = (unsigned char*)m_image.GetBits();

		// 3���� ������ ������ ���� �׸���
		DrawCircle(fm, center.x - exactRadius, center.y - exactRadius, exactRadius, nGray, false);

#if 0
		// 3���� ������ ������ ���� ���� �׸���
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
	// �����ϰ� ���� ��ǥ ����

	// random_device: �ü���� �ϵ��� �����ϴ� ����
	// mt19937: C++11���� ���Ե� ���� ���� ���� [Mersenne Twister �˰��� ���]
	std::random_device rd;	// �ϵ���� ��� ���� �õ�
	std::mt19937 gen(rd()); // ������ �õ� ����
	// ������ ���� ����
	std::uniform_int_distribution<> width(0, m_image.GetWidth());
	std::uniform_int_distribution<> height(0, m_image.GetHeight());

	// �����ϰ� ���� ��ǥ ����
	m_clickPoints[0].x = width(gen);
	m_clickPoints[0].y = height(gen);
	m_clickPoints[1].x = width(gen);
	m_clickPoints[1].y = height(gen);
	m_clickPoints[2].x = width(gen);
	m_clickPoints[2].y = height(gen);

	// CPoint[int] -> double ����ȯ
	double x1 = m_clickPoints[0].x, y1 = m_clickPoints[0].y;
	double x2 = m_clickPoints[1].x, y2 = m_clickPoints[1].y;
	double x3 = m_clickPoints[2].x, y3 = m_clickPoints[2].y;

	// �� ���� ���������� Ȯ���ϴ� �Ǻ���
	double ret = x1 * (y2 - y3) - y1 * (x2 - x3) + x2 * y3 - x3 * y2;

	// ��� ���� [1e-2 = 0.01]
	if (fabs(ret) < 5000)
	{
		cout << "RandPos(): ������ �����" << fabs(ret) << endl;
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
		AfxMessageBox(_T("�̹��� �ı� ����"));
}


int CMyProjectDlg::ThreadWork()
{
	static int num = 0;

	num++;
	cout << "CMyProjectDlg::ThreadWork(): Start" << endl;

	auto start = system_clock::now();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����� �� ��Ű��
	cout << "CMyProjectDlg::ThreadWork(): Work!!! " << endl;
	
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	///////////////////////////////////////////////////////////////////
	// �����
	///////////////////////////////////////////////////////////////////
	SumCircle(255);

	for (int i = 0; i < m_clickPoints.size(); ++i)
	{
		DrawCircle(fm, m_clickPoints[i].x - m_radius, m_clickPoints[i].y - m_radius, m_radius, 255, true); // Ŭ������ �׸���
		DrawCircle(fm, m_clickPoints[i].x - m_radius / 4, m_clickPoints[i].y - m_radius / 4, m_radius / 4, 255, true); // Ŭ������ ���� �׸���
	}

	///////////////////////////////////////////////////////////////////
	// �ٽ� �׸���
	///////////////////////////////////////////////////////////////////
	RandPos();

	SumCircle(0);

	for (int i = 0; i < m_clickPoints.size(); ++i)
	{
		DrawCircle(fm, m_clickPoints[i].x - m_radius, m_clickPoints[i].y - m_radius, m_radius, 0, true); // Ŭ������ �׸���
		DrawCircle(fm, m_clickPoints[i].x - m_radius / 4, m_clickPoints[i].y - m_radius / 4, m_radius / 4, 128, true); // Ŭ������ ���� �׸���
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
	// �� �Լ��� ���� �����忡�� ȣ��Ǹ�, Invalidate()�� ȭ���� ����
	// Invalidate()�� GDI/ȭ���� �ǵ帮�� �Լ�
	// MFC������ GDI�� ���� ������(UI ������)������ �����ϰ� ��� ����
	Invalidate();

	// LRESULT ��ȯ�� ��� 0�� �޽��� ó�� �ϷḦ �� ��
	return 0;
}


CEvent g_event(TRUE, FALSE); // ������ ����ȭ ��ü [��ȣ / ����]
UINT ThreadEvent(LPVOID pParam)
{
	CMyProjectDlg *pDlg = (CMyProjectDlg*)pParam;
	if (!pDlg)
		return 1; // ����

	static int cnt = 0;

	g_event.Lock(); // ������ ���
	g_event.ResetEvent(); // �̺�Ʈ ���ȣ ����
	cnt++;

	cout << "ThreadEvent(): Start: " << cnt << endl;

	pDlg->ThreadWork();

	cout << "ThreadEvent(): End: " << cnt<< endl;

	//Sleep(500);
	this_thread::sleep_for(milliseconds(500));

	g_event.SetEvent(); // �̺�Ʈ ��ȣ ����

	return 0; // ����
}


void CMyProjectDlg::OnBnClickedBtnRandom()
{
	if (m_clickPoints.size() != CLICKCNT)
	{
		AfxMessageBox(_T("Ŭ�� ���� 3���� ����"));
		return;
	}

	cout << "CMyProjectDlg::OnBnClickedBtnRandom(): Start" << endl;

	UpdateData(true);

	for (int i = 0; i < REPEATCNT; i++)
		AfxBeginThread(ThreadEvent, this); // ������ ����

	cout << "CMyProjectDlg::OnBnClickedBtnRandom(): End" << endl;
}


void CMyProjectDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	// �巡�� ����
	if (m_clickPoints.size() == CLICKCNT)
	{
		UpdateData(true);

		for (int i = 0; i < m_clickPoints.size(); ++i)
		{
			// ���Ϳ� ����� ��ǥ�� �������� ����
			CRect rc(m_clickPoints[i].x - m_radius, m_clickPoints[i].y - m_radius,
				m_clickPoints[i].x + m_radius, m_clickPoints[i].y + m_radius);

			// ���� ���콺�� ���Ϳ� ����� ��ǥ�ȿ� ������
			if (rc.PtInRect(point))
			{
				m_dragIndex = i; // ���Ϳ� ����� ���
				
				SetCapture(); // Ŭ���Ʈ �����츦 ����� ��ǥ�� ����
			}
		}

		return; // �� ��° Ŭ�����ʹ� Ŭ�� ���� ���� �׸��� ����
	}

	// Ŭ�� ���� �׸���
	m_clickPoints.push_back(point);

	UpdateData(true);
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	DrawCircle(fm, point.x - m_radius, point.y - m_radius, m_radius, 0, true); // Ŭ������ �׸���
	DrawCircle(fm, point.x - m_radius/4, point.y - m_radius/4, m_radius/4, 128, true); // Ŭ������ ���� �׸���

	SumCircle(0); // 3���� ������ ������ �� ���

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMyProjectDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	// ���콺 �̵�
	if (m_dragIndex != -1 && (nFlags & MK_LBUTTON))
	{
		UpdateData(true);
		unsigned char* fm = (unsigned char*)m_image.GetBits();

		///////////////////////////////////////////////////////////////////
		// �����
		///////////////////////////////////////////////////////////////////
		for (int i = 0; i < m_clickPoints.size(); ++i)
		{
			DrawCircle(fm, m_clickPoints[i].x - m_radius, m_clickPoints[i].y - m_radius, m_radius, 255, true); // Ŭ������ �׸���
			DrawCircle(fm, m_clickPoints[i].x - m_radius / 4, m_clickPoints[i].y - m_radius / 4, m_radius / 4, 255, true); // Ŭ������ ���� �׸���
		}

		SumCircle(255); // 3���� ������ ������ �� ���

		///////////////////////////////////////////////////////////////////
		// �ٽ� �׸���
		///////////////////////////////////////////////////////////////////
		// ���� ��ǥ ����
		m_clickPoints[m_dragIndex] = point;

		for (int i = 0; i < m_clickPoints.size(); ++i)
		{
			DrawCircle(fm, m_clickPoints[i].x - m_radius, m_clickPoints[i].y - m_radius, m_radius, 0, true); // Ŭ������ �׸���
			DrawCircle(fm, m_clickPoints[i].x - m_radius / 4, m_clickPoints[i].y - m_radius / 4, m_radius / 4, 128, true); // Ŭ������ ���� �׸���
		}

		SumCircle(0); // 3���� ������ ������ �� ���
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CMyProjectDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	// �巡�� ����
	if (m_dragIndex != -1)
	{
		m_dragIndex = -1;
		ReleaseCapture(); // ��ǥ �ޱ� ����
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}


BOOL CMyProjectDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	// ȭ�� ������ ����
	//return CDialogEx::OnEraseBkgnd(pDC);
	return TRUE;
}


HBRUSH CMyProjectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

	// ����ƽ ��Ʈ�� ������ ����
	int nRet = pWnd->GetDlgCtrlID();
	if (nRet == IDC_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}
