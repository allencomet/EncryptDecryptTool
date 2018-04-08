
// EncryptDecryptDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <string>
#include "./openssl/md5.h"
#include "blowfish.h"


#pragma pack(push, 1)
typedef struct tagEncryptDecryptFile
{
	char	m_szTag[2];			//填充标志，默认为TK
	UINT	m_nOriginDataLen;	//填充前数据长度
}EncryptDecryptFile;
#pragma pack(pop)


// CEncryptDecryptDlg 对话框
class CEncryptDecryptDlg : public CDialogEx
{
// 构造
public:
	CEncryptDecryptDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CEncryptDecryptDlg();

// 对话框数据
	enum { IDD = IDD_ENCRYPTDECRYPT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpen();
	
	afx_msg void OnDropFiles(HDROP hDropInfo);
	
	afx_msg void OnBnClickedBtnSavepath();
	virtual BOOL PreTranslateMessage(MSG* pMsg);



	std::string GetDataMD5(char *pCertData,int nCertLen);


	//=================================AES算法加解密文件==========================
	//加密子函数开始 
	bool	AESEnCodeFile(const char *in_filename,const char *pwd,const char *out_file);
	//解密子函数开始
	bool	AESDeCodeFile(const char *in_filename,const char *pwd,const char *out_file);


	//=================================AES算法加解密函数==============================
	//使用AES 加密Key对数据进行加密
	bool SocketDataEncrypt(unsigned char *pOutData,AES_KEY *pAESEncrypt,unsigned char *pInData,int nOutDataLen,int nLen = 16);
	//使用AES 解密Key对数据进行解密
	bool SocketDataDecrypt(unsigned char *pOutData,AES_KEY *pAESDecrypt,unsigned char *pInData,int nOutDataLen,int nLen = 16);



	//=================================blowfish算法加解密文件==========================
	//blowfish算法加解密文件
	bool BlowFishEnCodeFile(const char *in_filename,const char *out_file);
	//blowfish算法加解密文件
	bool BlowFishDeCodeFile(const char *in_filename,const char *out_file);


	CListBox m_listAcceptFile;
	CString m_cstrSourceFile;
	afx_msg void OnBnClickedBtnEncrypt();
	afx_msg void OnLbnSelchangeListAcceptfile();
	afx_msg void OnLbnDblclkListAcceptfile();
	afx_msg void OnBnClickedBtnClearfile();
	afx_msg void OnBnClickedBtnDecrypt();

	std::string GetCipherKey(char *pKey);
private:
	CBlowFish *m_pBlowFish;					//数据加解密对象
	int			m_nEnDeType;				//加解密算法类型
public:
	CComboBox m_comboEnDeType;
	afx_msg void OnCbnSelchangeComboEndetype();
};
