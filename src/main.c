#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "list.h"
#include "sha2.h"

#define SIZE 1024*10
node_t *url_list,*file_list;

char *compile_str(char *src)
{
	char *compile = "href=";
	char *index,*find_index;
	static char a[1024*10];
	int i=0;

	if (src == NULL){
		printf("error\n");
		return NULL;
	}
	while(*src!='\0') {
		index = src;
		find_index = compile;
		while(1) {
			if (*index != *find_index || *find_index == '\0'){
				break;
			}
			index++;
			find_index++;
		}
		if (*find_index == '\0' && *index == '\"'){
			index++;
			while(*index != '\"') {
				a[i] = *index;
				index++;
				i++;
			}
			a[i]='\0';
			return a;
		}
		src++;
	}
	return NULL;
}

int find_str(char *src,char *find)
{
	char *index,*find_index;
	if (src == NULL || find == NULL){
		printf("error\n");
		return -1;
	}
	while(*src!='\0') {
		index = src;
		find_index = find;
		while(1) {
			if (*index != *find_index || *find_index == '\0'){
				break;
			}
			index++;
			find_index++;
		}
		if (*find_index == '\0'){
			return 0;
		}
		src++;
	}
	return -1;
}

void get_url(char *file)
{
    FILE *fp;
	char buff[SIZE];
	char url[SIZE];
	char *str;

    if(file == NULL){
        printf("file is not exist\n");
        return;
    }

	fp = fopen(file,"r");
    if(fp == NULL) {
        printf("The file <%s> can not be opened.\n",file);
        return;
    }

	while(fgets(buff,SIZE,fp)!=NULL) {
		str = compile_str(buff);
		if (str != NULL && find_str(buff,"no05") == 0) {
			if ((find_str(buff,"/post") == 0 && find_str(buff,"shtml") == 0) || find_str(buff,"nextid") == 0){
				memset(url,'\0',sizeof(url));
				if (find_str(buff,"http://bbs.tianya.cn") != 0){
					sprintf(url,"http://bbs.tianya.cn%s",str);
				} else {
					strcpy(url,str);
				}
				if(node_is_exist(url_list,url) != 0) {
					url_list = node_add(url_list, url);
					//printf("buffer is: %s\n",url);
				}
			}
		}
	}

    fclose(fp);
}

char *sha256_str(const char *src)
{
	unsigned char hash[32];
	static char hash_str[128];
	char byte[3];
	int i;

	if (src == NULL){
		return NULL;
	}

	sha256(src, strlen(src), hash);

	memset(hash_str,'\0',sizeof(hash_str));
	for (i=0;i<32;i++ ){
		sprintf(byte,"%02x",hash[i]);
		strcat(hash_str,byte);
	}
	strcat(hash_str,".html");
	return hash_str;
}

void down_file(void)
{
	char cmd[SIZE];
	char *hash,*url;
	int count,i;

	count = node_count(url_list);
	if (count >= 60){
		count /= 2;
	}

	for (i=0; i < count; i++ ){
		url = url_list->next->str;
		hash = sha256_str(url);
		if (access(hash,0)) {
			sprintf(cmd,"aria2c -o %s %s -c -D &",hash,url);
			file_list = node_add(file_list, hash);
			printf("%s\n",cmd);
			system(cmd);
		}
		url_list = node_delete(url_list);
	}
}

int main()
{
	url_list = node_init();
	file_list = node_init();

	system("aria2c -o 1.html http://bbs.tianya.cn/list-no05-1.shtml -c &");
	sleep(1);
	get_url("1.html");
	down_file();
	sleep(3);

	while(1) {
		while(file_list->next != NULL){
			get_url(file_list->next->str);
			node_delete(file_list);
		}
		down_file();
		printf("\n\n\n%d\n\n\n",node_count(url_list));
		sleep(3);
	}
	return 1;
}