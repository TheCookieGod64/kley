#include "kley.h"

int cmd_on(int argc, char **argv){
    if(file_exists(HOSTS_BACKUP)){ printf(YELLOW "⚠  Kley mode is already ON!" RESET "\n"); cmd_status(); return 1; }
    ensure_state_dir();
    printf(BLUE "🔓 Activating kley mode (disabling adblock)..." RESET "\n");
    char cmd[PATH_MAX*2];
    snprintf(cmd,sizeof(cmd),"sudo mv '%s' '%s'", HOSTS_FILE, HOSTS_BACKUP);
    if(system(cmd)!=0){ fprintf(stderr, RED "❌ Failed to backup hosts file!" RESET "\n"); return 1; }
    char hn[256]; get_hostname_custom(hn,sizeof(hn));
    snprintf(cmd,sizeof(cmd),"printf '127.0.0.1\\tlocalhost\\n::1\\t\\tlocalhost\\n127.0.1.1\\t%s\\n' | sudo tee '%s' >/dev/null", hn, HOSTS_FILE);
    system(cmd);
    FILE *f=fopen(state_file,"w"); if(f){ fprintf(f,"%ld\n", (long)time(NULL)); fclose(f); }
    if(argc>0){
        char *t=argv[0];
        int isnum=1; for(char *p=t; *p; p++) if(!isdigit((unsigned char)*p)) isnum=0;
        if(!isnum) printf(YELLOW "⚠  Invalid timeout: '%s' (must be a number)" RESET "\n", t);
        else {
            long mins=atol(t);
            if(mins<1) printf(YELLOW "⚠  Timeout too short (<1 min), ignoring" RESET "\n");
            else if(mins>120) printf(YELLOW "⚠  Timeout too long (>120 min), ignoring" RESET "\n");
            else {
                long sec=mins*60; long at=time(NULL)+sec;
                FILE *tf=fopen(timeout_file,"w"); if(tf){ fprintf(tf,"%ld\n",at); fclose(tf); }
                pid_t pid=fork();
                if(pid==0){
                    sleep(sec);
                    if(file_exists(HOSTS_BACKUP)) execlp("kley","kley","off","--auto",(char*)NULL);
                    exit(0);
                } else if(pid>0){
                    FILE *pf=fopen(timeout_pid_file,"w"); if(pf){ fprintf(pf,"%d\n", pid); fclose(pf); }
                    printf(MAGENTA "⏰ Auto-off scheduled in %ld minute(s)" RESET "\n", mins);
                }
            }
        }
    }
    printf(GREEN "✅ Adblock OFF - Go get your key!" RESET "\n");
    printf(YELLOW "⚠  Don't forget: " BOLD "kley off" RESET YELLOW " when done." RESET "\n");
    if(argc>0) notify_send("critical","security-low","🔓 KLEY MODE: ON","Adblock disabled — auto-off scheduled",5000);
    else notify_send("critical","security-low","🔓 KLEY MODE: ON","Adblock is DISABLED — remember to turn it off!",5000);
    return 0;
}
