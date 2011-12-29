#ifndef _CONVERT_H_
#define _CONVERT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////////////////////////////
// 사용자가 호출하는 함수 
//	nFlag == 1 : 완성형 -> 조합형
//	nFlag == 0 : 조합형 -> 완성형
void	han_conv(int nFlag, char* strSrc, char* strDest);

// 완성형을 조합형으로 변환하고 싶을 때, 사용 예 
//	char buf[16];
//	han_conv(1, "한글코드변환", buf);
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
