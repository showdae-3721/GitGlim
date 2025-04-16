// MyProjectDlg.h : ��� ����

/*
�䱸����
- �Ʒ� �׸��� ������� �����Ͽ� �䱸���׵��� ��� �����ϴ� ���α׷��� �ۼ��մϴ�.
- Ŭ�� ���� ���� �׸� ���� ������ ũ��� ����ڷκ��� �Է� �޽��ϴ�. - ok
- �� ��° Ŭ�� ���Ŀ� Ŭ�� ���� 3���� ��� �������� ���� 1���� �׸��ϴ�. - ok
- �� Ŭ�� ������ �������� ������ ���δ� ä������ �ʾƾ� �ϸ�, �����ڸ� �β��� ����ڷκ��� �Է� �޽��ϴ�. - ok
- �� ��° Ŭ�����ʹ� Ŭ�� ���� ���� �׸��� �ʽ��ϴ�. - ok
- [�ʱ�ȭ] ��ư�� ������ �׷����� ��� ������� �����ϰ� ó������ �Է� ���� �� �ִ� ���°� �Ǿ�� �մϴ�. - ok
- �� Ŭ�� ���� ���� �߽� ��ǥ�� UI�� ǥ���մϴ�. - ok

- Ŭ�� ���� 3�� �� �ϳ��� Ŭ���ϰ� �巡�� ���� �� ������ �ٽ� �׸��ϴ�. - ok
- �� ��, ���콺 Ŀ�� ��ǥ�� �ٲ�� ���� ��, ���콺 �巡�� ���°� ���� �� ���� ������ ����ؼ� �̵��ϸ� �׷����� �մϴ�. - ing [���� �ӵ�?]

- ������ �׷��� ���¿��� [���� �̵�] ��ư�� ������ 3���� Ŭ�� ���� �� ��θ� ������ ��ġ�� �̵���ŵ�ϴ�. - ok
- �� ��, ���� ���� ���������� �ٽ� �׷����� �մϴ�. - ok
- ������ ��ġ�� �̵� �� ���� �׸��� ������ �ʴ� 2ȸ, �� 10�� �ڵ����� �ݺ��ϵ� ����UI�� ����¡ ���°� ���� �ʵ��� ���� ������� �����ؾ� �մϴ�. - ok

- MFC Dialog ��� ������Ʈ�� �ۼ��ؾ� �մϴ�.
- Ŭ�� ���� ���� ������ �׸� �� CDC Ŭ������ ����ϸ� �ȵ˴ϴ�. (�ȳ��� ��Ʃ�� ���� ����)
*/

#pragma once

#include <vector>

using namespace std;

#define CLICKCNT 3
#define REPEATCNT 10
#define WM_USER_DRAW_DONE (WM_USER + 1) // ����� ���� �޽���

// CMyProjectDlg ��ȭ ����
class CMyProjectDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMyProjectDlg(CWnd* pParent = NULL); // ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYPROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CImage	m_image;				// �̹��� ���� Ŭ����
	int		m_radius;				// ������
	int		m_thickness;			// �����ڸ� �β�
	int		m_dragIndex;			// �巡�� ���� �ε���
	vector<CPoint> m_clickPoints;	// Ŭ�� ����Ʈ ����

public:
	void	InitImgBuffer();
	void	DrawCircle(unsigned char* fm, int i, int j, int nRadius, int nGray, bool fill);
	bool	IsInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius, bool fill);
	void	SumCircle(int nGray);
	bool	ValidImgPos(int x, int y);
	void	RandPos();
	int		ThreadWork();
	LRESULT OnDrawDone(WPARAM wParam, LPARAM lParam); // ���������� �ñ״�ó�� �ݵ�� ����� �۵�

public:
	afx_msg void	OnBnClickedBtnInit();
	afx_msg void	OnBnClickedBtnRandom();
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
