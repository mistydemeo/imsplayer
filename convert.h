#ifndef _CONVERT_H_
#define _CONVERT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////////////////////////////
// ����ڰ� ȣ���ϴ� �Լ� 
//	nFlag == 1 : �ϼ��� -> ������
//	nFlag == 0 : ������ -> �ϼ���
void	han_conv(int nFlag, char* strSrc, char* strDest);

// �ϼ����� ���������� ��ȯ�ϰ� ���� ��, ��� �� 
//	char buf[16];
//	han_conv(1, "�ѱ��ڵ庯ȯ", buf);
unsigned int 	Binary(unsigned int val, unsigned int range);
void 		TG2KS(unsigned char *c1, unsigned char *c2);
void 		KS2TG(unsigned char *c1, unsigned char *c2);

#define MAX_KSCODE      2350   	
#define SINGLENUM       51           
#define KS              1
#define TG              0
#define SPACE           0x20 ;

#ifdef __cplusplus
}
#endif

#endif	//_CONVERT_H_
