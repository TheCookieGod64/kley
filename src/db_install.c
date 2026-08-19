#include "kley.h"

int cmd_install_db(int argc, char **argv){
    const char *url = argc>0 ? argv[0] : DB_URL;
    char tmp1[]="/tmp/kley-blocklist.XXXXXX", tmp2[]="/tmp/kley-existing.XXXXXX", tmp3[]="/tmp/kley-new.XXXXXX";
    int fd1=mkstemp(tmp1), fd2=mkstemp(tmp2), fd3=mkstemp(tmp3); close(fd1); close(fd2); close(fd3);
    if (argc==0 || strcmp(url, DB_URL)==0)
        printf(BLUE "📥 Downloading TheCookieGod64's blocklist database..." RESET "\n");
    else
        printf(BLUE "📥 Downloading blocklist from %s..." RESET "\n", url);
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
    snprintf(cmd,sizeof(cmd),"grep -q '%s' '%s' && sudo sed -i '/%s/,/%s/d' '%s' || true", MARKER_START, HOSTS_FILE, MARKER_START, MARKER_END, HOSTS_FILE); system(cmd);
    printf(BLUE "🔍 Checking for duplicate domains..." RESET "\n");
    snprintf(cmd,sizeof(cmd),"awk '/^[0-9]/ {print $2}' '%s' | sort -u > '%s'", HOSTS_FILE, tmp2); system(cmd);
    snprintf(cmd,sizeof(cmd),"awk 'NR==FNR { existing[$1]=1; next } { if ($1 ~ /^[0-9]/ && !existing[$2]) print $1 \" \" $2 }' '%s' '%s' > '%s'", tmp2, tmp1, tmp3); system(cmd);
    char wc2[512]; snprintf(wc2,sizeof(wc2),"wc -l < '%s'", tmp3); pp=popen(wc2,"r"); long newc=0; if(pp){ fscanf(pp,"%ld",&newc); pclose(pp); }
    if(newc==0){ printf(YELLOW "⚠  All entries already exist. Nothing new!" RESET "\n"); unlink(tmp1); unlink(tmp2); unlink(tmp3); return 0; }
    printf(BLUE "➕ Adding %ld new entries (skipped %ld duplicates)..." RESET "\n", newc, total-newc);
    char datecmd[1024]; snprintf(datecmd,sizeof(datecmd),"(echo ''; echo '%s'; echo \"# Installed: $(date -u +'%%Y-%%m-%%d %%H:%%M UTC')\"; echo '# Source: %s'; echo '# Total entries: %ld'; cat '%s'; echo '%s') | sudo tee -a '%s' >/dev/null", MARKER_START, url, newc, tmp3, MARKER_END, HOSTS_FILE);
    system(datecmd);
    unlink(tmp1); unlink(tmp2); unlink(tmp3);
    pp=popen("wc -l < /etc/hosts","r"); long th=0; if(pp){ fscanf(pp,"%ld",&th); pclose(pp); }
    printf(GREEN "✅ Successfully installed %ld rules!" RESET "\n" CYAN "📊 Total /etc/hosts lines: %ld" RESET "\n", newc, th);
    notify_send("normal","dialog-information","📥 Blocklist Updated","Blocklist installed",4000);
    return 0;
}
