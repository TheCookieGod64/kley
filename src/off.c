#include "kley.h"

int cmd_off(int argc, char **argv){
    int is_auto = argc>0 && strcmp(argv[0],"--auto")==0;
    if(!file_exists(HOSTS_BACKUP)){
        if(!is_auto){ printf(YELLOW "⚠  Kley mode is already OFF!" RESET "\n"); cmd_status(); }
        return 1;
    }
    printf(BLUE "🔒 Restoring adblock..." RESET "\n");
    char cmd[PATH_MAX*2]; snprintf(cmd,sizeof(cmd),"sudo mv '%s' '%s'", HOSTS_BACKUP, HOSTS_FILE);
    if(system(cmd)!=0){ fprintf(stderr, RED "❌ Failed to restore hosts backup!" RESET "\n"); return 1; }
    long elapsed=get_elapsed();
    unlink(state_file); cancel_timeout();
    FILE *f=fopen(HOSTS_FILE,"r"); long lines=0; if(f){ int c; while((c=fgetc(f))!=EOF) if(c=='\n') lines++; fclose(f); }
    printf(GREEN "✅ Adblock back ON (%ld rules active)" RESET "\n", lines);
    if(elapsed>0){ char d[32]; format_duration(elapsed,d,sizeof(d)); printf(CYAN "⏱  Was active for: %s" RESET "\n", d); }
    if(is_auto){ char body[128]; snprintf(body,sizeof(body),"Auto-timeout: adblock restored (%ld rules)", lines); notify_send("normal","security-high","🔒 KLEY MODE: OFF",body,4000); }
    else { char body[128]; snprintf(body,sizeof(body),"Adblock restored (%ld rules active)", lines); notify_send("normal","security-high","🔒 KLEY MODE: OFF",body,3000); }
    return 0;
}
