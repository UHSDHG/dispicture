/*********************************************************************************
*����image�ļ���������ͼƬ�ļ����ҷ���images�ṹ����
*2017.6.4
*powered by xph
*********************************************************************************/

#include "image_manger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>

#include <fb.h>

#include <debug.h>


image_manger_t images[20];
unsigned int  image_index;


int scan_image(char *basePath)
{	
	DIR *dir;
	 struct dirent *ptr;
	 char base[1000];

	 if ((dir=opendir(basePath)) == NULL)
	 {
		perror("Open dir error...");
		 exit(1);
	 }

	 while ((ptr=readdir(dir)) != NULL)
	 {
		 if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)	  ///current dir OR parrent dir
			 continue;

		switch (ptr->d_type)
		{
			case DT_DIR:			//�ļ���
			
				memset(base,'\0',sizeof(base));
				strcpy(base,basePath);
				strcat(base,"/");
				strcat(base,ptr->d_name);
				scan_image(base);

				break;
			case DT_REG:			//��ͨ�ļ�
			
				memset(base,'\0',sizeof(base));
				strcpy(base,basePath);
				strcat(base,"/");
				strcat(base,ptr->d_name);
				
				debug("d_name:%s\n",base);
			// �������ͨ�ļ�����Ҫ��������д���:
			// ����˼·���� ���ж��Ƿ�������֪��ĳ��ͼƬ��ʽ���������ŵ�images������
			// �����������������
				if (!is_bmp(base))
				{
					strcpy(images[image_index].pathname, base);
					images[image_index].image_type = IMAGE_TYPE_BMP;
				}
				else if (!is_jpeg(base))
				{
					strcpy(images[image_index].pathname, base);
					images[image_index].image_type = IMAGE_TYPE_JPG;
				}
				else if (!is_png(base))
				{
					strcpy(images[image_index].pathname, base);
					images[image_index].image_type = IMAGE_TYPE_PNG;
				}	
				
				break;

			case DT_UNKNOWN:
			default:
				debug("DT_UNKNOWN file !/n");
				break;
		}
		 image_index++;
	 }

	 closedir(dir);
	 return 1;

}

int scan_image2(const char *path)
{
	// �ڱ������еݹ����path�ļ��У�����������ͼƬ��䵽iamges������ȥ
	DIR *dir;
	struct dirent *ptr;
	char base[1000];
	struct stat sta;

	if ((dir = opendir(path)) == NULL)
	{
		perror("Open dir error...");
		exit(1);
	}

	// readdir����ÿ����һ�ξͻ᷵��opendir�򿪵�basepathĿ¼�µ�һ���ļ���ֱ��
	// basepathĿ¼�������ļ���������֮�󣬾ͻ᷵��NULL
	while ((ptr = readdir(dir)) != NULL)
	{
		if(strcmp(ptr->d_name, ".")==0 || strcmp(ptr->d_name, "..")==0)    ///current dir OR parrent dir
			continue;

		// ��lstat����ȡ�ļ����Բ��ж��ļ�����
		memset(base,'\0',sizeof(base));
		strcpy(base,path);
		strcat(base,"/");
		strcat(base,ptr->d_name);
		lstat(base, &sta);

		if (S_ISREG(sta.st_mode))
		{
			//printf("regular file.\n");
			//printf("d_name:%s/%s\n", path, ptr->d_name);
			// �������ͨ�ļ�����Ҫ��������д���:
			// ����˼·���� ���ж��Ƿ�������֪��ĳ��ͼƬ��ʽ���������ŵ�images������
			// �����������������
			if (!is_bmp(base))
			{
				strcpy(images[image_index].pathname, base);
				images[image_index].image_type = IMAGE_TYPE_BMP;
			}
			else if (!is_jpeg(base))
			{
				strcpy(images[image_index].pathname, base);
				images[image_index].image_type = IMAGE_TYPE_JPG;
			}
			else if (!is_png(base))
			{
				strcpy(images[image_index].pathname, base);
				images[image_index].image_type = IMAGE_TYPE_PNG;
			}		
			image_index++;
			
		}
		if (S_ISDIR(sta.st_mode))
		{
			//printf("directory.\n");
			//printf("d_name:%s/%s\n", path, ptr->d_name);
			scan_image2(base);
		}
	}
}

void print_images(void)
{
	int i = 0;
	fprintf(stderr, "\n imagers file = %d \n", image_index);

	for (i = 0; i < image_index ; i ++)
	{	
		fprintf(stderr, "pathname  : %s\n", images[i].pathname);
		fprintf(stderr, "file type : %d \n", images[i].image_type);
	}

}

static void show_image(int index)
{
	debug("index = %d.\n", index);
	switch (images[index].image_type)
	{
		case IMAGE_TYPE_BMP:
			display_bmp(images[index].pathname);		break;
		case IMAGE_TYPE_JPG:
			display_jpg(images[index].pathname);		break;
		case IMAGE_TYPE_PNG:
			display_png(images[index].pathname);		break;
		default:
			break;
	}	
}


// ������ʵ��ͨ������������ͼƬ��ҳ��ʾ
int ts_updown(void)
{
	// ��һ��: �������Ĵ����������
	int fd = -1, ret = -1;
	struct input_event ev;
	int i = 0;					// ������¼��ǰ��ʾ���ǵڼ���ͼƬ
		
	fd = open(DEVICE_TOUCHSCREEN, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return -1;
	}
		
	while (1)
	{
		memset(&ev, 0, sizeof(struct input_event));
		ret = read(fd, &ev, sizeof(struct input_event));
		if (ret != sizeof(struct input_event))
		{
			perror("read");
			close(fd);
			return -1;
		}

		// �ڶ���: ���ݴ�����������ҳ
		if ((ev.type == EV_ABS) && (ev.code == ABS_X))
		{
			// ȷ�������x����
			if ((ev.value >= 0) && (ev.value < TOUCH_WIDTH))
			{
				// �Ϸ�ҳ
				if (i-- <= 1)
				{
					i = image_index;
					debug("i=%d.\n", i);
				}
				
			}
			else if ((ev.value > (COL - TOUCH_WIDTH)) && (ev.value <= COL))
			{
				// �·�ҳ			
				if (i++ >= image_index)
				{
					i = 1;
					debug("i=%d.\n", i);
				}
			}
			else
			{
				// ����ҳ
			}
			show_image(i - 1);
		}
		
			
/*
		printf("-------------------------\n");
		printf("type: %hd\n", ev.type);
		printf("code: %hd\n", ev.code);
		printf("value: %d\n", ev.value);
		printf("\n");
		*/
	}	
	close(fd);

	return 0;
	
}

















