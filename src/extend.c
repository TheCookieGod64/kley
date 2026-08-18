#include "kley.h"

int cmd_extend(int argc, char **argv){
    if(!file_exists(HOSTS_BACKUP)){ printf(YELLOW "⚠  Kley mode is not active. Use 'kley on <minutes>' instead." RESET "\n"); return 1; }
    if(argc==0){ fprintf(stderr, RED "❌ Usage: kley extend <minutes>" RESET "\n"); return 1; }
    char *t=argv[0]; int isnum=1; for(char *p=t; *p; p++) if(!isdigit((unsigned char)*p)) isnum=0;
    if(!isnum){ fprintf(stderr, RED "❌ Usage: kley extend <minutes>" RESET "\n"); return 1; }
    cancel_timeout();
    long mins=atol(t); long sec=mins*60; long at=time(NULL)+sec;
    FILE *tf=fopen(timeout_file,"w"); if(tf){ fprintf(tf,"%ld\n",at); fclose(tf); }
    pid_t pid=fork();
    if(pid==0){ sleep(sec); if(file_exists(HOSTS_BACKUP)) execlp("kley","kley","off","--auto",(char*)NULL); exit(0); }
    else if(pid>0){ FILE *pf=fopen(timeout_pid_file,"w"); if(pf){ fprintf(pf,"%d\n",pid); fclose(pf); } }
    printf(GREEN "✅ Auto-off rescheduled to %ld minute(s) from now" RESET "\n", mins);
    notify_send("normal","dialog-information","⏰ Timer Extended","Auto-off rescheduled",3000);
    return 0;
}
