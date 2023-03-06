#ifndef CITE_H
#define CITE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>


#include "common.h"

#define IRC_IMPLEMENTATION
#include "irc.h"

#define INFO_IMPLEMENTATION
#include "info.h"

#define STRUTIL_IMPLEMENTATION
#include "strutil.h"

#define UTIL_IMPLEMENTATION
#include "util.h"



typedef struct Cite Cite;

struct Cite {
  size_t bnum;
  size_t scnum;
  size_t ecnum;
  size_t svnum;
  size_t evnum;  
};



Cite *Cite_New(size_t bnum,size_t scnum,size_t ecnum,size_t svnum,size_t evnum);

void Cites_Free(Cite ***cites,size_t *ncites);
void Cite_Append(Cite ***cites,size_t *ncites,Cite *cite);

void Cite_Print(int conn,const char *chn,size_t page,Info **infos,size_t ninfos,Cite *cite);

void Cites_Print(int conn,const char *chn,size_t page,Info **infos,size_t ninfos,Cite **cites,size_t ncites);



#ifdef CITE_IMPLEMENTATION



#endif /* CITE_IMPLEMENTATION */



Cite *Cite_New(size_t bnum,size_t scnum,size_t ecnum,size_t svnum,size_t evnum) {
  Cite *cite=malloc(sizeof(Cite));
  if(cite) {
    cite->bnum=bnum;
    cite->scnum=scnum;
    cite->ecnum=ecnum;
    cite->svnum=svnum;
    cite->evnum=evnum;
  }
  return cite;
}



void Cite_Free(Cite **cite) {
	free(*cite);
	*cite=NULL;
}



void Cites_Free(Cite ***cites,size_t *ncites) {
  for(size_t i=0;i<*ncites;i++) {
    Cite_Free(&(*cites)[i]);
  }
  free(*cites);
  *cites=NULL;
  *ncites=0;
}



void Cite_Append(Cite ***cites,size_t *ncites,Cite *cite) {  *cites=realloc(*cites,sizeof(**cites)*(*ncites+1));
  (*cites)[(*ncites)++]=cite;
}



void Cite_Print(int conn,const char *chn,size_t page,Info **infos,size_t ninfos,Cite *cite) {

  FILE *fp=fopen(BIBLE_FILE,"r");

  char *line=NULL;
  size_t llen=0;
  ssize_t rlen=0;

  char *bname=getbname(infos,ninfos,cite->bnum);
  size_t scnum=cite->scnum;
  size_t ecnum=cite->ecnum?cite->ecnum:scnum;
  size_t svnum=cite->svnum;
  size_t evnum=cite->evnum?cite->evnum:svnum;

	size_t p=1;
	size_t c=0;
	size_t n=0;

  while(c<=4 && (rlen=getline(&line,&llen,fp))>0) {

		rmnl(line);

    char **tokens=NULL;
    size_t ntokens=0;
    tokenize(&tokens,&ntokens,line,"|");

    char *hbname=tokens[0];
    size_t hcnum=atoi(tokens[1]);
    size_t hvnum=atoi(tokens[2]);
    char *htext=tokens[3];

    if(
        strcasecmp(bname,hbname)==0 &&
        ((svnum==0 && evnum==0 && hcnum>=scnum && hcnum<=ecnum) ||
        (hcnum==scnum && hvnum>=svnum && hvnum<=evnum) ||
        (ecnum==0 && evnum==0 && hcnum==scnum && hvnum==svnum))
    ) {

			n++;
    
			if(p==page) privmsg(conn,chn,"(page %zu # %zu) %s %zu:%zu -> %s\r\n",p,n,hbname,hcnum,hvnum,htext);
			c++; if(c>=4) { c=0; p++; }

    }
    
    tokfree(&tokens,&ntokens);

		free(line);
		line=NULL;
		llen=0;
		rlen=0;
  }

	size_t pages=(size_t)ceil((double)n/4);

	raw(conn,"PRIVMSG %s :found %zu in %zu %s\r\n",chn,n,pages,pages<2?"page":"pages");

	free(line);
	line=NULL;
	llen=0;
	rlen=0;

	fclose(fp);
 
}



void Cites_Print(int conn,const char *chn,size_t page,Info **infos,size_t ninfos,Cite **cites,size_t ncites) {
  for(size_t i=0;i<ncites;i++) {
    Cite_Print(conn,chn,page,infos,ninfos,cites[i]);
  }
}



#endif /* CITE_H */


