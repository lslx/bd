#ifndef __FUNC_H
#define __FUNC_H


//bd ͸�����ܶ����ӿ�
/*
GetKey:  ��ȡ��������Կ
����1��char* disc----------���룬�̷� ��"j:"
����2��char* outkey----------�����16�ֽ���Կ
����3��int& vertype----------�����BD���ܰ汾

����ֵ��ture-----�����Կ�ɹ���false--------�����Կʧ��

*/
bool  GetKey(char* disc,char* outkey,int& vertype );


/*
fread_xl:    ���ܶ�ȡ�������ļ�����
����1��char* key----------���룬KEY
����6��int vertype----------���룬BD���ܰ汾
����������ͬfread
����ֵ���ɹ���ȡ�����ݴ�С����ͬfread����ֵ��
ע��    ������Ϊ��fread��ͬ

*/

size_t fread_xl(char* key, void *buffer, size_t size, size_t count,int fh, int vertype );


#endif//__FUNC_H