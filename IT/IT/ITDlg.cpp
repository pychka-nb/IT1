// ITDlg.cpp: файл реализации
#include "pch.h"
#include "framework.h"
#include "IT.h"
#include "ITDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define Pi 3.1415926535

// Диалоговое окно CITDlg

CITDlg::CITDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IT_DIALOG, pParent),
	A1(1),
	A2(2),
	A3(3),
	f1(0.01),
	f2(0.03),
	f3(0.05),
	phi1(0),
	phi2(0),
	phi3(0),
	N(1024),
	fd(1),
	alpha(20),
	betta(80),
	Ediff(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CITDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, A1);
	DDX_Text(pDX, IDC_EDIT2, A2);
	DDX_Text(pDX, IDC_EDIT3, A3);
	DDX_Text(pDX, IDC_EDIT4, f1);
	DDX_Text(pDX, IDC_EDIT5, f2);
	DDX_Text(pDX, IDC_EDIT6, f3);
	DDX_Text(pDX, IDC_EDIT7, phi1);
	DDX_Text(pDX, IDC_EDIT8, phi2);
	DDX_Text(pDX, IDC_EDIT9, phi3);
	DDX_Text(pDX, IDC_EDIT10, N);
	DDX_Text(pDX, IDC_EDIT11, fd);

	DDX_Text(pDX, IDC_EDIT12, alpha);
	DDX_Text(pDX, IDC_EDIT13, betta);
	DDX_Text(pDX, IDC_EDIT14, Ediff);
}

BEGIN_MESSAGE_MAP(CITDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Create_Signal, &CITDlg::OnBnClickedCreateSignal)
END_MESSAGE_MAP()


// Обработчики сообщений CITDlg

BOOL CITDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	graph1.Create(GetDlgItem(IDC_Signal)->GetSafeHwnd());
	graph2.Create(GetDlgItem(IDC_Spectrum)->GetSafeHwnd());
	graph3.Create(GetDlgItem(IDC_Recovered_Signal)->GetSafeHwnd());
	graph4.Create(GetDlgItem(IDC_SignalNoise)->GetSafeHwnd());

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CITDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CITDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/*Ф-ция, вычисляющая предел графика*/
double CITDlg::FindMin(vector <double> data, int N)
{
	double min = data[0];
	for (int i = 0; i < N; i++)
	{
		if (min > data[i])
			min = data[i];
	}
	return min;
}
/*Ф-ция, вычисляющая предел графика*/
double CITDlg::FindMax(vector <double> data, int N)
{
	double max = data[0];
	for (int i = 0; i < N; i++)
	{
		if (max < data[i])
			max = data[i];
	}
	return max;
}
/*Ф-ция, создающая полигармонический сигнал*/
void CITDlg::CreateSignal()
{
	for (int i = 0; i < N; i++)
	{
		Sign.push_back(
			A1 * sin(2 * Pi * f1 * (i / fd) + phi1) + A2 * sin(2 * Pi * f2 * (i / fd) + phi2) + A3 * sin(2 * Pi * f3 * (i / fd) + phi3)
		);
	}
}
/*Алгоритм БПФ*/
void fourea(struct cmplx* data, int n, int is)
{
	int i, j, istep;
	int m, mmax;
	float r, r1, theta, w_r, w_i, temp_r, temp_i;
	float pi = 3.1415926f;

	r = pi * is;
	j = 0;
	for (i = 0; i < n; i++)
	{
		if (i < j)
		{
			temp_r = data[j].real;
			temp_i = data[j].image;
			data[j].real = data[i].real;
			data[j].image = data[i].image;
			data[i].real = temp_r;
			data[i].image = temp_i;
		}
		m = n >> 1;
		while (j >= m) { j -= m; m = (m + 1) / 2; }
		j += m;
	}
	mmax = 1;
	while (mmax < n)
	{
		istep = mmax << 1;
		r1 = r / (float)mmax;
		for (m = 0; m < mmax; m++)
		{
			theta = r1 * m;
			w_r = (float)cos((double)theta);
			w_i = (float)sin((double)theta);
			for (i = m; i < n; i += istep)
			{
				j = i + mmax;
				temp_r = w_r * data[j].real - w_i * data[j].image;
				temp_i = w_r * data[j].image + w_i * data[j].real;
				data[j].real = data[i].real - temp_r;
				data[j].image = data[i].image - temp_i;
				data[i].real += temp_r;
				data[i].image += temp_i;
			}
		}
		mmax = istep;
	}
	if (is > 0)
		for (i = 0; i < n; i++)
		{
			data[i].real /= (float)n;
			data[i].image /= (float)n;
		}

}
/*Ф-ция, генерирующая рандомное число*/
double Random()
{
	double a = 0;
	for (int i = 0; i < 12; i++)
		a += -1. + 2 * (double)rand() / RAND_MAX;
	return a;
}
/*Ф-ция, считающая сумму квадратов отсчётов сигнала*/
double Sum(vector <double> mas)
{
	double a = 0;
	for (int i = 0; i < mas.size(); i++)
		a += mas[i] * mas[i];
	return a;
}
/*Ф-ция, создающая полигармонический сигнал с наложенным белым гауссовым шумом*/
double CITDlg::Noise()
{
	vector <double> n0;
	double Es;
	double En;
	double B;
	double Al = alpha / 100;
	for (int i = 0; i < N; i++)
	{
		n0.push_back(Random());
	}
	Es = Sum(Sign);
	En = Sum(n0);
	B = sqrt(Al * Es / En);
	for (int i = 0; i < N; i++)
	{
		SignN.push_back(Sign[i] + B * n0[i]);
	}
	return 0;
}
/*Ф-ция фильтрации спектра*/
void CITDlg::Clear(double* re, double* im)
{
	double f = 0;
	double E = 0;
	double Esum = betta * Sum(AmplN) / 100;
	while (E < Esum)
	{
		E += AmplN[f] * AmplN[f];
		E += AmplN[N - f - 1] * AmplN[N - f - 1];
		f++;
	}
	for (int i = 0; i < N; i++)
	{
		AmplC.push_back(AmplN[i]);
		if ((i > f) && (i < N - f))
		{
			re[i] = 0;
			im[i] = 0;
			AmplC[i] = 0;
		}
	}
}
/*Ф-ция, строящая графики при нажатии на кнопку*/
void CITDlg::OnBnClickedCreateSignal()
{
	srand(time(0));
	graph1.DrawW();
	graph2.DrawW();
	graph3.DrawW();
	graph4.DrawW();
	UpdateData(true);
	double
		* datare = new double[N],
		* dataim = new double[N]
		;

	struct cmplx* data = new struct cmplx[N];
	CreateSignal();
	for (int i = 0; i < N; i++)
	{
		keys.push_back(i / fd);
		data[i].real = Sign[i];
		data[i].image = 0.;
	}
	double xmax = (N - 1) / fd,
		ymax = FindMax(Sign, N),
		ymin = FindMin(Sign, N);
	graph1.DrawOne(Sign, 0, xmax, ymin, ymax, keys);

	fourea(data, N, 1);
	for (int i = 0; i < N; i++)
	{
		Ampl.push_back(sqrt(data[i].real* data[i].real+ data[i].image* data[i].image));
		keys1.push_back(fd * i / N);
	}
	//graph2.DrawOne(Ampl, 0, fd, 0, ymax, keys1); спектр исходного сигнала
	fourea(data, N, -1);
	Noise();
	for (int i = 0; i < N; i++)
	{
		data[i].real = SignN[i];
	}
	ymax = FindMax(SignN, N);
	ymin = FindMin(SignN, N);

	fourea(data,  N,1);
	for (int i = 0; i < N; i++)
	{
		AmplN.push_back(sqrt(data[i].real * data[i].real + data[i].image * data[i].image));
	}
	
	for (int i = 0; i < N; i++)
	{
		datare[i] = data[i].real;
		dataim[i] = data[i].image;
	}
	Clear(datare, dataim);
	for (int i = 0; i < N; i++)
	{
		data[i].real=datare[i] ;
		data[i].image=dataim[i];
	}
	fourea(data, N, -1);
	for (int i = 0; i < N; i++)
	{
		SignC.push_back(data[i].real);
		Diff.push_back(Sign[i] - SignC[i]);
	}
	Ediff = Sum(Diff);
	double ymaxA = FindMax(Ampl, N);
	UpdateData(false);

	graph2.DrawTwo(AmplN, AmplC, 0, fd, 0, ymaxA, keys1, keys1);
	graph4.DrawOneBlue(SignN, 0, xmax, ymin, ymax, keys); //SignNoise

	ymax = FindMax(Sign, N),
		ymin = FindMin(Sign, N);
	graph3.DrawTwoRecovered(Sign, SignC, 0, xmax, ymin, ymax, keys, keys);
	
	Sign.clear();
	SignN.clear();
	keys.clear();
	keys1.clear();
	Ampl.clear();
	AmplN.clear();
	SignC.clear();
	AmplC.clear();
	Diff.clear();
}

