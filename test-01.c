#include <string.h>

#include "ezxml.h"



int main(void) {


	char books[66][256];

	FILE *fin,*fout;

	fin=fopen("books.txt","r");
	for(int i=0;i<66;i++) {
		fgets(books[i],256,fin);
		char *p=strchr(books[i],'\n'); if(p) *p='\0'; 
	}
	fclose(fin);

	fin=fopen("kjv.inf","r");
	fout=fopen("adb.inf","w");
	char line[1024];
	for(int i=0;i<66;i++) {
		fgets(line,1024,fin);
		char *p=strchr(line,'\n'); if(p) *p='\0';
		fprintf(fout,"%s|%s\n",strstr(books[i],"|")+1,strstr(line,"|")+1); 
	}
	fclose(fout);
	fclose(fin);
	



	ezxml_t bible = ezxml_parse_file("adb.xml"), book, chap, vers;

	for (book = ezxml_child(bible, "BIBLEBOOK"); book; book = book->next) {
		for (chap = ezxml_child(book, "CHAPTER"); chap; chap = chap->next) {
			for (vers = ezxml_child(chap, "VERS"); vers; vers = vers->next) {
				const char *bname = ezxml_attr(book, "bname");
				int bnum = atoi(ezxml_attr(book, "bnumber"));
				int cnum = atoi(ezxml_attr(chap, "cnumber"));
				int vnum = atoi(ezxml_attr(vers, "vnumber"));
				const char *text=ezxml_txt(vers);
				printf("%s|%d|%d|%s\n",strstr(books[bnum-1],"|")+1,cnum,vnum,text);
			}
		}
	}

	ezxml_free(bible);

	return 0;
}
