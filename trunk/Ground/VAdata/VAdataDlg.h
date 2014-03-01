// VAdataDlg.h : 头文件
//

#pragma once
#include<fstream>
using namespace std;

const int DET_NUM=9216;

// CProjectDlg 对话框
class CProjectDlg : public CDialog
{
// 构造
public:
	CProjectDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VADATA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//修改VA ID的一系列操作
	int VAID;//VA的ID，从1~72
	CEdit edit_vaid;//VA ID的编辑框
	int GetVAID();//从VA ID的编辑框得到VA ID
	void SetVAID(int);//赋值新的VA ID，刷写数据
	afx_msg void EDIT_vaid();//编辑框编辑
	CSpinButtonCtrl spin_vaid;//VA ID的上下箭头
	afx_msg void SPIN_vaid(NMHDR *pNMHDR, LRESULT *pResult);//点击上下箭头


	//修改DAQM的一系列操作
	int DAQM;//VA的DAQM，从0-11
	CEdit edit_daqm;
	int GetDAQM();
	void SetDAQM(int);
	afx_msg void EDIT_daqm();
	CSpinButtonCtrl spin_daqm;
	afx_msg void SPIN_daqm(NMHDR *pNMHDR, LRESULT *pResult);


	//修改DVA的一系列操作
	int DVA;//VA的DVA，从0-5
	CEdit edit_dva;
	int GetDVA();
	void SetDVA(int);
	CSpinButtonCtrl spin_dva;
	afx_msg void EDIT_dva();
	afx_msg void SPIN_dva(NMHDR *pNMHDR, LRESULT *pResult);


	//编辑框阵列的操作
	afx_msg void BTN_NewVAData();//把新的VA数据赋值给buffer
	CArray<CEdit*,CEdit*> v_edit; //编辑框阵列
	char buffer[DET_NUM];//存储每个通道的数据


	afx_msg void BTN_OpenFile();//打开输入文件
	afx_msg void BTN_GenerateTest();//产生测试数据
	int good;//是否正确读入文件


	afx_msg void BTN_Output();//打开输出文件


	CStatic static_address;


	void ResetAll();//清空
};
