/*���ļ���������BMPͼƬ ����ʾ��fb��*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include <fb_bmp.h>
#include "bmp.h"
#include <fb.h>
#include <debug.h>
#include <config.h>
#include <string.h>





/******************************************************************* 
*�������: *path Ҫ������bmp�ļ���pathname
*�������:��ȷ����0 ����ȷ����-1
*��������:�ж��Ƿ�ΪbmpͼƬ
*
*/
int is_bmp(char *pathname)
{
	int fd = -1;
	int ret = -1;
	unsigned char buf[2]={0};
	
	/*��һ�� ��bmp�ļ�*/
	fd = open(pathname, O_RDONLY);
	if(fd < 0 )
	{
		debug(stderr, "open %s is fail \n",pathname);
		ret = -1;
		goto close;
		
	}
	debug("open is success ! \n");

	/*�ڶ���:��ȡ�ļ�*/
	ret = read(fd, buf, 2);
	if (ret != 2)
	{
		fprintf(stderr, "read %s is fail \n",pathname);
		ret = -1;
		goto close;
	}	
	debug("read is success ! \n");

	/*������:�ж��Ƿ�Ϊbmp�ļ�*/
	if ((buf[0] == 'B') && (buf[1] == 'M'))
	{
		
		ret = 0 ;
		goto close;
	}
	else
	{
		fprintf(stderr, "%s is not a real bmp file \n",pathname);
		ret = -1;
		goto close;
	}

close:
	close(fd);
	return ret;

	return 0;

}



/******************************************************************* 
*�������: *path Ҫ������bmp�ļ���pathname
*�������:��ȷ����0 ����ȷ����-1
*��������:����bmp�ļ� �����������������ݶ���bmp_buf������
*
***************************************************************************/
static int bmp_analyse(struct pic_info *pic)
{
	int fd = -1;
	int res = -1;
	int len = -1;
	BitMapFileHeader fHeader;
	BitMapInforHeader infoHeader;
	
	/*��һ�� ��bmp�ļ�*/
	fd = open(pic->pathname, O_RDONLY);
	if(fd < 0 )
	{
		fprintf(stderr, "open %s is fail \n", pic->pathname);
		return -1;
	}

	/*�ڶ���:��ȡ�ļ�*/
	res = read(fd, &bfType, 2);
	debug("------%c%c--------\n",*((unsigned char *)&bfType),*((unsigned char *)&bfType+1));


	res = read(fd, &fHeader, sizeof(fHeader));
	debug("The size is %ld Byte \n",fHeader.bfSize);
	debug("The bfOffBits is %ld \n",fHeader.bfOffBits);
	
	res = read(fd, &infoHeader, sizeof(infoHeader));
	debug("The resolution is %ld * %ld \n", infoHeader.biWidth, infoHeader.biHeight);
	debug("The biBitCount is %d  \n", infoHeader.biBitCount);

	memset(rgb_buf, 0, sizeof(rgb_buf));
	lseek(fd, fHeader.bfOffBits, SEEK_SET );
	len = infoHeader.biWidth * infoHeader.biHeight * (infoHeader.biBitCount/8);
	debug("len =  %ld Byte \n",len);
	res = read(fd, pic->pdata, len);
	debug("res =  %d  \n", res);

	// ͼƬ�������
	pic->hight	 = infoHeader.biHeight;
	pic->width	 = infoHeader.biWidth ;
	pic->bpp  	 = infoHeader.biBitCount;
//	pic->pdata	 = bmp_buf;

	//lcd_draw_image(bmp_buf);
	lcd_draw_image3(pic);
	
	close(fd);

	return 0;

}

/******************************************************************* 
*
*�������: *path Ҫ������bmp�ļ���pathname
*�������:��ȷ����0 ����ȷ����-1
*��������:��ʾbmpͼƬ
*
***************************************************************************/

int display_bmp(const char *pathname)
{

	struct pic_info pic;
	
	if(is_bmp(pathname))
	{
		printf("this file is not a jpg file! open fail!!! \n");
		return -1;
	}

	pic.pathname = pathname;
	pic.pdata = rgb_buf;

	return bmp_analyse(&pic);

}













































