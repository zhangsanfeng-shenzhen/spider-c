#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

int node_find_str(char *src,char *find)
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

node_t *node_init(void)
{
	node_t *s;

	s = (node_t *) malloc(sizeof(node_t));
	if (NULL == s) {
		printf("error\n");
		return NULL;
	}
	memset(s->str,'\0',sizeof(s->str));
	s->next = NULL;

	return s;
}

node_t *node_add(node_t * src, char *str)
{
	node_t *add;

	add = (node_t *) malloc(sizeof(node_t));
	if (NULL == add) {
		printf("error\n");
		return NULL;
	}
	strcpy(add->str,str);
	add->next = src->next;
	src->next = add;
	return src;
}

node_t *node_insert(node_t * src, int index, char *str)
{
	node_t *p, *add;
	int i;

	p = src->next;
	i = 0;
	while (NULL != p && i < index - 1) {
		i++;
		p = p->next;
	}

	add = (node_t *) malloc(sizeof(node_t));
	if (NULL == add) {
		printf("error\n");
		return NULL;
	}
	strcpy(add->str,str);
	add->next = p->next;
	p->next = add;

	return src;
}

void node_delete_all(node_t * src)
{
	node_t *p, *tmp;

	p = src->next;
	src->next = NULL;
	while (p != NULL) {
		tmp = p->next;
		free(p);
		p = tmp;
	}
}

node_t *node_delete(node_t * src)
{
	node_t *p;

	p = src->next;
	if (src->next == NULL){
		return src;
	}
	src->next = src->next->next;
	free(p);

	return src;
}

void node_print(node_t * src)
{
	while (NULL != src->next) {
		printf("%s\n", src->next->str);
		src->next = src->next->next;
	}
}

int node_is_exist(node_t *src, char *str)
{
	if (src == NULL || str == NULL){
		printf("error");
		return -1;
	}

	node_t *p;
	p = src->next;
	while (NULL != p) {
		if (node_find_str(p->str,str) == 0){
			return 0;
		}
		p = p->next;
	}
	return -1;
}

int node_count(node_t * src)
{
	int count = 0;
	node_t *p;

	p = src->next;
	while (NULL != p) {
		count++;
		p = p->next;
	}
	return count;
}

/*
int main()
{
	int i;
	node_t *src;
	char dst[1024];

	src = node_init();
	for (i = 0; i < 20; i++) {
		memset(dst,'\0',sizeof(dst));
		sprintf(dst,"%d",i*10);
		src = node_add(src, dst);
	}
	memset(dst,'\0',sizeof(dst));
	sprintf(dst,"%d",2000);
	src = node_insert(src, 3, dst);
		node_delete(src);
	node_print(src);


	return 1;

}
*/