
// EncryptDecryptDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <string>
#include "./openssl/md5.h"
#include "blowfish.h"


#pragma pack(push, 1)
typedef struct tagEncryptDecryptFile
{
	char	m_szTag[2];			//����־��Ĭ��ΪTK
	UINT	m_nOriginDataLen;	//���ǰ���ݳ���
}EncryptDecryptFile;
#pragma pack(pop)


// CEncryptDecryptDlg �Ի���
class CEncryptDecryptDlg : public CDialogEx
{
// ����
public:
	CEncryptDecryptDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CEncryptDecryptDlg();

// �Ի�������
	enum { IDD = IDD_ENCRYPTDECRYPT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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


	//=================================AES�㷨�ӽ����ļ�==========================
	//�����Ӻ�����ʼ 
	bool	AESEnCodeFile(const char *in_filename,const char *pwd,const char *out_file);
	//�����Ӻ�����ʼ
	bool	AESDeCodeFile(const char *in_filename,const char *pwd,const char *out_file);


	//=================================AES�㷨�ӽ��ܺ���==============================
	//ʹ��AES ����Key�����ݽ��м���
	bool SocketDataEncrypt(unsigned char *pOutData,AES_KEY *pAESEncrypt,unsigned char *pInData,int nOutDataLen,int nLen = 16);
	//ʹ��AES ����Key�����ݽ��н���
	bool SocketDataDecrypt(unsigned char *pOutData,AES_KEY *pAESDecrypt,unsigned char *pInData,int nOutDataLen,int nLen = 16);



	//=================================blowfish�㷨�ӽ����ļ�==========================
	//blowfish�㷨�ӽ����ļ�
	bool BlowFishEnCodeFile(const char *in_filename,const char *out_file);
	//blowfish�㷨�ӽ����ļ�
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
	CBlowFish *m_pBlowFish;					//���ݼӽ��ܶ���
	int			m_nEnDeType;				//�ӽ����㷨����
public:
	CComboBox m_comboEnDeType;
	afx_msg void OnCbnSelchangeComboEndetype();
};
