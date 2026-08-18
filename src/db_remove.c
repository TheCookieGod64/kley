#include "kley.h"

int cmd_remove_db(void){
    char cmd[512]; snprintf(cmd,sizeof(cmd),"grep -q '%s' '%s'", MARKER_START, HOSTS_FILE);
    if(system(cmd)!=0){ printf(YELLOW "⚠  No kley database entries found in %s." RESET "\n", HOSTS_FILE); return 0; }
    char cntcmd[512]; snprintf(cntcmd,sizeof(cntcmd),"sed -n '/%s/,/%s/p' '%s' | grep -c '^[0-9]' || true", MARKER_START, MARKER_END, HOSTS_FILE);
    FILE *pp=popen(cntcmd,"r"); long cnt=0; if(pp){ fscanf(pp,"%ld",&cnt); pclose(pp); }
    printf(YELLOW "⚠  This will remove all %ld kley entries from %s." RESET "\n", cnt, HOSTS_FILE);
    printf("Continue? [y/N]: "); fflush(stdout); char ans[8]; if(!fgets(ans,sizeof(ans),stdin)) ans[0]='n';
    if(ans[0]!='y' && ans[0]!='Y'){ printf(YELLOW "Cancelled." RESET "\n"); return 0; }
    snprintf(cmd,sizeof(cmd),"sudo cp '%s' '%s.kley-pre-remove'", HOSTS_FILE, HOSTS_FILE); system(cmd);
    snprintf(cmd,sizeof(cmd),"sudo sed -i '/%s/,/%s/d' '%s'", MARKER_START, MARKER_END, HOSTS_FILE); system(cmd);
    printf(GREEN "✅ Successfully removed %ld kley rules!" RESET "\n", cnt);
    FILE *pp2=popen("wc -l < /etc/hosts","r"); long rem=0; if(pp2){ fscanf(pp2,"%ld",&rem); pclose(pp2); }
    printf(CYAN "📊 Remaining /etc/hosts lines: %ld" RESET "\n", rem);
    notify_send("normal","dialog-information","🗑️ Blocklist Cleaned","Removed kley domains",3000);
    return 0;
}
