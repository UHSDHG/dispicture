/***************************************************************************
*fb.c ����frambuffer�Ļ������룬����fb�Ĵ� ioctl��ȡ��Ϣ ������fb���Դ���
*
*
*
*****************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <unistd.h>

#include "image.h"


#include <fb.h>
#include <debug.h>
#include <config.h>


unsigned int *pfb = NULL ;
static int fb_fd = -1;  



// �����ص�(x, y)�����Ϊcolor��ɫ
static void lcd_draw_pixel(u32 x, u32 y, u32 color)
{
	*(pfb + COL * y + x) = color;
}

// ��������Ļȫ������һ����ɫcolor
void lcd_draw_background(u32 color)
{
	u32 i, j;
	
	for (j=0; j<ROW; j++)
	{
		for (i=0; i<COL; i++)
		{
			lcd_draw_pixel(i, j, color);
		}
	}
}

// ���ƺ��ߣ���ʼ����Ϊ(x1, y)��(x2, y),��ɫ��color
static void lcd_draw_hline(u32 x1, u32 x2, u32 y, u32 color)
{
	u32 x;
	
	for (x = x1; x<x2; x++)
	{
		lcd_draw_pixel(x, y, color);
	}
}

void lcd_draw_image(unsigned char *pic)
 {
 	unsigned char *p = pic ;
	u32 x, y;
	u32 cnt,a;
	a = 600*902*3 ;
	
	for (y=0; y<600; y++)       //��
	{
		for (x=0; x<902; x++)     //��
		{
			cnt = COL * y + x; //��λ��Ҫд�����ص�
			*(pfb + cnt) = ((p[a + 2 ]<<16) | (p[a + 1 ]<<8) | (p[a + 0 ]<<0));
			a -= 3;
		}
	}

}


void lcd_draw_image1( struct pic_info *pPic)
 {
	 const char *pData = (const char *)pPic->pdata; 	 // ָ��ָ��ͼ������
	 unsigned int cnt = 0, a = 0;
	 unsigned int x, y;
	 
	 debug("image resolution: %d * %d, bpp=%d.\n", 
		 pPic->width, pPic->hight, pPic->bpp);
	 
	 if ((pPic->bpp != 32) && (pPic->bpp != 24))
	 {
		 fprintf(stderr, "BPP %d is not support.\n", pPic->bpp);
		 return;
	 }
	 
	 a = pPic->hight * pPic->width * 3 - 3;
	 for (y=0; y<pPic->hight; y++)
	 {
		 for (x=0; x<pPic->width; x++)
		 { 
			 //cnt��ʾ��ǰ���ص�ı��
			 cnt = COL * y + x;
			 // ��ǰ���ص��Ӧ��ͼ�����ݵ�RGB��Ӧ�÷ֱ���:
			 // pData[cnt+0]  pData[cnt+1]	pData[cnt+2]  
			 // ��ǰ���ص������
			 *(pfb + cnt) = ((pData[a+0]<<16) | (pData[a+1]<<8)| (pData[a+2]<<0)); 
			 //*p = ((pData[cnt+0]<<16) | (pData[cnt+1]<<8)| (pData[cnt+2]<<0)); 
			 a -= 3;
		 }
	 }


}

void lcd_draw_image2( struct pic_info *pPic)
 {
	 const char *pData = (const char *)pPic->pdata; 	 // ָ��ָ��ͼ������
	 unsigned int cnt = 0, a = 0;
	 unsigned int x, y;
	 
	 debug("image resolution: %d * %d, bpp=%d.\n", 
		 pPic->width, pPic->hight, pPic->bpp);
	 
	 if ((pPic->bpp != 32) && (pPic->bpp != 24))
	 {
		 fprintf(stderr, "BPP %d is not support.\n", pPic->bpp);
		 return;
	 }
	 
	 a = 0;
	 for (y=0; y<pPic->hight; y++)
	 {
		 for (x=0; x<pPic->width; x++)
		 { 
			 //cnt��ʾ��ǰ���ص�ı��
			 cnt = COL * y + x;
			 // ��ǰ���ص��Ӧ��ͼ�����ݵ�RGB��Ӧ�÷ֱ���:
			 // pData[cnt+0]  pData[cnt+1]	pData[cnt+2]  
			 // ��ǰ���ص������
			 *(pfb + cnt) = ((pData[a+0]<<16) | (pData[a+1]<<8)| (pData[a+2]<<0)); 
			 //*p = ((pData[cnt+0]<<16) | (pData[cnt+1]<<8)| (pData[cnt+2]<<0)); 
			 a += 3;
		 }
	 }

	printf("\n");

}

void lcd_draw_image3( struct pic_info *pPic)
 {
	 const char *pData = (const char *)pPic->pdata; 	 // ָ��ָ��ͼ������
	 unsigned int cnt = 0, a = 0;
	 unsigned int x, y;
	 
	 debug("image resolution: %d * %d, bpp=%d.\n", 
		 pPic->width, pPic->hight, pPic->bpp);
	 
	 if ((pPic->bpp != 32) && (pPic->bpp != 24))
	 {
		 fprintf(stderr, "BPP %d is not support.\n", pPic->bpp);
		 return;
	 }
	 
	 a = pPic->hight * pPic->width * 3 - 3;
	 for (y=0; y<pPic->hight; y++)
	 {
		 for (x=0; x<pPic->width; x++)
		 { 
			 //cnt��ʾ��ǰ���ص�ı��
			 cnt = COL * y + x;
			 // ��ǰ���ص��Ӧ��ͼ�����ݵ�RGB��Ӧ�÷ֱ���:
			 // pData[cnt+0]  pData[cnt+1]	pData[cnt+2]  
			 // ��ǰ���ص������
			 *(pfb + cnt) = ((pData[a+0]<<0) | (pData[a+1]<<8)| (pData[a+2]<<16)); 
			 a -= 3;
		 }
	 }

}

int fb_open(void)
{
	int ret = -1;
	struct fb_fix_screeninfo finfo = {0};
	struct fb_var_screeninfo vinfo = {0};
	/*���豸*/
	fb_fd = open(FBDEVICE, O_RDWR);
	if(fb_fd<0)
	{
		perror("open");
		return 0;
	}
	debug("open file :"FBDEVICE" success... \n");
	
	/*��ȡӲ���豸��Ϣ*/
	ret = ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo);
	if(ret < 0)
	{
		perror("ioctl");
		return -1;
	}
	debug("smem_start = 0x%x; smem_len = %d \n" , finfo.smem_start, finfo.smem_len);
	
	ret = ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);
	if(ret < 0)
	{
		perror("ioctl");
		return -1;
	}
	debug("xres		= %4d; yres		= %4d \n" , vinfo.xres, vinfo.yres);
	debug("xres_virtual	= %4d; yres_virtual	= %4d \n" , vinfo.xres_virtual, vinfo.yres_virtual);
	debug("xoffset		= %4d; yoffset		= %4d \n" , vinfo.xoffset, vinfo.yoffset);

#if 0	
	/*�޸������е���Ļ�ֱ���*/
	vinfo.xres = COL;
	vinfo.yres = ROW;
	vinfo.xres_virtual = COL;
	vinfo.yres_virtual = ROW*2;
	ret = ioctl(fd, FBIOPUT_VSCREENINFO, &vinfo);
	if(ret < 0)
	{
		perror("ioctl");
		return -1;
	}
	printf("**********************************************\n");
	printf("xres		= %4d; yres		= %4d \n" , vinfo.xres, vinfo.yres);
	printf("xres_virtual	= %4d; yres_virtual	= %4d \n" , vinfo.xres_virtual, vinfo.yres_virtual);
	printf("xoffset		= %4d; yoffset		= %4d \n" , vinfo.xoffset, vinfo.yoffset);
#endif	
	
	/*����mmap*/
	pfb = mmap(NULL, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
	if (NULL == pfb)
	{
		perror("mmap");
		return -1;
	}
	debug("pfb = %p \n", pfb);

	


	return 0 ;
}



int fb_close(void)
{
	close(fb_fd);
	return 0;
}






















