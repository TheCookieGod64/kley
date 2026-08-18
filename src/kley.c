#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <limits.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>

#define KLEY_VERSION "2.0.0"
#define HOSTS_FILE "/etc/hosts"
#define HOSTS_BACKUP "/etc/hosts.monster"
#define MARKER_START "# === KLEY BLOCKLIST START ==="
#define MARKER_END "# === KLEY BLOCKLIST END ==="
#define DB_URL "https://raw.githubusercontent.com/TheCookieGod64/kley-db/main/blocklist.txt"

// ANSI colors
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[0;34m"
#define CYAN "\033[0;36m"
#define MAGENTA "\033[0;35m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

static char state_dir[PATH_MAX];
static char state_file[PATH_MAX];
static char timeout_file[PATH_MAX];
static char timeout_pid_file[PATH_MAX];

void init_paths() {
    const char *xdg = getenv("XDG_STATE_HOME");
    const char *home = getenv("HOME");
    if (xdg && strlen(xdg)>0) snprintf(state_dir, sizeof(state_dir), "%s/kley", xdg);
    else if (home) snprintf(state_dir, sizeof(state_dir), "%s/.local/state/kley", home);
    else snprintf(state_dir, sizeof(state_dir), "/tmp/kley");
    snprintf(state_file, sizeof(state_file), "%s/session", state_dir);
    snprintf(timeout_file, sizeof(timeout_file), "%s/timeout", state_dir);
    snprintf(timeout_pid_file, sizeof(timeout_pid_file), "%s/timeout.pid", state_dir);
}

int has_notify() { return system("command -v notify-send >/dev/null 2>&1")==0; }
void notify_send(const char *urgency, const char *icon, const char *title, const char *body, int ms){
    if(!has_notify()) return;
    char cmd[1024];
    snprintf(cmd,sizeof(cmd),"notify-send -u '%s' -i '%s' -t %d -a kley '%s' '%s' 2>/dev/null || true", urgency, icon, ms, title, body);
    system(cmd);
}
void ensure_state_dir(){ char cmd[PATH_MAX+20]; snprintf(cmd,sizeof(cmd),"mkdir -p '%s'", state_dir); system(cmd); }
long get_elapsed(){
    FILE *f=fopen(state_file,"r"); if(!f) return 0;
    long start=0; fscanf(f,"%ld",&start); fclose(f);
    return time(NULL)-start;
}
void format_duration(long sec, char *out, size_t n){ snprintf(out,n,"%ldm %lds", sec/60, sec%60); }
void cancel_timeout(){
    FILE *f=fopen(timeout_pid_file,"r");
    if(f){ long pid=0; fscanf(f,"%ld",&pid); fclose(f); if(pid>0) kill((pid_t)pid, SIGTERM); }
    unlink(timeout_pid_file); unlink(timeout_file);
}
int file_exists(const char *p){ struct stat s; return stat(p,&s)==0; }
void get_hostname_custom(char *out, size_t n){
    FILE *f=fopen("/etc/hostname","r");
    if(f){ if(fgets(out,n,f)){ out[strcspn(out,"\r\n")]=0; fclose(f); return; } fclose(f); }
    if(gethostname(out,n)==0) return;
    strncpy(out,"localhost",n);
}

int cmd_status(){
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
    // auto timeout
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
                    if(file_exists(HOSTS_BACKUP)){
                        char self[PATH_MAX]; ssize_t len=readlink("/proc/self/exe", self,sizeof(self)-1);
                        if(len>0){ self[len]=0; execl(self, self, "off", "--auto", (char*)NULL); }
                        // fallback to /usr/local/bin/kley etc
                        execlp("kley","kley","off","--auto",(char*)NULL);
                        exit(0);
                    }
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
int cmd_toggle(int argc, char **argv){ if(file_exists(HOSTS_BACKUP)) return cmd_off(argc,argv); else return cmd_on(argc,argv); }
int cmd_cancel(){
    if(!file_exists(timeout_file)){ printf(YELLOW "⚠  No auto-off timer is scheduled." RESET "\n"); return 0; }
    cancel_timeout();
    printf(GREEN "✅ Auto-off timer cancelled." RESET "\n");
    printf(YELLOW "   Kley mode is still ON — turn off manually with: " BOLD "kley off" RESET "\n");
    notify_send("normal","dialog-information","⏰ Timer Cancelled","Auto-off cancelled — kley mode still ON",3000);
    return 0;
}
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

// DB helpers simplified - call curl and do dedup via shell tools for robustness
int cmd_install_db(int argc, char **argv){
    const char *url = argc>0 ? argv[0] : DB_URL;
    char tmp1[]="/tmp/kley-blocklist.XXXXXX", tmp2[]="/tmp/kley-existing.XXXXXX", tmp3[]="/tmp/kley-new.XXXXXX";
    int fd1=mkstemp(tmp1), fd2=mkstemp(tmp2), fd3=mkstemp(tmp3); close(fd1); close(fd2); close(fd3);
    printf(BLUE "📥 Downloading TheCookieGod64's blocklist database..." RESET "\n");
    char cmd[1024];
    if(strncmp(url,"file://",7)==0) snprintf(cmd,sizeof(cmd),"cp '%s' '%s'", url+7, tmp1);
    else if(access(url,F_OK)==0) snprintf(cmd,sizeof(cmd),"cp '%s' '%s'", url, tmp1);
    else snprintf(cmd,sizeof(cmd),"curl -fsSL '%s' -o '%s'", url, tmp1);
    if(system(cmd)!=0){ fprintf(stderr, RED "❌ Failed to download database" RESET "\n"); unlink(tmp1); unlink(tmp2); unlink(tmp3); return 1; }
    char wc[512]; snprintf(wc,sizeof(wc),"wc -l < '%s'", tmp1); FILE *pp=popen(wc,"r"); long total=0; if(pp){ fscanf(pp,"%ld",&total); pclose(pp); }
    printf(CYAN "📊 Source database contains %ld blocklist rules." RESET "\n", total);
    printf(YELLOW "⚠  This will merge entries into %s." RESET "\n" YELLOW "   Existing domains will NOT be duplicated." RESET "\n", HOSTS_FILE);
    printf("Do you want to proceed? [y/N]: "); fflush(stdout);
    char ans[8]; if(!fgets(ans,sizeof(ans),stdin)) ans[0]='n';
    if(ans[0]!='y' && ans[0]!='Y'){ printf(YELLOW "Installation cancelled." RESET "\n"); unlink(tmp1); unlink(tmp2); unlink(tmp3); return 0; }
    if(file_exists(HOSTS_BACKUP)){ fprintf(stderr, RED "❌ Kley mode is currently active. Run 'kley off' first." RESET "\n"); unlink(tmp1); unlink(tmp2); unlink(tmp3); return 1; }
    char bak[PATH_MAX]; snprintf(bak,sizeof(bak),"%s.kley-backup-%ld", HOSTS_FILE, (long)time(NULL));
    snprintf(cmd,sizeof(cmd),"sudo cp '%s' '%s'", HOSTS_FILE, bak); system(cmd);
    // remove old kley entries
    snprintf(cmd,sizeof(cmd),"grep -q '%s' '%s' && sudo sed -i '/%s/,/%s/d' '%s' || true", MARKER_START, HOSTS_FILE, MARKER_START, MARKER_END, HOSTS_FILE); system(cmd);
    printf(BLUE "🔍 Checking for duplicate domains..." RESET "\n");
    snprintf(cmd,sizeof(cmd),"awk '/^[0-9]/ {print $2}' '%s' | sort -u > '%s'", HOSTS_FILE, tmp2); system(cmd);
    snprintf(cmd,sizeof(cmd),"awk 'NR==FNR { existing[$1]=1; next } { if ($1 ~ /^[0-9]/ && !existing[$2]) print $1 \" \" $2 }' '%s' '%s' > '%s'", tmp2, tmp1, tmp3); system(cmd);
    pp=popen(wc,"r"); long total2=total; // reuse
    char wc2[512]; snprintf(wc2,sizeof(wc2),"wc -l < '%s'", tmp3); pp=popen(wc2,"r"); long newc=0; if(pp){ fscanf(pp,"%ld",&newc); pclose(pp); }
    if(newc==0){ printf(YELLOW "⚠  All entries already exist. Nothing new!" RESET "\n"); unlink(tmp1); unlink(tmp2); unlink(tmp3); return 0; }
    printf(BLUE "➕ Adding %ld new entries (skipped %ld duplicates)..." RESET "\n", newc, total-newc);
    snprintf(cmd,sizeof(cmd),"(echo ''; echo '%s'; echo '# Installed: %s'; echo '# Source: %s'; echo '# Total entries: %ld'; cat '%s'; echo '%s') | sudo tee -a '%s' >/dev/null", MARKER_START, "now", url, newc, tmp3, MARKER_END, HOSTS_FILE);
    // use date
    char datecmd[512]; snprintf(datecmd,sizeof(datecmd),"(echo ''; echo '%s'; echo \"# Installed: $(date -u +'%%Y-%%m-%%d %%H:%%M UTC')\"; echo '# Source: %s'; echo '# Total entries: %ld'; cat '%s'; echo '%s') | sudo tee -a '%s' >/dev/null", MARKER_START, url, newc, tmp3, MARKER_END, HOSTS_FILE);
    system(datecmd);
    unlink(tmp1); unlink(tmp2); unlink(tmp3);
    pp=popen("wc -l < /etc/hosts","r"); long th=0; if(pp){ fscanf(pp,"%ld",&th); pclose(pp); }
    printf(GREEN "✅ Successfully installed %ld rules!" RESET "\n" CYAN "📊 Total /etc/hosts lines: %ld" RESET "\n", newc, th);
    notify_send("normal","dialog-information","📥 Blocklist Updated","Blocklist installed",4000);
    return 0;
}
int cmd_remove_db(){
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
int cmd_db_info(){
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
void print_help(){
    printf(BOLD BLUE "🔑 KLEY" RESET " - Adblock toggle & hosts manager for key-farming\n" CYAN "Version %s by TheCookieGod64 (C port - major v2.0.0)" RESET "\n\n", KLEY_VERSION);
    printf(BOLD YELLOW "USAGE:" RESET "\n    kley <command> [args]\n\n");
    printf(BOLD YELLOW "ADBLOCK TOGGLE:" RESET "\n");
    printf("    " GREEN "on" RESET " [minutes]      Disable adblock (with optional auto-off timer)\n");
    printf("    " GREEN "off" RESET "               Restore adblock\n");
    printf("    " GREEN "toggle" RESET ", t         Toggle between on/off\n");
    printf("    " GREEN "status" RESET ", check     Show current adblock status\n");
    printf("    " GREEN "cancel" RESET "            Cancel scheduled auto-off timer\n");
    printf("    " GREEN "extend" RESET " <min>      Reschedule auto-off to X minutes from now\n\n");
    printf(BOLD YELLOW "DATABASE COMMANDS:" RESET "\n");
    printf("    " GREEN "install-db" RESET " [url]  Download and smart-merge community blocklist\n");
    printf("    " GREEN "update-db" RESET "         Update the blocklist to latest version\n");
    printf("    " GREEN "remove-db" RESET "         Remove only kley's entries from /etc/hosts\n");
    printf("    " GREEN "db-info" RESET "           Show stats on installed blocklist entries\n\n");
    printf(BOLD YELLOW "GENERAL:" RESET "\n");
    printf("    " GREEN "version" RESET ", -v       Show version\n");
    printf("    " GREEN "help" RESET ", -h          Show this help\n\n");
    printf(BOLD YELLOW "EXAMPLES:" RESET "\n");
    printf("    kley on                # Disable adblock (manual off required)\n");
    printf("    kley on 5              # Disable adblock, auto-off in 5 minutes\n");
    printf("    kley extend 3          # Reschedule auto-off to 3 min from now\n");
    printf("    kley cancel            # Cancel auto-off (still ON, manual off needed)\n");
    printf("    kley off               # Re-enable adblock protection\n");
}

int main(int argc, char *argv[]){
    init_paths();
    const char *cmd = argc>1 ? argv[1] : "status";
    if(strcmp(cmd,"on")==0||strcmp(cmd,"start")==0||strcmp(cmd,"aan")==0||strcmp(cmd,"onn")==0) return cmd_on(argc-2, argv+2);
    if(strcmp(cmd,"off")==0||strcmp(cmd,"stop")==0||strcmp(cmd,"uit")==0||strcmp(cmd,"oof")==0) return cmd_off(argc-2, argv+2);
    if(strcmp(cmd,"status")==0||strcmp(cmd,"check")==0||strcmp(cmd,"stauts")==0||strcmp(cmd,"")==0) return cmd_status();
    if(strcmp(cmd,"toggle")==0||strcmp(cmd,"t")==0||strcmp(cmd,"toggly")==0||strcmp(cmd,"togly")==0||strcmp(cmd,"toggel")==0){ return cmd_toggle(argc-2, argv+2); }
    if(strcmp(cmd,"cancel")==0||strcmp(cmd,"abort")==0||strcmp(cmd,"nocancel")==0) return cmd_cancel();
    if(strcmp(cmd,"extend")==0||strcmp(cmd,"reset-timer")==0||strcmp(cmd,"reschedule")==0) return cmd_extend(argc-2, argv+2);
    if(strcmp(cmd,"install-db")==0||strcmp(cmd,"installdb")==0) return cmd_install_db(argc-2, argv+2);
    if(strcmp(cmd,"update-db")==0||strcmp(cmd,"updatedb")==0) return cmd_install_db(argc-2, argv+2);
    if(strcmp(cmd,"remove-db")==0||strcmp(cmd,"removedb")==0) return cmd_remove_db();
    if(strcmp(cmd,"db-info")==0||strcmp(cmd,"dbinfo")==0||strcmp(cmd,"info")==0) return cmd_db_info();
    if(strcmp(cmd,"version")==0||strcmp(cmd,"-v")==0||strcmp(cmd,"--version")==0){ printf("kley v%s\n", KLEY_VERSION); return 0; }
    if(strcmp(cmd,"help")==0||strcmp(cmd,"-h")==0||strcmp(cmd,"--help")==0||strcmp(cmd,"halp")==0){ print_help(); return 0; }
    fprintf(stderr, RED "❌ Unknown command: %s" RESET "\n" "Type " BLUE "kley help" RESET " for usage.\n", cmd);
    return 1;
}
