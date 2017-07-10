#ifndef __BMP_H__
#define __BMP_H__

//�ļ���Ϣ  
unsigned short bfType;//�ļ�����,ASCII�ַ�BM;  
  
//λͼ�ļ�ͷ     
typedef struct{  
    unsigned long bfSize;//�ļ���С  
    unsigned short bfReserved1;//����  
    unsigned short bfReserved2;//����  
    unsigned long bfOffBits;//ͼ��ʼ�����ֽ�ƫ��  
      
}BitMapFileHeader; 

// λͼ��Ϣͷ  
typedef struct{  
    unsigned long biSize;//��Ϣͷ��С  
    unsigned long biWidth;//ͼ����,������Ϊ��λ  
    unsigned long biHeight;//ͼ��߶�,������Ϊ��λ  
    unsigned short biPlanes;//λƽ����,Ϊ1;  
    unsigned short biBitCount;//ÿ����λ��,1,4,8,24  
    unsigned long biCompression;//ѹ������,0Ϊ��ѹ��  
    unsigned long biSizeImage;//ѹ��ͼ���С  
    unsigned long biXpelspermeter;//ˮƽ�ֱ���  
    unsigned long biYpelspermeter;//��ֱ�ֱ���  
    unsigned long biClrUsed;//ʹ�õ�ɫ����  
    unsigned long biClrImportant;//��Ҫɫ����  
}BitMapInforHeader;  












#endif





