
// EncryptDecryptDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CEncryptDecryptDlg 对话框



CEncryptDecryptDlg::CEncryptDecryptDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEncryptDecryptDlg::IDD, pParent)
	, m_cstrSourceFile(_T(""))
{
	m_pBlowFish = NULL;		//数据加解密对象
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


// CEncryptDecryptDlg 消息处理程序

BOOL CEncryptDecryptDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_nEnDeType = 0;
	m_comboEnDeType.InsertString(0,_T("AES算法加解密"));
	m_comboEnDeType.InsertString(1,_T("BlowFish算法加解密"));
	m_comboEnDeType.SetCurSel(0);

	//初始化创建用户信息加解密对象
	std::string strEncKey = "216E0D565E294060";
	std::string strCommunicateKey = GetCipherKey((char*)strEncKey.c_str());
	m_pBlowFish = new CBlowFish((unsigned char*)strCommunicateKey.c_str(),strCommunicateKey.length());


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEncryptDecryptDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CEncryptDecryptDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CEncryptDecryptDlg::OnBnClickedBtnOpen()
{
	char strExt[_MAX_EXT] = "tif";//设置读取的文件类型  
	CString szFilter = "Encrypt Files (*.Enc)|*.Enc|Decrypt Files (*.Dec)|*.Dec|Cert Files (*.cer)|*.cer|Pem Files (*.key)|*.key|All Files (*.*)|*.*||";  
	CFileDialog   dlg( TRUE, strExt,NULL,OFN_FILEMUSTEXIST | OFN_HIDEREADONLY| OFN_ALLOWMULTISELECT,szFilter );  
	dlg.m_ofn.lpstrTitle = _T("请选择需要处理的文件");//设置标题  
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
	int SelFileNum = aryFilename.GetSize();//获取选择的文件数  
	if(SelFileNum==0)  
	{  
		AfxMessageBox("没有选择需要处理的文件！");  
		return ;  
	}  

	int nIndex = 0;
	for (; nIndex < SelFileNum; ++nIndex)
	{
		m_listAcceptFile.AddString(aryFilename.GetAt(nIndex)); 
	}
	UpdateData();
	
	//CFileDialog dlgFile(TRUE,0,0,OFN_FILEMUSTEXIST,0,0,0);		//文件选择
	//if(dlgFile.DoModal()==IDOK)
	//{
	//	m_cstrSourceFile = dlgFile.GetPathName();		//获取选中的文件的路径名	
	//}else{
	//	m_cstrSourceFile = "请选择！";
	//}

	//UpdateData(FALSE);
	//::SetDlgItemText(m_hWnd,IDC_EDIT_SOURCEFILE,m_cstrSourceFile);
}


void CEncryptDecryptDlg::OnDropFiles(HDROP hDropInfo)
{
	int DropCount=DragQueryFile(hDropInfo,-1,NULL,0);//取得被拖动文件的数目  
	for(int i=0;i< DropCount;i++)  
	{  
		WCHAR wcStr[MAX_PATH];  
		DragQueryFile(hDropInfo,i,(LPSTR)wcStr,MAX_PATH);//获得拖曳的第i个文件的文件名  
		m_listAcceptFile.AddString((LPSTR)wcStr);  
	}   
	DragFinish(hDropInfo);  //拖放结束后,释放内存

	CDialogEx::OnDropFiles(hDropInfo);
}


//选择保存目录
void CEncryptDecryptDlg::OnBnClickedBtnSavepath()
{
	char szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = "请选择需要打包的目录：";   
	bi.ulFlags = 0;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		str.Format("选择的目录为 %s",  szPath);
		//AfxMessageBox(str); 
		SetDlgItemText(IDC_EDIT_SAVEPATH,szPath);

	}
	else   
	{
		SetDlgItemText(IDC_EDIT_SAVEPATH,"无效的目录，请重新选择");
		//AfxMessageBox("无效的目录，请重新选择");   
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
	函数说明：加密数据
	参数说明：
		in_filename：待加密文件名
		pwd：加密文件密码
		out_file：加密后文件密码
*/
/************************************************************************/
bool  CEncryptDecryptDlg::AESEnCodeFile(const char *in_filename,const char *pwd,const char *out_file)   
{   
	if (NULL == in_filename || NULL == pwd || NULL == out_file)
	{
		return false;
	}

	AES_KEY *pAESEncryptKey = NULL;	//AES加密KEY
	pAESEncryptKey = new AES_KEY;
	//利用会话KEY，生成AES密钥对并更新
	AES_set_encrypt_key((const unsigned char *)pwd,256,pAESEncryptKey);	//加密Key


	FILE *fpSrc, *fpDst;   
		
	fpSrc = fopen(in_filename,"rb");
	char	szTmp[256] = {0};


	if(fpSrc == NULL){   
		memset(szTmp,0,20);
		sprintf_s(szTmp,"无法打开输入文件%s",in_filename);
		AfxMessageBox(szTmp); 
		if (NULL != pAESEncryptKey)
		{
			delete pAESEncryptKey;
			pAESEncryptKey = NULL;
		}
		return false;
	}   

	fseek (fpSrc, 0, SEEK_END);				//将文件指针移动文件结尾
	int nOriginDataLen = ftell (fpSrc);		//求出当前文件指针距离文件开始的字节数
	rewind(fpSrc);	//回到文件首


	char *pOriginData = NULL;
	pOriginData = new char[nOriginDataLen + 1];
	memset(pOriginData,0,nOriginDataLen+1);

	//计算加密之后数据长度
	int nEncryptDataLen = (nOriginDataLen/16+1)*16;		//扩展字节数为16的倍数
	char *pEncryptData = NULL;
	pEncryptData = new char[nEncryptDataLen + 1];
	memset(pEncryptData,0,nEncryptDataLen + 1);

	//读取文件内容
	int nIndex = 0;
	while(!feof(fpSrc)){ 
		pOriginData[nIndex++] = fgetc(fpSrc);
	}
	
	bool bRet = false;
	//加密数据
	bRet = SocketDataEncrypt((unsigned char *)pEncryptData,pAESEncryptKey,(unsigned char *)pOriginData,nEncryptDataLen);

	//加密成功之后写入另一个文件
	if (bRet)
	{
		fpDst = fopen(out_file,"wb");   
		if(fpDst == NULL){   
			memset(szTmp,0,20);
			sprintf_s(szTmp,"无法打开输出文件%s",out_file);
			AfxMessageBox(szTmp); 
			fclose(fpSrc);//关闭源文件
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

		memcpy(pEncryptDecryptFile->m_szTag,"TK",2);	//加密标志
		pEncryptDecryptFile->m_nOriginDataLen = nOriginDataLen;	//原始数据长度（填充前）

		char *pProtocol = (char *)pEncryptDecryptFile;

		//写入加密协议头
		for (int i = 0; i < sizeof(EncryptDecryptFile); i++)
		{
			fputc(pProtocol[i],fpDst);
		}

		//将加密后的数据写入文件
		for (int i = 0; i < nEncryptDataLen; ++i)
		{
			fputc(pEncryptData[i],fpDst);
		}
		fclose(fpSrc);//关闭源文件
		fclose(fpDst);//关闭目标文件

		if (NULL != pEncryptDecryptFile)
		{
			delete pEncryptDecryptFile;
			pEncryptDecryptFile = NULL;
		}
	}
	

	fclose(fpSrc);//关闭源文件  
	  

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
	函数说明：解密数据
	参数说明：
		in_filename：待解密文件名
		pwd：解密文件密码
		out_file：解密后文件密码
*/
/************************************************************************/
bool CEncryptDecryptDlg::AESDeCodeFile(const char *in_filename,const char *pwd,const char *out_file)   
{   
	if (NULL == in_filename || NULL == pwd || NULL == out_file)
	{
		return false;
	}

	AES_KEY *pAESDecryptKey = NULL;	//AES解密KEY
	pAESDecryptKey = new AES_KEY;
	//利用会话KEY，生成AES密钥对并更新
	AES_set_decrypt_key((const unsigned char *)pwd,256,pAESDecryptKey);	//解密Key


	FILE *fpSrc, *fpDst;   
	char	szTmp[256] = {0};


	fpSrc = fopen(in_filename,"rb");  
	if(fpSrc == NULL){   
		memset(szTmp,0,20);
		sprintf_s(szTmp,"无法打开输入文件%s",in_filename);
		AfxMessageBox(szTmp); 
		if (NULL != pAESDecryptKey)
		{
			delete pAESDecryptKey;
			pAESDecryptKey = NULL;
		}
		return false;
	}   

	fseek (fpSrc, 0, SEEK_END);				//将文件指针移动文件结尾
	int nFileSize = ftell (fpSrc);		//求出当前文件指针距离文件开始的字节数
	rewind(fpSrc);	//回到文件首

	int nHeadLen = sizeof(EncryptDecryptFile);

	//小于加密协议头长度
	if (nFileSize < nHeadLen)
	{
		if (NULL != pAESDecryptKey)
		{
			delete pAESDecryptKey;
			pAESDecryptKey = NULL;
		}
		fclose(fpSrc);//关闭源文件
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
	if (strncmp(pEncryptDecryptFile->m_szTag,"TK",2))	//协议标记不正确
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
		fclose(fpSrc);//关闭源文件
		return false;
	}

	//原始数据长度（填充之前长度）
	int nOriginDataLen = pEncryptDecryptFile->m_nOriginDataLen;


	if (NULL != pProtocol)
	{
		delete pProtocol;
		pProtocol = NULL;
	}

	int nEncryptDataLen = nFileSize - nHeadLen;		//加密之后数据长度
	//分配加密数据空间
	char *pEncryptData = NULL;
	pEncryptData = new char[nEncryptDataLen + 1];
	memset(pEncryptData,0,nEncryptDataLen + 1);

	//分配解密数据空间
	int nDecryptDataLen = nEncryptDataLen;
	char *pDecryptData = NULL;
	pDecryptData = new char[nDecryptDataLen + 1];
	memset(pDecryptData,0,nDecryptDataLen + 1);

	//读取文件内容（现在读取的是文件指针已经偏移至协议头后面）
	int nIndex = 0;
	while(!feof(fpSrc)){ 
		pEncryptData[nIndex++] = fgetc(fpSrc);
	}

	//解密数据
	bool bRet = SocketDataDecrypt((unsigned char *)pDecryptData,pAESDecryptKey,(unsigned char *)pEncryptData,nDecryptDataLen);

	//解密成功之后写入另一个文件
	if (bRet)
	{
		fpDst = fopen(out_file,"wb");   
		if(fpDst == NULL){   
			memset(szTmp,0,20);
			sprintf_s(szTmp,"无法打开输出文件%s",out_file);
			AfxMessageBox(szTmp); 
			fclose(fpSrc);//关闭源文件
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

		//将解密后的数据写入文件（只写入填充之前的长度）
		for (int i = 0; i < nOriginDataLen; ++i)
		{
			fputc(pDecryptData[i],fpDst);
		}
		fclose(fpSrc);//关闭源文件
		fclose(fpDst);//关闭目标文件
	}


	fclose(fpSrc);//关闭源文件

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
	int nFileCount = m_listAcceptFile.GetCount();	//获取列表控件的总数目

	if (nFileCount == 0)
	{
		AfxMessageBox(_T("没有选择待加密的文件!!!"));
		return;
	}


	CString cstrPath;
	GetDlgItemText(IDC_EDIT_SAVEPATH,cstrPath);
	std::string strPath = cstrPath.GetBuffer(cstrPath.GetLength());

	if (strPath.empty())
	{
		AfxMessageBox(_T("请选择保存路径!!!"));
		return;
	}

	std::string strSrcFile = "";
	std::string strDstFile = "";
	std::string strFileName = "";
	CString cstrFile;
	//int nFileCount = m_listAcceptFile.GetCount();	//获取列表控件的总数目
	for (int nIndex = 0 ; nIndex < nFileCount ; ++nIndex)
	{
		m_listAcceptFile.GetText(nIndex,cstrFile);
		strSrcFile = cstrFile.GetBuffer(cstrFile.GetLength());
		strFileName = strSrcFile.substr(strSrcFile.find_last_of("\\")+1);	//文件名称
		strFileName = strFileName.substr(0,strFileName.find_last_of("."));	//文件名称
		strDstFile = strPath + "\\" + strFileName + ".enc";

		switch (m_nEnDeType)
		{
		case 0:			//使用AES 加密Key对数据进行加密
			AESEnCodeFile(strSrcFile.c_str(),"1234567890QWERTYUIOPASDFRGHTCBXF",strDstFile.c_str());
			break;
		case 1:			//blowfish算法加解密文件
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


//blowfish算法加解密文件
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
		sprintf_s(szTmp,"无法打开输入文件%s",in_filename);
		AfxMessageBox(szTmp); 
		return false;
	}   

	fseek (fpSrc, 0, SEEK_END);				//将文件指针移动文件结尾
	int nOriginDataLen = ftell (fpSrc);		//求出当前文件指针距离文件开始的字节数
	rewind(fpSrc);	//回到文件首


	char *pOriginData = NULL;
	pOriginData = new char[nOriginDataLen + 1];
	memset(pOriginData,0,nOriginDataLen+1);

	//读取文件内容
	int nIndex = 0;
	while(!feof(fpSrc)){ 
		pOriginData[nIndex++] = fgetc(fpSrc);
	}

	
	//计算加密之后数据长度
	int nEncryptDataLen = (nOriginDataLen / 8 + 1) * 8;		//计算加密之后的长度
	char *pEncryptData = NULL;
	pEncryptData = new char[nEncryptDataLen + 1];
	memset(pEncryptData,0,nEncryptDataLen + 1);

	memcpy(pEncryptData,pOriginData,nOriginDataLen);
	bool bRet = false;
	bRet = m_pBlowFish->Encrypt((unsigned char*)pEncryptData,nEncryptDataLen);

	//加密成功之后写入另一个文件
	if (bRet)
	{
		fpDst = fopen(out_file,"wb");   
		if(fpDst == NULL){   
			memset(szTmp,0,20);
			sprintf_s(szTmp,"无法打开输出文件%s",out_file);
			AfxMessageBox(szTmp); 
			fclose(fpSrc);//关闭源文件
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

		memcpy(pEncryptDecryptFile->m_szTag,"TK",2);	//加密标志
		pEncryptDecryptFile->m_nOriginDataLen = nOriginDataLen;	//原始数据长度（填充前）

		char *pProtocol = (char *)pEncryptDecryptFile;

		//写入加密协议头
		for (int i = 0; i < sizeof(EncryptDecryptFile); i++)
		{
			fputc(pProtocol[i],fpDst);
		}

		//将加密后的数据写入文件
		for (int i = 0; i < nEncryptDataLen; ++i)
		{
			fputc(pEncryptData[i],fpDst);
		}

		fclose(fpSrc);//关闭源文件
		fclose(fpDst);//关闭目标文件

		if (NULL != pEncryptDecryptFile)
		{
			delete pEncryptDecryptFile;
			pEncryptDecryptFile = NULL;
		}
	}


	fclose(fpSrc);//关闭源文件  


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


//blowfish算法加解密文件
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
		sprintf_s(szTmp,"无法打开输入文件%s",in_filename);
		AfxMessageBox(szTmp); 
		return false;
	}   

	fseek (fpSrc, 0, SEEK_END);				//将文件指针移动文件结尾
	int nFileSize = ftell (fpSrc);		//求出当前文件指针距离文件开始的字节数
	rewind(fpSrc);	//回到文件首

	int nHeadLen = sizeof(EncryptDecryptFile);

	//小于加密协议头长度
	if (nFileSize < nHeadLen)
	{
		fclose(fpSrc);//关闭源文件
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
	if (strncmp(pEncryptDecryptFile->m_szTag,"TK",2))	//协议标记不正确
	{
		if (NULL != pProtocol)
		{
			delete pProtocol;
			pProtocol = NULL;
		}
		return false;
	}

	//原始数据长度（填充之前长度）
	int nOriginDataLen = pEncryptDecryptFile->m_nOriginDataLen;


	if (NULL != pProtocol)
	{
		delete pProtocol;
		pProtocol = NULL;
	}

	int nDecryptDataLen = nFileSize - nHeadLen;		//加密之后数据长度 == 解密之后的长度
	//分配保存解密数据的空间
	char *pDecryptData = NULL;
	pDecryptData = new char[nDecryptDataLen + 1];
	memset(pDecryptData,0,nDecryptDataLen + 1);

	//读取文件内容（现在读取的是文件指针已经偏移至协议头后面）
	int nIndex = 0;
	while(!feof(fpSrc)){ 
		pDecryptData[nIndex++] = fgetc(fpSrc);
	}

	bool bRet = false;
	bRet = m_pBlowFish->Decrypt((unsigned char*)pDecryptData,nDecryptDataLen);

	//解密成功之后写入另一个文件
	if (bRet)
	{
		fpDst = fopen(out_file,"wb");   
		if(fpDst == NULL){   
			memset(szTmp,0,20);
			sprintf_s(szTmp,"无法打开输出文件%s",out_file);
			AfxMessageBox(szTmp); 
			fclose(fpSrc);//关闭源文件
			if (NULL != pDecryptData)
			{
				delete pDecryptData;
				pDecryptData = NULL;
			}

			return false; 
		}   

		//将解密后的数据写入文件（只写入填充之前的长度）
		for (int i = 0; i < nOriginDataLen; ++i)
		{
			fputc(pDecryptData[i],fpDst);
		}

		fclose(fpDst);//关闭目标文件
	}


	fclose(fpSrc);//关闭源文件

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
	//MessageBox(cstrSel,_T("您选择的是"),MB_OK);
	std::string strFile = cstrSelFile.GetBuffer(cstrSelFile.GetLength());

	FILE *fpFile = fopen(strFile.c_str(),"rb");  
	if(fpFile == NULL){   
		return ;
	}   

	fseek (fpFile, 0, SEEK_END);				//将文件指针移动文件结尾
	int nFileSize = ftell (fpFile);		//求出当前文件指针距离文件开始的字节数
	rewind(fpFile);	//回到文件首


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
	int nFileCount = m_listAcceptFile.GetCount();	//获取列表控件的总数目

	if (nFileCount == 0)
	{
		AfxMessageBox(_T("没有选择待解密的文件!!!"));
		return;
	}


	CString cstrPath;
	GetDlgItemText(IDC_EDIT_SAVEPATH,cstrPath);
	std::string strPath = cstrPath.GetBuffer(cstrPath.GetLength());

	if (strPath.empty())
	{
		AfxMessageBox(_T("请选择保存路径!!!"));
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
		strFileName = strSrcFile.substr(strSrcFile.find_last_of("\\")+1);	//文件名称
		strFileName = strFileName.substr(0,strFileName.find_last_of("."));	//文件名称
		strDstFile = strPath + "\\" + strFileName + ".dec";

		switch (m_nEnDeType)
		{
		case 0:		//AES算法加解密文件
			AESDeCodeFile(strSrcFile.c_str(),"1234567890QWERTYUIOPASDFRGHTCBXF",strDstFile.c_str());
			break;
		case 1:		//blowfish算法加解密文件
			BlowFishDeCodeFile(strSrcFile.c_str(),strDstFile.c_str());
			break;
		default:
			break;
		}
		
	}
}


////////////////////////////////////////////////////////////////////////////////
//
// 函数名称:    GetCipherKey
//
// 函数说明:    根据密码生成加解密KEY
//
// 参数说明:
//
// 返 回 值:
//
// 重要说明:
//
////////////////////////////////////////////////////////////////////////////////
std::string CEncryptDecryptDlg::GetCipherKey(char *pKey)
{
	CMD5 md5;

	std::string strKey = pKey;
	strKey = md5.MD5_String(strKey.c_str()); //生成一次
	strKey = strKey.substr(16) + strKey.substr(0,16);  //前后16位交换
	strKey = md5.MD5_String(strKey.c_str());  //再生成一次
	strKey = strKey.substr(0,8);  //取前面8位

	return strKey;
}

// 获取组合框控件的列表框中选中项的索引 
void CEncryptDecryptDlg::OnCbnSelchangeComboEndetype()
{
	m_nEnDeType = m_comboEnDeType.GetCurSel();   
}
