#include "kley.h"

int cmd_db_info(void){
    char cmd[512]; snprintf(cmd,sizeof(cmd),"grep -q '%s' '%s'", MARKER_START, HOSTS_FILE);
    if(system(cmd)!=0){ printf(YELLOW "⚠  Kley database is NOT installed in %s." RESET "\n" "Install with: " BLUE "kley install-db" RESET "\n", HOSTS_FILE); return 0; }
    printf(BOLD BLUE "📊 KLEY Database Info" RESET "\n");
    char cntcmd[512]; snprintf(cntcmd,sizeof(cntcmd),"sed -n '/%s/,/%s/p' '%s' | grep -c '^[0-9]' || true", MARKER_START, MARKER_END, HOSTS_FILE);
    FILE *pp=popen(cntcmd,"r"); long cnt=0; if(pp){ fscanf(pp,"%ld",&cnt); pclose(pp); }
    char datecmd[512]; snprintf(datecmd,sizeof(datecmd),"grep -A2 '%s' '%s' | grep 'Installed:' | sed 's/# Installed: //' || echo Unknown", MARKER_START, HOSTS_FILE);
    char d[256]="Unknown"; pp=popen(datecmd,"r"); if(pp){ fgets(d,sizeof(d),pp); d[strcspn(d,"\r\n")]=0; pclose(pp); }
    char srccmd[512]; snprintf(srccmd,sizeof(srccmd),"grep -A3 '%s' '%s' | grep 'Source:' | sed 's/# Source: //' || echo Unknown", MARKER_START, HOSTS_FILE);
    char s[512]="Unknown"; pp=popen(srccmd,"r"); if(pp){ fgets(s,sizeof(s),pp); s[strcspn(s,"\r\n")]=0; pclose(pp); }
    FILE *f=fopen(HOSTS_FILE,"r"); long tot=0; if(f){ int c; while((c=fgetc(f))!=EOF) if(c=='\n') tot++; fclose(f); }
    printf(CYAN "Installed on:" RESET "    %s\n" CYAN "Source:" RESET "          %s\n" CYAN "Kley rules:" RESET "      %ld\n" CYAN "Total /etc/hosts:" RESET " %ld\n", d,s,cnt,tot);
    return 0;
}
