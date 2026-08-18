#include "kley.h"

int cmd_status(void){
    if(file_exists(HOSTS_BACKUP)){
        printf(RED "🔓 KLEY MODE: ON" RESET YELLOW " (adblock disabled)" RESET "\n");
        long elapsed=get_elapsed();
        if(elapsed>0){ char d[32]; format_duration(elapsed,d,sizeof(d)); printf(CYAN "⏱  Active for: %s" RESET "\n", d); }
        if(file_exists(timeout_file)){
            FILE *f=fopen(timeout_file,"r"); long at=0; if(f){ fscanf(f,"%ld",&at); fclose(f); }
            long rem= at - time(NULL);
            if(rem>0){ char d[32]; format_duration(rem,d,sizeof(d)); printf(MAGENTA "⏰ Auto-off in: %s" RESET "\n", d); }
        }
        if(elapsed/60 > 10) printf(RED "⚠  You have been unprotected for %ld minutes!" RESET "\n" YELLOW "   Turn off with: " BOLD "kley off" RESET "\n", elapsed/60);
    } else {
        FILE *f=fopen(HOSTS_FILE,"r"); long lines=0; if(f){ int c; while((c=fgetc(f))!=EOF) if(c=='\n') lines++; fclose(f); }
        printf(GREEN "🔒 KLEY MODE: OFF" RESET CYAN " (adblock active - %ld rules)" RESET "\n", lines);
    }
    return 0;
}
