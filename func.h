#ifndef __FUNC_H
#define __FUNC_H


//bd 透明解密读，接口
/*
GetKey:  获取蓝光盘密钥
参数1：char* disc----------输入，盘符 如"j:"
参数2：char* outkey----------输出，16字节密钥
参数3：int& vertype----------输出，BD加密版本

返回值：ture-----获得密钥成功，false--------获得密钥失败

*/
bool  GetKey(char* disc,char* outkey,int& vertype );


/*
fread_xl:    解密读取蓝光盘文件内容
参数1：char* key----------输入，KEY
参数6：int vertype----------输入，BD加密版本
其他参数：同fread
返回值：成功读取的内容大小，（同fread返回值）
注：    函数行为与fread类同

*/

size_t fread_xl(char* key, void *buffer, size_t size, size_t count,int fh, int vertype );


#endif//__FUNC_H