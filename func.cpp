#include "stdafx.h"
#include "func.h"
#include "fcntl.h"
#include <Windows.h>
#include <assert.h>
#include <atlenc.h>
#include <stdio.h>
#include <io.h>
#include <string> 
using namespace std;


//https://online.xilisoft.com/getNewKey.php
#define  XL_BD_SVR_HTTPS  "https://online.xilisoft.com/getNewKey.php" //



EXTERN_C
{
	int __cdecl new8d24Obj();
	int __cdecl c_new1848Obj();
	int __cdecl decryptv1();//--------v1,-版本小于0a,解密函数
	int __cdecl decryptv2();//-----------v2-版本大于0a，解密函数


	int __cdecl sub_10004910();
	int __cdecl sub_10004920();
	int __cdecl sub_10004970();
	int __cdecl sub_10004A50();
	int __cdecl createkeyfile_wraper2();//aacs 初始化



	int __cdecl AskNetFileContent(char* svrurl , char* pObj45b4, int sizeObj45b4, char** ppkey);

};

//函数功能：替代原来访问https://75.126.159.226/api/v2/1.0.00.php，获取密钥文件的代码:AskNetFileContent
//AskNetFileContent_xlsvr功能为：访问Xili服务器，由Xili服务器代为访问75.126.159.226，最终获取密钥返回到本代码
//本函数的调用是从汇编代码(readblufile.asm)发起。
//当BD盘加密格式版本大于0a，且本地目录ProgramData\xlk中没有KEY文件时，本函数将被调用。

/*
参数含义：char* svrurl      -------------  https串
char* pObj45b4              传送的数据指针
int sizeObj45b4            传送的数据大小
char** ppkey                如果执行成功接受一个指针，指向服务器返回的数据

返回值：xxx         服务器返回的数据大小（实际堆大小比这个值大1）    ，当  char** ppkey 为空时候，执行成功返回值 为0
ffffff36    服务器不可访问
ffffff38    服务器不可访问2
其他        当前未知
*/
//-------------------online.xilisoft.com
#define  USE_XL_SVR_PROXY_main    1//------------1：访问曦力服务器，0：直接访问aiseesoft服务器
#define  USE_XL_SVR_PROXY         0
#define  XL_BD_SVR_title_save     "http://online.xilisoft.com/getReNewKey.php?a=6" //
#define  XL_BD_SVR_title_load     "http://online.xilisoft.com/getReNewKey.php?a=7" //----------------------main
#define  XL_BD_SVR_volume_load    "http://online.xilisoft.com/getReNewKey.php?a=8" //
#define  XL_BD_SVR_volume_save    "http://online.xilisoft.com/getReNewKey.php?a=9" //
#define  XL_BD_SVR_cert_check     "http://online.xilisoft.com/getReNewKey.php?a=1" //
#define  XL_BD_SVR_cert_load      "http://online.xilisoft.com/getReNewKey.php?a=2" //
#define  XL_BD_cert_sign          "http://online.xilisoft.com/getReNewKey.php?a=3" //
#define  XL_BD_SVR_cache_fail     "http://online.xilisoft.com/getReNewKey.php?a=4" //

EXTERN_C __int32 __cdecl AskNetFileContent_xlsvr_base64_trans(char* svrurl , char* pObj45b4, int sizeObj45b4, char** ppkey);
EXTERN_C __int32 __cdecl AskNetFileContent_xlsvr_title_save(char* svrurl , char* pObj45b4, int sizeObj45b4, char** ppkey)
{
#if(USE_XL_SVR_PROXY)
	return AskNetFileContent_xlsvr_base64_trans(XL_BD_SVR_title_save ,  pObj45b4,  sizeObj45b4,  ppkey);//---------访问XILI服务器------base64加密传输
#else
	return  AskNetFileContent(svrurl ,  pObj45b4,  sizeObj45b4,  ppkey);//-----------访问真实服务器
#endif
}
EXTERN_C __int32 __cdecl AskNetFileContent_xlsvr_title_load(char* svrurl , char* pObj45b4, int sizeObj45b4, char** ppkey)
{
#if(USE_XL_SVR_PROXY_main)
	return   AskNetFileContent_xlsvr_base64_trans(XL_BD_SVR_title_load ,  pObj45b4,  sizeObj45b4,  ppkey);//---------访问XILI服务器------base64加密传输0;//
#else
	return  AskNetFileContent(svrurl ,  pObj45b4,  sizeObj45b4,  ppkey);//-----------访问真实服务器
#endif
}
EXTERN_C __int32 __cdecl AskNetFileContent_xlsvr_volume_load(char* svrurl , char* pObj45b4, int sizeObj45b4, char** ppkey)
{
#if(USE_XL_SVR_PROXY)
	return AskNetFileContent_xlsvr_base64_trans(XL_BD_SVR_volume_load ,  pObj45b4,  sizeObj45b4,  ppkey);//---------访问XILI服务器------base64加密传输
#else
	return AskNetFileContent(svrurl ,  pObj45b4,  sizeObj45b4,  ppkey);//-----------访问真实服务器
#endif
}
EXTERN_C __int32 __cdecl AskNetFileContent_xlsvr_volume_save(char* svrurl , char* pObj45b4, int sizeObj45b4, char** ppkey)
{
#if(USE_XL_SVR_PROXY)
	return AskNetFileContent_xlsvr_base64_trans(XL_BD_SVR_volume_save ,  pObj45b4,  sizeObj45b4,  ppkey);//---------访问XILI服务器------base64加密传输
#else
	return AskNetFileContent(svrurl ,  pObj45b4,  sizeObj45b4,  ppkey);//-----------访问真实服务器
#endif
}
EXTERN_C __int32 __cdecl AskNetFileContent_xlsvr_cert_check(char* svrurl , char* pObj45b4, int sizeObj45b4, char** ppkey)
{
#if(USE_XL_SVR_PROXY)
	return AskNetFileContent_xlsvr_base64_trans(XL_BD_SVR_cert_check ,  pObj45b4,  sizeObj45b4,  ppkey);//---------访问XILI服务器------base64加密传输
#else
	return AskNetFileContent(svrurl ,  pObj45b4,  sizeObj45b4,  ppkey);//-----------访问真实服务器
#endif
}
EXTERN_C __int32 __cdecl AskNetFileContent_xlsvr_cert_load(char* svrurl , char* pObj45b4, int sizeObj45b4, char** ppkey)
{
#if(USE_XL_SVR_PROXY)
	return AskNetFileContent_xlsvr_base64_trans(XL_BD_SVR_cert_load ,  pObj45b4,  sizeObj45b4,  ppkey);//---------访问XILI服务器------base64加密传输
#else
	return AskNetFileContent(svrurl ,  pObj45b4,  sizeObj45b4,  ppkey);//-----------访问真实服务器
#endif
}
EXTERN_C __int32 __cdecl AskNetFileContent_xlsvr_cert_sign(char* svrurl , char* pObj45b4, int sizeObj45b4, char** ppkey)
{
#if(USE_XL_SVR_PROXY)
	return AskNetFileContent_xlsvr_base64_trans(XL_BD_SVR_cert_sign ,  pObj45b4,  sizeObj45b4,  ppkey);//---------访问XILI服务器------base64加密传输
#else
	return AskNetFileContent(svrurl ,  pObj45b4,  sizeObj45b4,  ppkey);//-----------访问真实服务器
#endif
}
EXTERN_C __int32 __cdecl AskNetFileContent_xlsvr_cache_fail(char* svrurl , char* pObj45b4, int sizeObj45b4, char** ppkey)
{
#if(USE_XL_SVR_PROXY)
	return AskNetFileContent_xlsvr_base64_trans(XL_BD_SVR_cache_fail ,  pObj45b4,  sizeObj45b4,  ppkey);//---------访问XILI服务器------base64加密传输
#else
	return AskNetFileContent(svrurl ,  pObj45b4,  sizeObj45b4,  ppkey);//-----------访问真实服务器
#endif
}

EXTERN_C __int32 __cdecl AskNetFileContent_fake(char* svrurl , char* pObj45b4, int sizeObj45b4, char** ppkey)
{

	return 0xffffff38;//--------- 不可访问
}


//define    curl   --begin
typedef void CURL;
typedef enum {
CURLE_OK = 0,
CURLE_UNSUPPORTED_PROTOCOL,    /* 1 */
CURLE_FAILED_INIT,             /* 2 */
CURLE_URL_MALFORMAT,           /* 3 */
CURLE_OBSOLETE4,               /* 4 - NOT USED */
CURLE_COULDNT_RESOLVE_PROXY,   /* 5 */
CURLE_COULDNT_RESOLVE_HOST,    /* 6 */
CURLE_COULDNT_CONNECT,         /* 7 */
CURLE_FTP_WEIRD_SERVER_REPLY,  /* 8 */
CURLE_REMOTE_ACCESS_DENIED,    /* 9 a service was denied by the server
due to lack of access - when login fails
this is not returned. */
CURLE_OBSOLETE10,              /* 10 - NOT USED */
CURLE_FTP_WEIRD_PASS_REPLY,    /* 11 */
CURLE_OBSOLETE12,              /* 12 - NOT USED */
CURLE_FTP_WEIRD_PASV_REPLY,    /* 13 */
CURLE_FTP_WEIRD_227_FORMAT,    /* 14 */
CURLE_FTP_CANT_GET_HOST,       /* 15 */
CURLE_OBSOLETE16,              /* 16 - NOT USED */
CURLE_FTP_COULDNT_SET_TYPE,    /* 17 */
CURLE_PARTIAL_FILE,            /* 18 */
CURLE_FTP_COULDNT_RETR_FILE,   /* 19 */
CURLE_OBSOLETE20,              /* 20 - NOT USED */
CURLE_QUOTE_ERROR,             /* 21 - quote command failure */
CURLE_HTTP_RETURNED_ERROR,     /* 22 */
CURLE_WRITE_ERROR,             /* 23 */
CURLE_OBSOLETE24,              /* 24 - NOT USED */
CURLE_UPLOAD_FAILED,           /* 25 - failed upload "command" */
CURLE_READ_ERROR,              /* 26 - couldn't open/read from file */
CURLE_OUT_OF_MEMORY,           /* 27 */
/* Note: CURLE_OUT_OF_MEMORY may sometimes indicate a conversion error
instead of a memory allocation error if CURL_DOES_CONVERSIONS
is defined
*/
CURLE_OPERATION_TIMEDOUT,      /* 28 - the timeout time was reached */
CURLE_OBSOLETE29,              /* 29 - NOT USED */
CURLE_FTP_PORT_FAILED,         /* 30 - FTP PORT operation failed */
CURLE_FTP_COULDNT_USE_REST,    /* 31 - the REST command failed */
CURLE_OBSOLETE32,              /* 32 - NOT USED */
CURLE_RANGE_ERROR,             /* 33 - RANGE "command" didn't work */
CURLE_HTTP_POST_ERROR,         /* 34 */
CURLE_SSL_CONNECT_ERROR,       /* 35 - wrong when connecting with SSL */
CURLE_BAD_DOWNLOAD_RESUME,     /* 36 - couldn't resume download */
CURLE_FILE_COULDNT_READ_FILE,  /* 37 */
CURLE_LDAP_CANNOT_BIND,        /* 38 */
CURLE_LDAP_SEARCH_FAILED,      /* 39 */
CURLE_OBSOLETE40,              /* 40 - NOT USED */
CURLE_FUNCTION_NOT_FOUND,      /* 41 */
CURLE_ABORTED_BY_CALLBACK,     /* 42 */
CURLE_BAD_FUNCTION_ARGUMENT,   /* 43 */
CURLE_OBSOLETE44,              /* 44 - NOT USED */
CURLE_INTERFACE_FAILED,        /* 45 - CURLOPT_INTERFACE failed */
CURLE_OBSOLETE46,              /* 46 - NOT USED */
CURLE_TOO_MANY_REDIRECTS ,     /* 47 - catch endless re-direct loops */
CURLE_UNKNOWN_TELNET_OPTION,   /* 48 - User specified an unknown option */
CURLE_TELNET_OPTION_SYNTAX ,   /* 49 - Malformed telnet option */
CURLE_OBSOLETE50,              /* 50 - NOT USED */
CURLE_PEER_FAILED_VERIFICATION, /* 51 - peer's certificate or fingerprint
wasn't verified fine */
CURLE_GOT_NOTHING,             /* 52 - when this is a specific error */
CURLE_SSL_ENGINE_NOTFOUND,     /* 53 - SSL crypto engine not found */
CURLE_SSL_ENGINE_SETFAILED,    /* 54 - can not set SSL crypto engine as
default */
CURLE_SEND_ERROR,              /* 55 - failed sending network data */
CURLE_RECV_ERROR,              /* 56 - failure in receiving network data */
CURLE_OBSOLETE57,              /* 57 - NOT IN USE */
CURLE_SSL_CERTPROBLEM,         /* 58 - problem with the local certificate */
CURLE_SSL_CIPHER,              /* 59 - couldn't use specified cipher */
CURLE_SSL_CACERT,              /* 60 - problem with the CA cert (path?) */
CURLE_BAD_CONTENT_ENCODING,    /* 61 - Unrecognized transfer encoding */
CURLE_LDAP_INVALID_URL,        /* 62 - Invalid LDAP URL */
CURLE_FILESIZE_EXCEEDED,       /* 63 - Maximum file size exceeded */
CURLE_USE_SSL_FAILED,          /* 64 - Requested FTP SSL level failed */
CURLE_SEND_FAIL_REWIND,        /* 65 - Sending the data requires a rewind
that failed */
CURLE_SSL_ENGINE_INITFAILED,   /* 66 - failed to initialise ENGINE */
CURLE_LOGIN_DENIED,            /* 67 - user, password or similar was not
accepted and we failed to login */
CURLE_TFTP_NOTFOUND,           /* 68 - file not found on server */
CURLE_TFTP_PERM,               /* 69 - permission problem on server */
CURLE_REMOTE_DISK_FULL,        /* 70 - out of disk space on server */
CURLE_TFTP_ILLEGAL,            /* 71 - Illegal TFTP operation */
CURLE_TFTP_UNKNOWNID,          /* 72 - Unknown transfer ID */
CURLE_REMOTE_FILE_EXISTS,      /* 73 - File already exists */
CURLE_TFTP_NOSUCHUSER,         /* 74 - No such user */
CURLE_CONV_FAILED,             /* 75 - conversion failed */
CURLE_CONV_REQD,               /* 76 - caller must register conversion
callbacks using curl_easy_setopt options
CURLOPT_CONV_FROM_NETWORK_FUNCTION,
CURLOPT_CONV_TO_NETWORK_FUNCTION, and
CURLOPT_CONV_FROM_UTF8_FUNCTION */
CURLE_SSL_CACERT_BADFILE,      /* 77 - could not load CACERT file, missing
or wrong format */
CURLE_REMOTE_FILE_NOT_FOUND,   /* 78 - remote file not found */
CURLE_SSH,                     /* 79 - error from the SSH layer, somewhat
generic so the error message will be of
interest when this has happened */

CURLE_SSL_SHUTDOWN_FAILED,     /* 80 - Failed to shut down the SSL
connection */
CURLE_AGAIN,                   /* 81 - socket is not ready for send/recv,
wait till it's ready and try again (Added
in 7.18.2) */
CURLE_SSL_CRL_BADFILE,         /* 82 - could not load CRL file, missing or
wrong format (Added in 7.19.0) */
CURLE_SSL_ISSUER_ERROR,        /* 83 - Issuer check failed.  (Added in
7.19.0) */
CURLE_FTP_PRET_FAILED,         /* 84 - a PRET command failed */
CURLE_RTSP_CSEQ_ERROR,         /* 85 - mismatch of RTSP CSeq numbers */
CURLE_RTSP_SESSION_ERROR,      /* 86 - mismatch of RTSP Session Identifiers */
CURLE_FTP_BAD_FILE_LIST,       /* 87 - unable to parse FTP file list */
CURLE_CHUNK_FAILED,            /* 88 - chunk callback reported error */

CURL_LAST /* never use! */
} CURLcode;
#ifdef CURL_ISOCPP
#define CINIT(name,type,number) CURLOPT_ ## name = CURLOPTTYPE_ ## type + number
#else
/* The macro "##" is ISO C, we assume pre-ISO C doesn't support it. */
#define LONG          CURLOPTTYPE_LONG
#define OBJECTPOINT   CURLOPTTYPE_OBJECTPOINT
#define FUNCTIONPOINT CURLOPTTYPE_FUNCTIONPOINT
#define OFF_T         CURLOPTTYPE_OFF_T
#define CINIT(name,type,number) CURLOPT_/**/name = type + number
#endif
typedef enum {

// 	  CINIT(URL,  OBJECTPOINT, 2),
// 	  CURLOPT_LASTENTRY
} CURLoption;
#define CURLINFO_STRING   0x100000
#define CURLINFO_LONG     0x200000
#define CURLINFO_DOUBLE   0x300000
#define CURLINFO_SLIST    0x400000
#define CURLINFO_MASK     0x0fffff
#define CURLINFO_TYPEMASK 0xf00000
typedef enum {
	CURLINFO_NONE, /* first, never use this */
	CURLINFO_EFFECTIVE_URL    = CURLINFO_STRING + 1,
	CURLINFO_RESPONSE_CODE    = CURLINFO_LONG   + 2,
	CURLINFO_TOTAL_TIME       = CURLINFO_DOUBLE + 3,
	CURLINFO_NAMELOOKUP_TIME  = CURLINFO_DOUBLE + 4,
	CURLINFO_CONNECT_TIME     = CURLINFO_DOUBLE + 5,
	CURLINFO_PRETRANSFER_TIME = CURLINFO_DOUBLE + 6,
	CURLINFO_SIZE_UPLOAD      = CURLINFO_DOUBLE + 7,
	CURLINFO_SIZE_DOWNLOAD    = CURLINFO_DOUBLE + 8,
	CURLINFO_SPEED_DOWNLOAD   = CURLINFO_DOUBLE + 9,
	CURLINFO_SPEED_UPLOAD     = CURLINFO_DOUBLE + 10,
	CURLINFO_HEADER_SIZE      = CURLINFO_LONG   + 11,
	CURLINFO_REQUEST_SIZE     = CURLINFO_LONG   + 12,
	CURLINFO_SSL_VERIFYRESULT = CURLINFO_LONG   + 13,
	CURLINFO_FILETIME         = CURLINFO_LONG   + 14,
	CURLINFO_CONTENT_LENGTH_DOWNLOAD   = CURLINFO_DOUBLE + 15,
	CURLINFO_CONTENT_LENGTH_UPLOAD     = CURLINFO_DOUBLE + 16,
	CURLINFO_STARTTRANSFER_TIME = CURLINFO_DOUBLE + 17,
	CURLINFO_CONTENT_TYPE     = CURLINFO_STRING + 18,
	CURLINFO_REDIRECT_TIME    = CURLINFO_DOUBLE + 19,
	CURLINFO_REDIRECT_COUNT   = CURLINFO_LONG   + 20,
	CURLINFO_PRIVATE          = CURLINFO_STRING + 21,
	CURLINFO_HTTP_CONNECTCODE = CURLINFO_LONG   + 22,
	CURLINFO_HTTPAUTH_AVAIL   = CURLINFO_LONG   + 23,
	CURLINFO_PROXYAUTH_AVAIL  = CURLINFO_LONG   + 24,
	CURLINFO_OS_ERRNO         = CURLINFO_LONG   + 25,
	CURLINFO_NUM_CONNECTS     = CURLINFO_LONG   + 26,
	CURLINFO_SSL_ENGINES      = CURLINFO_SLIST  + 27,
	CURLINFO_COOKIELIST       = CURLINFO_SLIST  + 28,
	CURLINFO_LASTSOCKET       = CURLINFO_LONG   + 29,
	CURLINFO_FTP_ENTRY_PATH   = CURLINFO_STRING + 30,
	CURLINFO_REDIRECT_URL     = CURLINFO_STRING + 31,
	CURLINFO_PRIMARY_IP       = CURLINFO_STRING + 32,
	CURLINFO_APPCONNECT_TIME  = CURLINFO_DOUBLE + 33,
	CURLINFO_CERTINFO         = CURLINFO_SLIST  + 34,
	CURLINFO_CONDITION_UNMET  = CURLINFO_LONG   + 35,
	CURLINFO_RTSP_SESSION_ID  = CURLINFO_STRING + 36,
	CURLINFO_RTSP_CLIENT_CSEQ = CURLINFO_LONG   + 37,
	CURLINFO_RTSP_SERVER_CSEQ = CURLINFO_LONG   + 38,
	CURLINFO_RTSP_CSEQ_RECV   = CURLINFO_LONG   + 39,
	CURLINFO_PRIMARY_PORT     = CURLINFO_LONG   + 40,
	CURLINFO_LOCAL_IP         = CURLINFO_STRING + 41,
	CURLINFO_LOCAL_PORT       = CURLINFO_LONG   + 42,
	/* Fill in new entries below here! */

	CURLINFO_LASTONE          = 42
} CURLINFO;
#ifdef  __cplusplus
extern "C" {
#endif

	__declspec(dllimport) CURL *curl_easy_init(void);
	__declspec(dllimport) CURLcode curl_easy_setopt(CURL *curl, CURLoption option, ...);
	__declspec(dllimport) CURLcode curl_easy_perform(CURL *curl);
	__declspec(dllimport) void curl_easy_cleanup(CURL *curl);
	__declspec(dllimport) CURLcode curl_easy_getinfo(CURL *curl, CURLINFO info, ...);
}
//define    curl   --end

long writer(char *data, int size, int nmemb, string &content)
{
	long sizes = size * nmemb;
	string temp(data,sizes);
	content += temp; 
	return sizes;
}
//HTTPS方式通过曦力服务器取得KEY内容，及key大小
bool AskXlDBSvr_https(char* pObj45b4, int sizeObj45b4,char* url,char** ppkey,int& nkey)
{

	CURL *curl;
	CURLcode code;
	string content;
	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl,(CURLoption)0x00002712 , url);//CURLOPT_URL
		curl_easy_setopt(curl,(CURLoption)0x00000040 , false);//CURLOPT_VERIFY_SSLPEER 
		curl_easy_setopt(curl,(CURLoption)0x00004e2b , writer);//CURLOPT_WRITEFUNCTION 
		curl_easy_setopt(curl,(CURLoption)0x00002711 , &content);//CURLOPT_WRITEDATA
		curl_easy_setopt(curl,(CURLoption)0x0000271f , pObj45b4);// CURLOPT_POSTFIELDS
		curl_easy_setopt(curl,(CURLoption)0x0000003c , sizeObj45b4);// CURLOPT_POSTFIELDSIZE
		code = curl_easy_perform(curl);
		if(CURLE_OK==code)
		{
			long retcode = 0;
			code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE , &retcode); 
			if ( (code == CURLE_OK) && retcode == 200 )
			{
				int keybase64len= content.length();
				char* pkeytmp= new char[keybase64len];
				if(pkeytmp)
				{
					memcpy(pkeytmp,content.c_str(),content.length());
					*ppkey= pkeytmp;
					nkey= keybase64len;
					curl_easy_cleanup(curl);
					return true;
				}//else nothing
			}//else nothing

		}//else nothing
	}//else nothing

	curl_easy_cleanup(curl);
	return false;
}

bool AskXlDBSvr(char* pObj45b4, int sizeObj45b4,char* url,char** ppkey,int& nkey)
{
	//return  AskXlDBSvr_http(pObj45b4, sizeObj45b4, url, port, ppkey,nkey);
	return  AskXlDBSvr_https(pObj45b4, sizeObj45b4, url,  ppkey,nkey);
}

bool  base64EncodeBlock(const char* hexdata,int hexlen, char** pbase64,int& outstrlen,int flags)
{
	if(!hexdata || hexlen <=0  || !pbase64)
		return false;
	int strlen= Base64EncodeGetRequiredLength(hexlen,flags);
	//int strlenreal= strlen;
	char* pbase64tmp= new char [strlen];
	if(pbase64tmp)
	{
		if(Base64Encode((PUCHAR)hexdata,hexlen,pbase64tmp, &strlen,flags))
		{
			if(strlen>0)
			{
				*pbase64= pbase64tmp;
				outstrlen= strlen;
				return true;

			}//else nothing
		}//else nothing
	}//else nothing
	return false;
}
bool base64DecodeBlock(const char* base64str,int strlen, char** phex,int& outhexlen,int flags)
{
	if(!base64str || strlen <=0  || !phex)
		return false;
	int hexlen= Base64DecodeGetRequiredLength(strlen);
	//int hexlenreal= hexlen;
	char* phextmp= new char [hexlen];
	if(phextmp)
	{
		if(Base64Decode((PCHAR)base64str,strlen,(PUCHAR)phextmp,&hexlen))
		{
			if(hexlen>0)
			{
				*phex= phextmp;
				outhexlen= hexlen;
				return true;

			}//else nothing
		}//else nothing
	}//else nothing
	return false;
}


//数据传输编码为BASE64
EXTERN_C __int32 __cdecl AskNetFileContent_xlsvr_base64_trans(char* svrurl , char* pObj45b4, int sizeObj45b4, char** ppkey)
{
	int nkey= 0;
	*ppkey= 0;


	char* pbase64= 0;
	int strlen= 0;
	if(true== base64EncodeBlock(pObj45b4,sizeObj45b4,&pbase64,strlen,ATL_BASE64_FLAG_NOCRLF))
	{
		char* pkeybase64buf= 0;
		int keybase64len= 0;
		if(true== AskXlDBSvr(pbase64,strlen,svrurl,&pkeybase64buf,keybase64len))
		{

			char* pkeyhexbuf= 0;
			int keyhexlen= 0;
			if(true== base64DecodeBlock(pkeybase64buf,keybase64len,&pkeyhexbuf,keyhexlen,ATL_BASE64_FLAG_NOCRLF))
			{
				*ppkey= pkeyhexbuf;
				nkey= keyhexlen;
			}//else nothing
		}//else nothing
		delete []pkeybase64buf;
	}//else nothing

	delete []pbase64;

	return nkey;
}





//数据块解密，一次性解密0x1800字节
void DeCrypt3SecData2(PVOID pdata)
{
	int pdecrypt_func= *(int*)((char *)pdata+0x1834);//解密函数
	__asm
	{
		push   pdata;
		call   pdecrypt_func      ; 
		add    esp,  4h;
	}
	for(int i=0;i<32;i++)
	{
		((byte*)pdata+0x20)[i*192] &= 0x3f;
	}
}
void InitObj1848(void* key,PVOID pObj,int vertpye )
{	
	INT32 Head[0x20/4]= {
		0x00000000,0x00000000,0x00000000,0x00000000,
		0x00000000,0x00000000,0x00000000,0x00001800
	};
	INT32 Tail[0x28/4]= {
		0x00000001,0x00000000,0x00000000,0x00000000,
		0x00000000,0x00000000,0x00000000,0xffffffff,
		0x00000000,0x00000000
	};
	if(vertpye>0x0a)//type-18-12-17
		Tail[5]= (int)decryptv2;
	else//type--1--4-
		Tail[5]=  (int)decryptv1;

	memcpy(&Tail[1],key,16);

	memcpy(pObj,Head,0x20);
	memcpy((char*)pObj+0x20+0x1800,Tail,0x28);

}
PVOID InitObj8d24(char* disc);

//确认解密算法函数
// void GenAlgType(char* pObj8d24)
// {
// 	int tpye= *(int*)((char *)pObj8d24+0x10-0x4);
// 	if(tpye>0x0a)//type--1--4--18-12-17
// 		pdecrypt_func=(int)ccf_decrypt;
// 	else
// 		pdecrypt_func= (int)sub_100070F0;
// }



void FillKeyInObj8d24(char* pObj8d24)
{
	__asm
	{
		push    ecx
		mov     ecx, pObj8d24;
		mov     dword ptr [ecx+8CF8h], offset sub_10004910 ; --- old sub_100049C0
		mov     dword ptr [ecx+8CFCh], offset sub_10004920 ; --- old sub_100049D0
		mov     dword ptr [ecx+8D00h], offset sub_10004970 ; --- old sub_10004A00
		mov     dword ptr [ecx+8D1Ch], offset sub_10004A50 ; --- old sub_10004A30
		call    createkeyfile_wraper2
		pop     ecx

	}
	//GenAlgType((char*)pObj8d24);
}
PVOID InitObj8d24(char* disc)
{

	__asm
	{
		mov eax,disc;
		push eax;
		call    new8d24Obj;
		add esp,4;
	}
}

//以盘符为输入，得到解密KEY
bool  GetKey(char* disc,char* outkey,int& vertpye )//v1,v2---OK
{
	__try
	{
		vertpye= -1;
		char* pObj8d24= (char*)InitObj8d24(disc);
		//_CrtDumpMemoryLeaks();
		if(pObj8d24)
		{
			FillKeyInObj8d24(pObj8d24);
			//_CrtDumpMemoryLeaks();
			if(2==*(int*)(pObj8d24+0x40-4))
			{
				memcpy(outkey,pObj8d24+0x50,0x10);//key
				vertpye= *(int*)((char *)pObj8d24+0x10-0x4);//来自:\AACS\MKB_RO.inf(或者MKB_RW.inf)文件偏移0x8的4个字节
				delete [](char*)(*(int*)pObj8d24);//盘符
				delete [](char*)(*(int*)(pObj8d24+0x30+0x4));//0x000100000 大小文件内容
				delete [](char*)(*(int*)(pObj8d24+0x4));// 0x0010000  大小块
				delete []pObj8d24;
				//_CrtDumpMemoryLeaks();
				return true;
			}
			delete [](char*)(*(int*)pObj8d24);//盘符
			delete [](char*)(*(int*)(pObj8d24+0x30+0x4));//0x000100000 大小文件内容
			delete [](char*)(*(int*)(pObj8d24+0x4));// 0x0010000  大小块
			delete []pObj8d24;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		printf("except\n");
		return false;
	}
	return false;
}

//读取指定长度的密文数据块
inline bool	fread_block(char *buffer, int nblockSize,__int64 iblock,long& nread,bool& bend, int fh )
{
	bool bSuc= false;

	nread=0;
	bend=false;

	if(-1L==_lseeki64((int)fh,0,SEEK_END))
		return false;
	__int64 endfp= _telli64((int)fh);


	if(-1L!=_lseeki64((int)fh,iblock*nblockSize,SEEK_SET))
	{
		int premode=  _setmode ((int)fh,_O_BINARY);
		int nreadtmp=_read((int) fh, buffer, nblockSize );
		_setmode ((int)fh,premode);

		//size_t nreadtmp= fread(buffer, 1, nblockSize, stream );
		nread= nreadtmp;
		bend= false;
		bSuc= true;
		if(endfp==_telli64((int)fh))
			bend= true;
		else
			bend= false;
		bSuc= true;
	}//else nothing

	return bSuc;
}


size_t fread_xl(char* key, void *buffer, size_t size, size_t count, int fh ,int vertpye )
{
#define BYTE_3SECTOR (2048*3)//0x1800

	if(-1== vertpye)
		return 0;

	long nuserout= size*count;
	if(nuserout<=0)
		return 0;
	__int64 curfp= _telli64((int)fh);
	if(-1L==_lseeki64((int)fh,0,SEEK_END))
		return 0;
	__int64 endfp= _telli64((int)fh);
	if(-1L==_lseeki64((int)fh,curfp,SEEK_SET))
		return 0;
	if(curfp>endfp)
		return 0;
	if(endfp%BYTE_3SECTOR)
		return 0;
	if(curfp==endfp)
		return 0;

	bool bSuc= true;

	__int64 offsetStart= curfp;
	__int64 offsetEnd= curfp+nuserout;
	__int64 indexMax= endfp/BYTE_3SECTOR;
	__int64 indexStart= offsetStart/BYTE_3SECTOR;
	__int64 indexEnd= offsetEnd/BYTE_3SECTOR;
	int nheadUse= 0;
	int ntailUse= 0;
	if(indexStart== indexEnd)
	{	
		nheadUse= (int)nuserout;
	}else
	{
		nheadUse= BYTE_3SECTOR-(offsetStart-indexStart*BYTE_3SECTOR);
		ntailUse= offsetEnd-indexEnd*BYTE_3SECTOR;
	}
//
	void* pData= new char[0x1848];
	InitObj1848(key,pData,vertpye );

	long nReadOut= 0;
	int offsetCopy= 0;
	for (__int64 i=indexStart;i<indexEnd+1;i++)
	{
		if(offsetCopy>=nuserout)
			break;
		bool bread= false;
		long nread= 0;
		bool bend= false;
		if(i==indexStart)
		{
			if(true==fread_block((char*)pData+0x20,BYTE_3SECTOR,i,nread,bend,fh))
			{
				nReadOut+=nread;
			}else
			{
				assert(0);
				bSuc= false;
				break;
			}
			DeCrypt3SecData2(pData);
			memcpy((char*)buffer+offsetCopy,(char*)pData+0x20+(offsetStart-indexStart*BYTE_3SECTOR),nheadUse);
			offsetCopy+=nheadUse;
			if(bend)
				break;
			//else nothing
		}else if(i!=indexEnd)
		{
			if(true==fread_block((char*)pData+0x20,BYTE_3SECTOR,i,nread,bend,fh))
			{
				nReadOut+=nread;
			}else
			{
				assert(0);
				bSuc= false;
				break;
			}
			DeCrypt3SecData2(pData);
			memcpy((char*)buffer+offsetCopy,(char*)pData+0x20,BYTE_3SECTOR);
			offsetCopy+=BYTE_3SECTOR;
			if(bend)
				break;
			//else nothing
		}else
		{
			if(true==fread_block((char*)pData+0x20,BYTE_3SECTOR,i,nread,bend,fh))
			{
				nReadOut+=nread;
			}else
			{
				assert(0);
				bSuc= false;
				break;
			}
			DeCrypt3SecData2(pData);
			memcpy((char*)buffer+offsetCopy,(char*)pData+0x20,ntailUse);
			offsetCopy+=ntailUse;
			if(bend)
				break;
			//else nothing
		}
	}

	if(true==bSuc)
	{
		_lseeki64((int)fh,curfp+offsetCopy,SEEK_SET);
	}else
	{
		_lseeki64((int)fh,curfp,SEEK_SET);
	}

	delete []pData;
	return offsetCopy;

}

