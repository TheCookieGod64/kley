#include "kley.h"

char state_dir[PATH_MAX];
char state_file[PATH_MAX];
char timeout_file[PATH_MAX];
char timeout_pid_file[PATH_MAX];

void init_paths(void){
    const char *xdg = getenv("XDG_STATE_HOME");
    const char *home = getenv("HOME");
    if (xdg && strlen(xdg)>0) snprintf(state_dir, sizeof(state_dir), "%s/kley", xdg);
    else if (home) snprintf(state_dir, sizeof(state_dir), "%s/.local/state/kley", home);
    else snprintf(state_dir, sizeof(state_dir), "/tmp/kley");
    snprintf(state_file, sizeof(state_file), "%s/session", state_dir);
    snprintf(timeout_file, sizeof(timeout_file), "%s/timeout", state_dir);
    snprintf(timeout_pid_file, sizeof(timeout_pid_file), "%s/timeout.pid", state_dir);
}

void ensure_state_dir(void){ char cmd[PATH_MAX+20]; snprintf(cmd,sizeof(cmd),"mkdir -p '%s'", state_dir); system(cmd); }
long get_elapsed(void){
    FILE *f=fopen(state_file,"r"); if(!f) return 0;
    long start=0; fscanf(f,"%ld",&start); fclose(f);
    return time(NULL)-start;
}
void format_duration(long sec, char *out, size_t n){ snprintf(out,n,"%ldm %lds", sec/60, sec%60); }
void cancel_timeout(void){
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

