#include "ezxml.h"



int main(void) {



	ezxml_t bible = ezxml_parse_file("adb.xml"), book, chap, vers;

	for (book = ezxml_child(bible, "BIBLEBOOK"); book; book = book->next) {
		const char *bname = ezxml_attr(book, "bname");
		printf("%s\n",bname);
	}

	ezxml_free(bible);

	return 0;
}
