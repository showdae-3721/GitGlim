// MyProjectDlg.h : 헤더 파일

/*
요구사항
- 아래 그림의 순서대로 조작하여 요구사항들을 모두 만족하는 프로그램을 작성합니다.
- 클릭 지점 원을 그릴 때의 반지름 크기는 사용자로부터 입력 받습니다. - ok
- 세 번째 클릭 이후에 클릭 지점 3개를 모두 지나가는 정원 1개를 그립니다. - ok
- 세 클릭 지점을 지나가는 정원의 내부는 채워지지 않아야 하며, 가장자리 두께는 사용자로부터 입력 받습니다. - ok
- 네 번째 클릭부터는 클릭 지점 원을 그리지 않습니다. - ok
- [초기화] 버튼을 누르면 그려졌던 모든 내용들을 삭제하고 처음부터 입력 받을 수 있는 상태가 되어야 합니다. - ok
- 각 클릭 지점 원의 중심 좌표를 UI에 표시합니다. - ok

- 클릭 지점 3개 중 하나를 클릭하고 드래그 했을 때 정원을 다시 그립니다. - ok
- 이 때, 마우스 커서 좌표가 바뀌는 동안 즉, 마우스 드래그 상태가 끝날 때 까지 정원이 계속해서 이동하며 그려져야 합니다. - ing [갱신 속도?]

- 정원이 그려진 상태에서 [랜덤 이동] 버튼을 누르면 3개의 클릭 지점 원 모두를 랜덤한 위치로 이동시킵니다. - ok
- 이 때, 정원 또한 마찬가지로 다시 그려져야 합니다. - ok
- 랜덤한 위치로 이동 및 정원 그리기 동작을 초당 2회, 총 10번 자동으로 반복하되 메인UI가 프리징 상태가 되지 않도록 별도 쓰레드로 구현해야 합니다. - ok

- MFC Dialog 기반 프로젝트로 작성해야 합니다.
- 클릭 지점 원과 정원을 그릴 때 CDC 클래스를 사용하면 안됩니다. (안내한 유튜브 영상 참고)
*/

#pragma once

#include <vector>

using namespace std;

#define CLICKCNT 3
#define REPEATCNT 10
#define WM_USER_DRAW_DONE (WM_USER + 1) // 사용자 정의 메시지

// CMyProjectDlg 대화 상자
class CMyProjectDlg : public CDialogEx
{
// 생성입니다.
public:
	CMyProjectDlg(CWnd* pParent = NULL); // 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYPROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CImage	m_image;				// 이미지 버퍼 클래스
	int		m_radius;				// 반지름
	int		m_thickness;			// 가장자리 두께
	int		m_dragIndex;			// 드래그 중인 인덱스
	vector<CPoint> m_clickPoints;	// 클릭 포인트 저장

public:
	void	InitImgBuffer();
	void	DrawCircle(unsigned char* fm, int i, int j, int nRadius, int nGray, bool fill);
	bool	IsInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius, bool fill);
	void	SumCircle(int nGray);
	bool	ValidImgPos(int x, int y);
	void	RandPos();
	int		ThreadWork();
	LRESULT OnDrawDone(WPARAM wParam, LPARAM lParam); // 내부적으로 시그니처를 반드시 따라야 작동

public:
	afx_msg void	OnBnClickedBtnInit();
	afx_msg void	OnBnClickedBtnRandom();
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
