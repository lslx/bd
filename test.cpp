#include "stdafx.h"
#include <shlwapi.h>
#include "func.h"
#include "fcntl.h"
#include <Windows.h>
#include <assert.h>
#include <atlenc.h>
#include <stdio.h>
#include <io.h>
#include <string> 
using namespace std;

bool decrypt_newinterface(const char disc[],const char cryptfile[],const char plainfile[])
{
#define BLOCK_SIZE  (0x1800)
	char outkey[16]={0};
	int bdver= 0;
	if(GetKey((char*)disc,outkey,bdver))
	{
		for (int i= 0;i<16;i++)
		{
			printf("%02X",unsigned char(outkey[i]));
		}
		printf("\n");

		int fh_crypt= 0;int fh_plain= 0;

		if( _sopen_s( &fh_crypt, cryptfile,  _O_RDONLY|_O_BINARY, _SH_DENYNO, 0 ) == 0
			&&_sopen_s( &fh_plain,plainfile ,  _O_WRONLY|_O_BINARY|_O_CREAT, _SH_DENYNO, 0 ) == 0 )
		{
			char* crypt= new char[BLOCK_SIZE];
			if(!crypt)
			{
				_close(fh_crypt);
				_close(fh_plain);
				return false;
			}
			_lseeki64(fh_crypt,0,SEEK_END);//fseek(stream_crypt,0,SEEK_END);
			__int64 filesize= _telli64(fh_crypt);//long filesize= ftell(stream_crypt);//
			_lseeki64(fh_crypt,0,SEEK_SET);


			__int64 nblock= filesize/BLOCK_SIZE;
			int lest= filesize%BLOCK_SIZE;
			__int64 i= 0;
			while(i<nblock)
			{
				__int64 fpcur= _telli64(fh_crypt);
				int nread1= fread_xl(outkey,crypt,sizeof(char),BLOCK_SIZE,fh_crypt,bdver);
				_write(fh_plain,crypt,BLOCK_SIZE);
				printf("filesize:%llx,   curfp:%llx curfpreal:%llx per: %d \n",filesize,BLOCK_SIZE*i,fpcur,(i+1)*100/nblock);
				i++;
			}
			if(lest>0)
			{
				int nread1= fread_xl(outkey,crypt,sizeof(char),BLOCK_SIZE,fh_crypt,bdver);
				_write(fh_plain,crypt,BLOCK_SIZE);
				printf("filesize:%llx,   curfp:%llx per: %d \n",filesize,filesize,100);
			}//else nothing
			delete []crypt; 

			_close(fh_crypt);
			_close(fh_plain);
			printf("done!\n");
			return true;

		}else
			printf("open file err!");
	}else
		printf("get key failed!");
	return false;
}

void test_interface_read(char* disc,char* dstpath,char* discname,char* thenum)
{
	char pathsrc[1024]= {0};
	char pathdst[1024]= {0};
	if(discname && thenum)
	{
		strcat(pathsrc,disc);
		strcat(pathsrc,"\\BDMV\\STREAM\\");
		strcat(pathsrc,thenum);
		strcat(pathsrc,".m2ts");


		strcat(pathdst,dstpath);
		strcat(pathdst,discname);
		strcat(pathdst,thenum);
		strcat(pathdst,".m2ts");
	}

	char* cryptfile1= pathsrc;
	char* plainfile1= pathdst;

	decrypt_newinterface(disc,cryptfile1,plainfile1);
}


void test_decrypt_file__2()
{
	int fh;
	long pos;               /* Position of file pointer */
	char buffer[10];

	char filename[]= "I:\\BDMV\\STREAM\\00021.m2ts";
	_sopen_s( &fh, filename, _O_RDONLY|_O_BINARY, _SH_DENYNO, 0 );

	/* Seek the beginning of the file: */
	pos = _lseek( fh, 0L, SEEK_SET );
	if( pos == -1L )
		perror( "_lseek to beginning failed" );
	else
		printf( "Position for beginning of file seek = %ld\n", pos );
	char disc[16]="i:";
	char outkey[16]={0};
	int ver=0;
	if(GetKey((char*)disc,outkey,ver))
	{
		char* crypt= new char[0x1800];
		fread_xl(outkey,crypt,sizeof(char),0x1800,fh,ver);
	
	}



	__asm int 3;



	/* Move file pointer a little */
	_read( fh, buffer, 10 );

	/* Find current position: */
	pos = _lseek( fh, 0L, SEEK_CUR );
	if( pos == -1L )
		perror( "_lseek to current position failed" );
	else
		printf( "Position for current position seek = %ld\n", pos );

	/* Set the end of the file: */
	pos = _lseek( fh, 0L, SEEK_END );
	if( pos == -1L )
		perror( "_lseek to end failed" );
	else
		printf( "Position for end of file seek = %ld\n", pos );

	_close( fh );

}
void test_decrypt_file_3()
{
	char disc[]="g:";
	char dstpath[]= "D:\\bul_ray_disc_file\\";
	
	
	//test_interface_read(disc,dstpath,"suck_","00005");-----非加密
	//test_interface_read(disc,dstpath,"hannah_","00028");
	
	//test_interface_read(disc,dstpath,"sony_","00004");//曦力软件可播放
	//test_interface_read(disc,dstpath,"princeofpersia_","00417");
	
	//***********
	//test_interface_read(disc,dstpath,"toy3_v19_","00066");
	
	//test_interface_read(disc,dstpath,"black_v4","00008"); //不可访问
	//test_interface_read(disc,dstpath,"resid_v19_","00112");//--------zuikuihuoshou
	//test_interface_read(disc,dstpath,"resid_v19_","00116");

	//**test 1
	//test_interface_read(disc,dstpath,"bolt_v12","00187");//11--- 播放器
	//test_interface_read(disc,dstpath,"toy3_v19_","00066");
	//test_interface_read(disc,dstpath,"iron_man_v18_","00250");//
	//test_interface_read(disc,dstpath,"iron_man_v17_","00812");//

	//test_interface_read(disc,dstpath,"crossroads_v18_","00001");
	//test_interface_read(disc,dstpath,"mentu_v4_","00002");
	//test_interface_read(disc,dstpath,"universalsoldier_v16_","00011");
	//test_interface_read(disc,dstpath,"fantastic_v4_","00001");
	//test_interface_read(disc,dstpath,"persia_v19_","00996");
	//test_interface_read(disc,dstpath,"oceans_v4_","00009");//00000,7----------- 播放器
	//test_interface_read(disc,dstpath,"startrek_14_","00000"); //test ok
	//test_interface_read(disc,dstpath,"transformers_15_","00038"); //test ok
	//test_interface_read(disc,dstpath,"MJSON_16_","00011"); //test ok
	//test_interface_read(disc,dstpath,"2012_16_","00535"); //test ok
	//test_interface_read(disc,dstpath,"AVATAR_16_","00002"); //test ok
	//test_interface_read(disc,dstpath,"universalsoldier_v16_","00011");
	//test_interface_read(disc,dstpath,"madman_v16_","00316");//----------
	test_interface_read(disc,dstpath,"alice_v16_","00564");





}
void testGetKey()
{
	char disc[]= "g:";
	char outkey[16]={0};
	int bdver= 0;
	if(true==GetKey((char*)disc,outkey,bdver))
	{
		printf("ok1\n");
		__asm int 3;
		return;
	}
	printf("er1\n");
	__asm int 3;
}





struct stru_INFO
{
	int a;
	int b;
};

void memleaksch()
{
	char* pmem5= new char;
	delete pmem5;
	char* pmem1= new char[10];
	delete [] pmem1;
	struct stru_INFO* pmem2= new struct stru_INFO;
	struct stru_INFO* pmem3= new struct stru_INFO[3];



	_CrtDumpMemoryLeaks();
}
void cfun_main()
{

	//memleaksch();
	testGetKey();
	//test_decrypt_file_3();
}

/*
task:
2.  整理 int3
3.  J:,del

备忘：
1.   MKB V21  BD支持（需要硬件disk）

*/

