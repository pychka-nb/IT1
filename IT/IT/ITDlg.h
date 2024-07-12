
// ITDlg.h: файл заголовка
//

#pragma once
#include <vector>
#include <iostream>
#include <time.h>
#include "Drawer.h"
#include <math.h>
typedef struct cmplx { float real; float image; } ;

// Диалоговое окно CITDlg
class CITDlg : public CDialogEx
{
// Создание
public:
	CITDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedCreateSignal();
	DECLARE_MESSAGE_MAP()
public:
	double A1;
	double A2;
	double A3;

	double f1;
	double f2;
	double f3;

	double phi1;
	double phi2;
	double phi3;

	double N;
	double fd;

	vector <double> Sign;
	vector <double> keys;
	vector <double> keys1;
	vector <double> SignN;
	vector <double>	SignC;
	vector <double> Ampl;
	vector <double>	AmplN;
	vector <double>	AmplC;
	vector <double>	Diff;

	void CreateSignal();
	double Noise();
	double FindMin(vector<double> data, int N);
	double FindMax(vector<double> data, int N);
	double Fur(vector<double> in, vector<double> out, int N);
	void Clear(double* re, double* im);

	Drawer
		graph1,
		graph2,
		graph3,
		graph4;
	double ymin, ymax;
	double alpha;
	double betta;
	double Ediff;

};
