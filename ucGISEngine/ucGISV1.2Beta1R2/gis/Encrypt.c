#include "gisbase.h"
char EncryptT[]={46, 47,'0','1','2','3','4','5','6','7',
'8','9',':',';','<','=','>','?','A','B',
'C','D','E','F','G','H','I','J','K','L',
'M','N','O','P','Q','R','S','T','U','V',
'W','X','Y','Z','a','b','c','d','e','f',
'g','h','i','j','k','l','m','n','o','p',
'q','r','s','t','u','v','w','x','y','z'
};
char* Encrypt(char* strIn,char *strOut)
{

	char *pstr;
	char *pstrOut;
	INT8U C1,C2,C3,C4;
	INT16U i,j=0, length=strlen(strIn);
	pstr=strIn;
	pstrOut=strOut;
	for(i=0;i<length;i+=3)
	{
		C1=pstr[i];
		C2=pstr[i+1];
		C3=pstr[i+2];
		*pstrOut++=EncryptT[C1&0x3F];
		*pstrOut++=EncryptT[C2&0x3F];
		*pstrOut++=EncryptT[C3&0x3F];
		C1&=0xC0;C2&=0xC0;C3&=0xC0;
		C1>>=2;C2>>=4;C3>>=6;
		C4=C1|C2|C3;
		*pstrOut++=EncryptT[C4];
	}
	*pstrOut=0;
	pstr=*strOut;
	return pstr;
}

