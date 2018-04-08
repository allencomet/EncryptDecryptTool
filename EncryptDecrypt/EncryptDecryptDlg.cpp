
// EncryptDecryptDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EncryptDecrypt.h"
#include "EncryptDecryptDlg.h"
#include "afxdialogex.h"
#include <string>
#include "md5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEncryptDecryptDlg �Ի���



CEncryptDecryptDlg::CEncryptDecryptDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEncryptDecryptDlg::IDD, pParent)
	, m_cstrSourceFile(_T(""))
{
	m_pBlowFish = NULL;		//���ݼӽ��ܶ���
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CEncryptDecryptDlg::~CEncryptDecryptDlg()
{
	if (NULL != m_pBlowFish)
	{
		delete m_pBlowFish;
		m_pBlowFish= NULL;
	}
}

void CEncryptDecryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_EDIT_SOURCEFILE, m_cstrSourceFile);
	DDX_Control(pDX, IDC_LIST_ACCEPTFILE, m_listAcceptFile);
	DDX_Control(pDX, IDC_COMBO_ENDETYPE, m_comboEnDeType);
}

BEGIN_MESSAGE_MAP(CEncryptDecryptDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN, &CEncryptDecryptDlg::OnBnClickedBtnOpen)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BTN_SAVEPATH, &CEncryptDecryptDlg::OnBnClickedBtnSavepath)
	ON_BN_CLICKED(IDC_BTN_ENCRYPT, &CEncryptDecryptDlg::OnBnClickedBtnEncrypt)
	ON_LBN_SELCHANGE(IDC_LIST_ACCEPTFILE, &CEncryptDecryptDlg::OnLbnSelchangeListAcceptfile)
	ON_LBN_DBLCLK(IDC_LIST_ACCEPTFILE, &CEncryptDecryptDlg::OnLbnDblclkListAcceptfile)
	ON_BN_CLICKED(IDC_BTN_CLEARFILE, &CEncryptDecryptDlg::OnBnClickedBtnClearfile)
	ON_BN_CLICKED(IDC_BTN_DECRYPT, &CEncryptDecryptDlg::OnBnClickedBtnDecrypt)
	ON_CBN_SELCHANGE(IDC_COMBO_ENDETYPE, &CEncryptDecryptDlg::OnCbnSelchangeComboEndetype)
END_MESSAGE_MAP()


// CEncryptDecryptDlg ��Ϣ�������

BOOL CEncryptDecryptDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_nEnDeType = 0;
	m_comboEnDeType.InsertString(0,_T("AES�㷨�ӽ���"));
	m_comboEnDeType.InsertString(1,_T("BlowFish�㷨�ӽ���"));
	m_comboEnDeType.SetCurSel(0);

	//��ʼ�������û���Ϣ�ӽ��ܶ���
	std::string strEncKey = "216E0D565E294060";
	std::string strCommunicateKey = GetCipherKey((char*)strEncKey.c_str());
	m_pBlowFish = new CBlowFish((unsigned char*)strCommunicateKey.c_str(),strCommunicateKey.length());


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CEncryptDecryptDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CEncryptDecryptDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CEncryptDecryptDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CEncryptDecryptDlg::OnBnClickedBtnOpen()
{
	char strExt[_MAX_EXT] = "tif";//���ö�ȡ���ļ�����  
	CString szFilter = "Encrypt Files (*.Enc)|*.Enc|Decrypt Files (*.Dec)|*.Dec|Cert Files (*.cer)|*.cer|Pem Files (*.key)|*.key|All Files (*.*)|*.*||";  
	CFileDialog   dlg( TRUE, strExt,NULL,OFN_FILEMUSTEXIST | OFN_HIDEREADONLY| OFN_ALLOWMULTISELECT,szFilter );  
	dlg.m_ofn.lpstrTitle = _T("��ѡ����Ҫ������ļ�");//���ñ���  
	CString inPath;  
	CArray<CString, CString> aryFilename;  
	if(dlg.DoModal() == IDOK)  
	{  
		POSITION posFile=dlg.GetStartPosition();  
		while(posFile!=NULL)  
		{  
			aryFilename.Add(dlg.GetNextPathName(posFile));  
		}  
	}  
	int SelFileNum = aryFilename.GetSize();//��ȡѡ����ļ���  
	if(SelFileNum==0)  
	{  
		AfxMessageBox("û��ѡ����Ҫ������ļ���");  
		return ;  
	}  

	int nIndex = 0;
	for (; nIndex < SelFileNum; ++nIndex)
	{
		m_listAcceptFile.AddString(aryFilename.GetAt(nIndex)); 
	}
	UpdateData();
	
	//CFileDialog dlgFile(TRUE,0,0,OFN_FILEMUSTEXIST,0,0,0);		//�ļ�ѡ��
	//if(dlgFile.DoModal()==IDOK)
	//{
	//	m_cstrSourceFile = dlgFile.GetPathName();		//��ȡѡ�е��ļ���·����	
	//}else{
	//	m_cstrSourceFile = "��ѡ��";
	//}

	//UpdateData(FALSE);
	//::SetDlgItemText(m_hWnd,IDC_EDIT_SOURCEFILE,m_cstrSourceFile);
}


void CEncryptDecryptDlg::OnDropFiles(HDROP hDropInfo)
{
	int DropCount=DragQueryFile(hDropInfo,-1,NULL,0);//ȡ�ñ��϶��ļ�����Ŀ  
	for(int i=0;i< DropCount;i++)  
	{  
		WCHAR wcStr[MAX_PATH];  
		DragQueryFile(hDropInfo,i,(LPSTR)wcStr,MAX_PATH);//�����ҷ�ĵ�i���ļ����ļ���  
		m_listAcceptFile.AddString((LPSTR)wcStr);  
	}   
	DragFinish(hDropInfo);  //�ϷŽ�����,�ͷ��ڴ�

	CDialogEx::OnDropFiles(hDropInfo);
}


//ѡ�񱣴�Ŀ¼
void CEncryptDecryptDlg::OnBnClickedBtnSavepath()
{
	char szPath[MAX_PATH];     //���ѡ���Ŀ¼·�� 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = "��ѡ����Ҫ�����Ŀ¼��";   
	bi.ulFlags = 0;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//����ѡ��Ŀ¼�Ի���
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		str.Format("ѡ���Ŀ¼Ϊ %s",  szPath);
		//AfxMessageBox(str); 
		SetDlgItemText(IDC_EDIT_SAVEPATH,szPath);

	}
	else   
	{
		SetDlgItemText(IDC_EDIT_SAVEPATH,"��Ч��Ŀ¼��������ѡ��");
		//AfxMessageBox("��Ч��Ŀ¼��������ѡ��");   
	}
		
}


BOOL CEncryptDecryptDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)  
	{  
		BOOL bCtrl = ::GetKeyState(VK_CONTROL) & 0x80;  
		//BOOL bShift = ::GetKeyState(VK_SHIFT) & 0x80;  
		//BOOL bAlt = ::GetKeyState(VK_MENU) & 0x80;  

		switch (pMsg->wParam)  
		{  
		case 'a':  
		case 'A':  
			if (bCtrl)  
			{  
				if (GetFocus() == GetDlgItem(IDC_EDIT_SAVEPATH))
				{
					SendDlgItemMessage(IDC_EDIT_SAVEPATH, EM_SETSEL, 0, -1);
				}
				else if (GetFocus() == GetDlgItem(IDC_EDIT_MD5))
				{
					SendDlgItemMessage(IDC_EDIT_MD5, EM_SETSEL, 0, -1);
				}
			}  
			break;  
		}  
	}  

	return CDialogEx::PreTranslateMessage(pMsg);
}



/************************************************************************/
/* 
	����˵������������
	����˵����
		in_filename���������ļ���
		pwd�������ļ�����
		out_file�����ܺ��ļ�����
*/
/************************************************************************/
bool  CEncryptDecryptDlg::AESEnCodeFile(const char *in_filename,const char *pwd,const char *out_file)   
{   
	if (NULL == in_filename || NULL == pwd || NULL == out_file)
	{
		return false;
	}

	AES_KEY *pAESEncryptKey = NULL;	//AES����KEY
	pAESEncryptKey = new AES_KEY;
	//���ûỰKEY������AES��Կ�Բ�����
	AES_set_encrypt_key((const unsigned char *)pwd,256,pAESEncryptKey);	//����Key


	FILE *fpSrc, *fpDst;   
		
	fpSrc = fopen(in_filename,"rb");
	char	szTmp[256] = {0};


	if(fpSrc == NULL){   
		memset(szTmp,0,20);
		sprintf_s(szTmp,"�޷��������ļ�%s",in_filename);
		AfxMessageBox(szTmp); 
		if (NULL != pAESEncryptKey)
		{
			delete pAESEncryptKey;
			pAESEncryptKey = NULL;
		}
		return false;
	}   

	fseek (fpSrc, 0, SEEK_END);				//���ļ�ָ���ƶ��ļ���β
	int nOriginDataLen = ftell (fpSrc);		//�����ǰ�ļ�ָ������ļ���ʼ���ֽ���
	rewind(fpSrc);	//�ص��ļ���


	char *pOriginData = NULL;
	pOriginData = new char[nOriginDataLen + 1];
	memset(pOriginData,0,nOriginDataLen+1);

	//�������֮�����ݳ���
	int nEncryptDataLen = (nOriginDataLen/16+1)*16;		//��չ�ֽ���Ϊ16�ı���
	char *pEncryptData = NULL;
	pEncryptData = new char[nEncryptDataLen + 1];
	memset(pEncryptData,0,nEncryptDataLen + 1);

	//��ȡ�ļ�����
	int nIndex = 0;
	while(!feof(fpSrc)){ 
		pOriginData[nIndex++] = fgetc(fpSrc);
	}
	
	bool bRet = false;
	//��������
	bRet = SocketDataEncrypt((unsigned char *)pEncryptData,pAESEncryptKey,(unsigned char *)pOriginData,nEncryptDataLen);

	//���ܳɹ�֮��д����һ���ļ�
	if (bRet)
	{
		fpDst = fopen(out_file,"wb");   
		if(fpDst == NULL){   
			memset(szTmp,0,20);
			sprintf_s(szTmp,"�޷�������ļ�%s",out_file);
			AfxMessageBox(szTmp); 
			fclose(fpSrc);//�ر�Դ�ļ�
			if (NULL != pEncryptData)
			{
				delete pEncryptData;
				pEncryptData = NULL;
			}
			if (NULL != pOriginData)
			{
				delete pOriginData;
				pOriginData = NULL;
			}
			if (NULL != pAESEncryptKey)
			{
				delete pAESEncryptKey;
				pAESEncryptKey = NULL;
			}
			return false;
		}   

		EncryptDecryptFile *pEncryptDecryptFile = NULL;
		pEncryptDecryptFile = new EncryptDecryptFile;
		memset(pEncryptDecryptFile,0,sizeof(EncryptDecryptFile));

		memcpy(pEncryptDecryptFile->m_szTag,"TK",2);	//���ܱ�־
		pEncryptDecryptFile->m_nOriginDataLen = nOriginDataLen;	//ԭʼ���ݳ��ȣ����ǰ��

		char *pProtocol = (char *)pEncryptDecryptFile;

		//д�����Э��ͷ
		for (int i = 0; i < sizeof(EncryptDecryptFile); i++)
		{
			fputc(pProtocol[i],fpDst);
		}

		//�����ܺ������д���ļ�
		for (int i = 0; i < nEncryptDataLen; ++i)
		{
			fputc(pEncryptData[i],fpDst);
		}
		fclose(fpSrc);//�ر�Դ�ļ�
		fclose(fpDst);//�ر�Ŀ���ļ�

		if (NULL != pEncryptDecryptFile)
		{
			delete pEncryptDecryptFile;
			pEncryptDecryptFile = NULL;
		}
	}
	

	fclose(fpSrc);//�ر�Դ�ļ�  
	  

	if (NULL != pEncryptData)
	{
		delete pEncryptData;
		pEncryptData = NULL;
	}

	if (NULL != pOriginData)
	{
		delete pOriginData;
		pOriginData = NULL;
	}

	if (NULL != pAESEncryptKey)
	{
		delete pAESEncryptKey;
		pAESEncryptKey = NULL;
	}
	return bRet;
}   


/************************************************************************/
/* 
	����˵������������
	����˵����
		in_filename���������ļ���
		pwd�������ļ�����
		out_file�����ܺ��ļ�����
*/
/************************************************************************/
bool CEncryptDecryptDlg::AESDeCodeFile(const char *in_filename,const char *pwd,const char *out_file)   
{   
	if (NULL == in_filename || NULL == pwd || NULL == out_file)
	{
		return false;
	}

	AES_KEY *pAESDecryptKey = NULL;	//AES����KEY
	pAESDecryptKey = new AES_KEY;
	//���ûỰKEY������AES��Կ�Բ�����
	AES_set_decrypt_key((const unsigned char *)pwd,256,pAESDecryptKey);	//����Key


	FILE *fpSrc, *fpDst;   
	char	szTmp[256] = {0};


	fpSrc = fopen(in_filename,"rb");  
	if(fpSrc == NULL){   
		memset(szTmp,0,20);
		sprintf_s(szTmp,"�޷��������ļ�%s",in_filename);
		AfxMessageBox(szTmp); 
		if (NULL != pAESDecryptKey)
		{
			delete pAESDecryptKey;
			pAESDecryptKey = NULL;
		}
		return false;
	}   

	fseek (fpSrc, 0, SEEK_END);				//���ļ�ָ���ƶ��ļ���β
	int nFileSize = ftell (fpSrc);		//�����ǰ�ļ�ָ������ļ���ʼ���ֽ���
	rewind(fpSrc);	//�ص��ļ���

	int nHeadLen = sizeof(EncryptDecryptFile);

	//С�ڼ���Э��ͷ����
	if (nFileSize < nHeadLen)
	{
		if (NULL != pAESDecryptKey)
		{
			delete pAESDecryptKey;
			pAESDecryptKey = NULL;
		}
		fclose(fpSrc);//�ر�Դ�ļ�
		return false;
	}

	char *pProtocol = NULL;
	pProtocol = new char[nHeadLen + 1];
	memset(pProtocol,0,nHeadLen + 1);

	int nCount = 0;
	while (!feof(fpSrc) && ( nCount < nHeadLen))
	{
		pProtocol[nCount++] = fgetc(fpSrc);
	}

	EncryptDecryptFile *pEncryptDecryptFile = (EncryptDecryptFile *)pProtocol;
	if (strncmp(pEncryptDecryptFile->m_szTag,"TK",2))	//Э���ǲ���ȷ
	{
		if (NULL != pAESDecryptKey)
		{
			delete pAESDecryptKey;
			pAESDecryptKey = NULL;
		}
		if (NULL != pProtocol)
		{
			delete pProtocol;
			pProtocol = NULL;
		}
		fclose(fpSrc);//�ر�Դ�ļ�
		return false;
	}

	//ԭʼ���ݳ��ȣ����֮ǰ���ȣ�
	int nOriginDataLen = pEncryptDecryptFile->m_nOriginDataLen;


	if (NULL != pProtocol)
	{
		delete pProtocol;
		pProtocol = NULL;
	}

	int nEncryptDataLen = nFileSize - nHeadLen;		//����֮�����ݳ���
	//����������ݿռ�
	char *pEncryptData = NULL;
	pEncryptData = new char[nEncryptDataLen + 1];
	memset(pEncryptData,0,nEncryptDataLen + 1);

	//����������ݿռ�
	int nDecryptDataLen = nEncryptDataLen;
	char *pDecryptData = NULL;
	pDecryptData = new char[nDecryptDataLen + 1];
	memset(pDecryptData,0,nDecryptDataLen + 1);

	//��ȡ�ļ����ݣ����ڶ�ȡ�����ļ�ָ���Ѿ�ƫ����Э��ͷ���棩
	int nIndex = 0;
	while(!feof(fpSrc)){ 
		pEncryptData[nIndex++] = fgetc(fpSrc);
	}

	//��������
	bool bRet = SocketDataDecrypt((unsigned char *)pDecryptData,pAESDecryptKey,(unsigned char *)pEncryptData,nDecryptDataLen);

	//���ܳɹ�֮��д����һ���ļ�
	if (bRet)
	{
		fpDst = fopen(out_file,"wb");   
		if(fpDst == NULL){   
			memset(szTmp,0,20);
			sprintf_s(szTmp,"�޷�������ļ�%s",out_file);
			AfxMessageBox(szTmp); 
			fclose(fpSrc);//�ر�Դ�ļ�
			if (NULL != pDecryptData)
			{
				delete pDecryptData;
				pDecryptData = NULL;
			}

			if (NULL != pEncryptData)
			{
				delete pEncryptData;
				pEncryptData = NULL;
			}
			if (NULL != pAESDecryptKey)
			{
				delete pAESDecryptKey;
				pAESDecryptKey = NULL;
			}
			return false; 
		}   

		//�����ܺ������д���ļ���ֻд�����֮ǰ�ĳ��ȣ�
		for (int i = 0; i < nOriginDataLen; ++i)
		{
			fputc(pDecryptData[i],fpDst);
		}
		fclose(fpSrc);//�ر�Դ�ļ�
		fclose(fpDst);//�ر�Ŀ���ļ�
	}


	fclose(fpSrc);//�ر�Դ�ļ�

	pDecryptData[nOriginDataLen] = '\0';
	/*std::string strMD5 = GetDataMD5(pDecryptData,nOriginDataLen);
	TRACE("==strMD5:%s",strMD5.c_str());*/
	

	if (NULL != pDecryptData)
	{
		delete pDecryptData;
		pDecryptData = NULL;
	}

	if (NULL != pEncryptData)
	{
		delete pEncryptData;
		pEncryptData = NULL;
	}

	if (NULL != pAESDecryptKey)
	{
		delete pAESDecryptKey;
		pAESDecryptKey = NULL;
	}

	return bRet;
}   

void CEncryptDecryptDlg::OnBnClickedBtnEncrypt()
{
	int nFileCount = m_listAcceptFile.GetCount();	//��ȡ�б�ؼ�������Ŀ

	if (nFileCount == 0)
	{
		AfxMessageBox(_T("û��ѡ������ܵ��ļ�!!!"));
		return;
	}


	CString cstrPath;
	GetDlgItemText(IDC_EDIT_SAVEPATH,cstrPath);
	std::string strPath = cstrPath.GetBuffer(cstrPath.GetLength());

	if (strPath.empty())
	{
		AfxMessageBox(_T("��ѡ�񱣴�·��!!!"));
		return;
	}

	std::string strSrcFile = "";
	std::string strDstFile = "";
	std::string strFileName = "";
	CString cstrFile;
	//int nFileCount = m_listAcceptFile.GetCount();	//��ȡ�б�ؼ�������Ŀ
	for (int nIndex = 0 ; nIndex < nFileCount ; ++nIndex)
	{
		m_listAcceptFile.GetText(nIndex,cstrFile);
		strSrcFile = cstrFile.GetBuffer(cstrFile.GetLength());
		strFileName = strSrcFile.substr(strSrcFile.find_last_of("\\")+1);	//�ļ�����
		strFileName = strFileName.substr(0,strFileName.find_last_of("."));	//�ļ�����
		strDstFile = strPath + "\\" + strFileName + ".enc";

		switch (m_nEnDeType)
		{
		case 0:			//ʹ��AES ����Key�����ݽ��м���
			AESEnCodeFile(strSrcFile.c_str(),"1234567890QWERTYUIOPASDFRGHTCBXF",strDstFile.c_str());
			break;
		case 1:			//blowfish�㷨�ӽ����ļ�
			BlowFishEnCodeFile(strSrcFile.c_str(),strDstFile.c_str());
			break;
		default:
			break;
		}
		
	}
}





bool CEncryptDecryptDlg::SocketDataEncrypt(unsigned char *pOutData,AES_KEY *pAESEncrypt,unsigned char *pInData,int nDataLen,int nLen)
{
	if(pOutData == NULL || pAESEncrypt == NULL 
		|| pInData == NULL || nDataLen <= 0)
	{
		return false;
	}

	unsigned char szEncryptData[17] = {0};
	for(int i = 0; i < nDataLen; i+= nLen)
	{
		memset((char *)&szEncryptData,0,17);
		AES_encrypt(pInData+i,(unsigned char *)&szEncryptData,pAESEncrypt);
		memcpy(pOutData+i,szEncryptData,nLen);
	}

	return true;
}


bool CEncryptDecryptDlg::SocketDataDecrypt(unsigned char *pOutData,AES_KEY *pAESDecrypt,unsigned char *pInData,int nDataLen,int nLen)
{
	if(pOutData == NULL || pAESDecrypt == NULL ||
		pInData == NULL || nDataLen <= 0)
	{
		return false;
	}

	unsigned char szDecryptData[17] = {0};
	for(int i = 0;i < nDataLen; i+= nLen)
	{
		memset((char *)&szDecryptData,0,17);
		AES_decrypt(pInData+i,(unsigned char *)&szDecryptData,pAESDecrypt);
		memcpy(pOutData+i,szDecryptData,nLen);
	}

	return true;
}


//blowfish�㷨�ӽ����ļ�
bool CEncryptDecryptDlg::BlowFishEnCodeFile(const char *in_filename,const char *out_file)
{
	if (NULL == in_filename || NULL == out_file || NULL == m_pBlowFish)
	{
		return false;
	}

	FILE *fpSrc, *fpDst;   

	fpSrc = fopen(in_filename,"rb");
	char	szTmp[256] = {0};


	if(fpSrc == NULL){   
		memset(szTmp,0,20);
		sprintf_s(szTmp,"�޷��������ļ�%s",in_filename);
		AfxMessageBox(szTmp); 
		return false;
	}   

	fseek (fpSrc, 0, SEEK_END);				//���ļ�ָ���ƶ��ļ���β
	int nOriginDataLen = ftell (fpSrc);		//�����ǰ�ļ�ָ������ļ���ʼ���ֽ���
	rewind(fpSrc);	//�ص��ļ���


	char *pOriginData = NULL;
	pOriginData = new char[nOriginDataLen + 1];
	memset(pOriginData,0,nOriginDataLen+1);

	//��ȡ�ļ�����
	int nIndex = 0;
	while(!feof(fpSrc)){ 
		pOriginData[nIndex++] = fgetc(fpSrc);
	}

	
	//�������֮�����ݳ���
	int nEncryptDataLen = (nOriginDataLen / 8 + 1) * 8;		//�������֮��ĳ���
	char *pEncryptData = NULL;
	pEncryptData = new char[nEncryptDataLen + 1];
	memset(pEncryptData,0,nEncryptDataLen + 1);

	memcpy(pEncryptData,pOriginData,nOriginDataLen);
	bool bRet = false;
	bRet = m_pBlowFish->Encrypt((unsigned char*)pEncryptData,nEncryptDataLen);

	//���ܳɹ�֮��д����һ���ļ�
	if (bRet)
	{
		fpDst = fopen(out_file,"wb");   
		if(fpDst == NULL){   
			memset(szTmp,0,20);
			sprintf_s(szTmp,"�޷�������ļ�%s",out_file);
			AfxMessageBox(szTmp); 
			fclose(fpSrc);//�ر�Դ�ļ�
			if (NULL != pOriginData)
			{
				delete pOriginData;
				pOriginData = NULL;
			}
			if (NULL != pEncryptData)
			{
				delete pEncryptData;
				pEncryptData = NULL;
			}
			return false;
		}   

		EncryptDecryptFile *pEncryptDecryptFile = NULL;
		pEncryptDecryptFile = new EncryptDecryptFile;
		memset(pEncryptDecryptFile,0,sizeof(EncryptDecryptFile));

		memcpy(pEncryptDecryptFile->m_szTag,"TK",2);	//���ܱ�־
		pEncryptDecryptFile->m_nOriginDataLen = nOriginDataLen;	//ԭʼ���ݳ��ȣ����ǰ��

		char *pProtocol = (char *)pEncryptDecryptFile;

		//д�����Э��ͷ
		for (int i = 0; i < sizeof(EncryptDecryptFile); i++)
		{
			fputc(pProtocol[i],fpDst);
		}

		//�����ܺ������д���ļ�
		for (int i = 0; i < nEncryptDataLen; ++i)
		{
			fputc(pEncryptData[i],fpDst);
		}

		fclose(fpSrc);//�ر�Դ�ļ�
		fclose(fpDst);//�ر�Ŀ���ļ�

		if (NULL != pEncryptDecryptFile)
		{
			delete pEncryptDecryptFile;
			pEncryptDecryptFile = NULL;
		}
	}


	fclose(fpSrc);//�ر�Դ�ļ�  


	if (NULL != pEncryptData)
	{
		delete pEncryptData;
		pEncryptData = NULL;
	}

	if (NULL != pOriginData)
	{
		delete pOriginData;
		pOriginData = NULL;
	}
	return bRet;
}


//blowfish�㷨�ӽ����ļ�
bool CEncryptDecryptDlg::BlowFishDeCodeFile(const char *in_filename,const char *out_file)
{
	if (NULL == in_filename || NULL == out_file || NULL == m_pBlowFish)
	{
		return false;
	}

	FILE *fpSrc, *fpDst;   
	char	szTmp[256] = {0};


	fpSrc = fopen(in_filename,"rb");  
	if(fpSrc == NULL){   
		memset(szTmp,0,20);
		sprintf_s(szTmp,"�޷��������ļ�%s",in_filename);
		AfxMessageBox(szTmp); 
		return false;
	}   

	fseek (fpSrc, 0, SEEK_END);				//���ļ�ָ���ƶ��ļ���β
	int nFileSize = ftell (fpSrc);		//�����ǰ�ļ�ָ������ļ���ʼ���ֽ���
	rewind(fpSrc);	//�ص��ļ���

	int nHeadLen = sizeof(EncryptDecryptFile);

	//С�ڼ���Э��ͷ����
	if (nFileSize < nHeadLen)
	{
		fclose(fpSrc);//�ر�Դ�ļ�
		return false;
	}

	char *pProtocol = NULL;
	pProtocol = new char[nHeadLen + 1];
	memset(pProtocol,0,nHeadLen + 1);

	int nCount = 0;
	while (!feof(fpSrc) && ( nCount < nHeadLen))
	{
		pProtocol[nCount++] = fgetc(fpSrc);
	}

	EncryptDecryptFile *pEncryptDecryptFile = (EncryptDecryptFile *)pProtocol;
	if (strncmp(pEncryptDecryptFile->m_szTag,"TK",2))	//Э���ǲ���ȷ
	{
		if (NULL != pProtocol)
		{
			delete pProtocol;
			pProtocol = NULL;
		}
		return false;
	}

	//ԭʼ���ݳ��ȣ����֮ǰ���ȣ�
	int nOriginDataLen = pEncryptDecryptFile->m_nOriginDataLen;


	if (NULL != pProtocol)
	{
		delete pProtocol;
		pProtocol = NULL;
	}

	int nDecryptDataLen = nFileSize - nHeadLen;		//����֮�����ݳ��� == ����֮��ĳ���
	//���䱣��������ݵĿռ�
	char *pDecryptData = NULL;
	pDecryptData = new char[nDecryptDataLen + 1];
	memset(pDecryptData,0,nDecryptDataLen + 1);

	//��ȡ�ļ����ݣ����ڶ�ȡ�����ļ�ָ���Ѿ�ƫ����Э��ͷ���棩
	int nIndex = 0;
	while(!feof(fpSrc)){ 
		pDecryptData[nIndex++] = fgetc(fpSrc);
	}

	bool bRet = false;
	bRet = m_pBlowFish->Decrypt((unsigned char*)pDecryptData,nDecryptDataLen);

	//���ܳɹ�֮��д����һ���ļ�
	if (bRet)
	{
		fpDst = fopen(out_file,"wb");   
		if(fpDst == NULL){   
			memset(szTmp,0,20);
			sprintf_s(szTmp,"�޷�������ļ�%s",out_file);
			AfxMessageBox(szTmp); 
			fclose(fpSrc);//�ر�Դ�ļ�
			if (NULL != pDecryptData)
			{
				delete pDecryptData;
				pDecryptData = NULL;
			}

			return false; 
		}   

		//�����ܺ������д���ļ���ֻд�����֮ǰ�ĳ��ȣ�
		for (int i = 0; i < nOriginDataLen; ++i)
		{
			fputc(pDecryptData[i],fpDst);
		}

		fclose(fpDst);//�ر�Ŀ���ļ�
	}


	fclose(fpSrc);//�ر�Դ�ļ�

	pDecryptData[nOriginDataLen] = '\0';
	/*std::string strMD5 = GetDataMD5(pDecryptData,nOriginDataLen);
	TRACE("==strMD5:%s",strMD5.c_str());*/

	if (NULL != pDecryptData)
	{
		delete pDecryptData;
		pDecryptData = NULL;
	}

	return bRet;
}


std::string CEncryptDecryptDlg::GetDataMD5(char *pCertData,int nCertLen)
{
	if(pCertData == NULL || nCertLen <= 0)
	{
		return "";
	}

	MD5_CTX md5CTX;
	unsigned char szMD5Result[17] = {0};
	MD5_Init(&md5CTX);
	MD5_Update(&md5CTX,(char *)pCertData,nCertLen);
	MD5_Final(szMD5Result,&md5CTX);

	std::string strMD5 = "";
	char szMD5[3] = {0};
	for(int i=0; i<16; ++i)
	{
		memset(szMD5,0,3);
		sprintf_s(szMD5,3,"%02x",(int)szMD5Result[i]);
		strMD5 += szMD5;
	}

	return strMD5;
}


void CEncryptDecryptDlg::OnLbnSelchangeListAcceptfile()
{
	int nSel;
	nSel = m_listAcceptFile.GetCurSel();
	CString cstrSelFile;
	m_listAcceptFile.GetText(nSel,cstrSelFile);
	//MessageBox(cstrSel,_T("��ѡ�����"),MB_OK);
	std::string strFile = cstrSelFile.GetBuffer(cstrSelFile.GetLength());

	FILE *fpFile = fopen(strFile.c_str(),"rb");  
	if(fpFile == NULL){   
		return ;
	}   

	fseek (fpFile, 0, SEEK_END);				//���ļ�ָ���ƶ��ļ���β
	int nFileSize = ftell (fpFile);		//�����ǰ�ļ�ָ������ļ���ʼ���ֽ���
	rewind(fpFile);	//�ص��ļ���


	char *pFileData = NULL;
	pFileData = new char[nFileSize + 1];
	memset(pFileData,0,nFileSize + 1);

	int nIndex = 0;
	while (!feof(fpFile))
	{
		pFileData[nIndex++] = fgetc(fpFile);
	}

	std::string strMD5 = GetDataMD5(pFileData,nFileSize);

	if (NULL != pFileData)
	{
		delete[] pFileData;
		pFileData = NULL;
	}

	if (NULL != fpFile)
	{
		fclose(fpFile);
	}

	cstrSelFile.ReleaseBuffer();

	SetDlgItemText(IDC_EDIT_MD5,strMD5.c_str());

	UpdateData();
}


void CEncryptDecryptDlg::OnLbnDblclkListAcceptfile()
{
	m_listAcceptFile.DeleteString(m_listAcceptFile.GetCurSel());
}


void CEncryptDecryptDlg::OnBnClickedBtnClearfile()
{
	m_listAcceptFile.ResetContent();
	UpdateData();
}


void CEncryptDecryptDlg::OnBnClickedBtnDecrypt()
{
	int nFileCount = m_listAcceptFile.GetCount();	//��ȡ�б�ؼ�������Ŀ

	if (nFileCount == 0)
	{
		AfxMessageBox(_T("û��ѡ������ܵ��ļ�!!!"));
		return;
	}


	CString cstrPath;
	GetDlgItemText(IDC_EDIT_SAVEPATH,cstrPath);
	std::string strPath = cstrPath.GetBuffer(cstrPath.GetLength());

	if (strPath.empty())
	{
		AfxMessageBox(_T("��ѡ�񱣴�·��!!!"));
		return;
	}

	std::string strSrcFile = "";
	std::string strDstFile = "";
	std::string strFileName = "";
	CString cstrFile;


	for (int nIndex = 0 ; nIndex < nFileCount ; ++nIndex)
	{
		m_listAcceptFile.GetText(nIndex,cstrFile);
		strSrcFile = cstrFile.GetBuffer(cstrFile.GetLength());
		strFileName = strSrcFile.substr(strSrcFile.find_last_of("\\")+1);	//�ļ�����
		strFileName = strFileName.substr(0,strFileName.find_last_of("."));	//�ļ�����
		strDstFile = strPath + "\\" + strFileName + ".dec";

		switch (m_nEnDeType)
		{
		case 0:		//AES�㷨�ӽ����ļ�
			AESDeCodeFile(strSrcFile.c_str(),"1234567890QWERTYUIOPASDFRGHTCBXF",strDstFile.c_str());
			break;
		case 1:		//blowfish�㷨�ӽ����ļ�
			BlowFishDeCodeFile(strSrcFile.c_str(),strDstFile.c_str());
			break;
		default:
			break;
		}
		
	}
}


////////////////////////////////////////////////////////////////////////////////
//
// ��������:    GetCipherKey
//
// ����˵��:    �����������ɼӽ���KEY
//
// ����˵��:
//
// �� �� ֵ:
//
// ��Ҫ˵��:
//
////////////////////////////////////////////////////////////////////////////////
std::string CEncryptDecryptDlg::GetCipherKey(char *pKey)
{
	CMD5 md5;

	std::string strKey = pKey;
	strKey = md5.MD5_String(strKey.c_str()); //����һ��
	strKey = strKey.substr(16) + strKey.substr(0,16);  //ǰ��16λ����
	strKey = md5.MD5_String(strKey.c_str());  //������һ��
	strKey = strKey.substr(0,8);  //ȡǰ��8λ

	return strKey;
}

// ��ȡ��Ͽ�ؼ����б����ѡ��������� 
void CEncryptDecryptDlg::OnCbnSelchangeComboEndetype()
{
	m_nEnDeType = m_comboEnDeType.GetCurSel();   
}
