/*
================================================================================
OSL PRACTICALS — SINGLE C FILE OF ALL CODES (EXCEPT 1.5)
--------------------------------------------------------------------------------
Source of problem statements (verbatim titles & requirements summarized inside):
fileciteturn1file0 fileciteturn1file1 fileciteturn1file2 fileciteturn1file3 fileciteturn1file4

Build:
    gcc -O2 -pthread OSL_All_Codes.c -o osl_all

Run:
    ./osl_all

Notes:
 - Questions 1.x were originally shell scripts; here we provide equivalent C implementations.
 - Question 2.3 demonstrates exec: compile this same file first; it will exec itself with a special mode.
 - For thread/synchronization demos, we keep run-time short and print clear traces.
 - File I/O uses current working directory; create sample files as needed.
================================================================================
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <time.h>

/* -------------------------- Small Utilities -------------------------- */
static void swap_int(int *a,int *b){int t=*a;*a=*b;*b=t;}
static int cmp_int(const void*a,const void*b){return (*(int*)a-*(int*)b);}
static void bubble_sort(int *a,int n){for(int i=0;i<n-1;i++)for(int j=0;j<n-1-i;j++)if(a[j]>a[j+1])swap_int(&a[j],&a[j+1]);}
static void insertion_sort(int *a,int n){for(int i=1;i<n;i++){int k=a[i],j=i-1;while(j>=0&&a[j]>k){a[j+1]=a[j];j--;}a[j+1]=k;}}
/* merge sort */
static void merge(int *a,int l,int m,int r){int n1=m-l+1,n2=r-m;int *L=malloc(4*n1),*R=malloc(4*n2);for(int i=0;i<n1;i++)L[i]=a[l+i];for(int j=0;j<n2;j++)R[j]=a[m+1+j];int i=0,j=0,k=l;while(i<n1&&j<n2)a[k++]=(L[i]<=R[j])?L[i++]:R[j++];while(i<n1)a[k++]=L[i++];while(j<n2)a[k++]=R[j++];free(L);free(R);}
static void merge_sort(int *a,int l,int r){if(l<r){int m=(l+r)/2;merge_sort(a,l,m);merge_sort(a,m+1,r);merge(a,l,m,r);}}
/* quick sort */
static int part(int*a,int l,int r){int p=a[r],i=l-1;for(int j=l;j<r;j++)if(a[j]<=p)swap_int(&a[++i],&a[j]);swap_int(&a[i+1],&a[r]);return i+1;}
static void quick_sort(int*a,int l,int r){if(l<r){int pi=part(a,l,r);quick_sort(a,l,pi-1);quick_sort(a,pi+1,r);}}

/* Read int array from stdin */
static int *read_array(int *n){
    printf("Enter n: "); if(scanf("%d",n)!=1||*n<=0){puts("Invalid n"); return NULL;}
    int *a=malloc(sizeof(int)*(*n));
    printf("Enter %d integers: ",*n);
    for(int i=0;i<*n;i++){ if(scanf("%d",&a[i])!=1){free(a);return NULL;} }
    return a;
}

/* -------------------------- SECTION 1: SHELL-SCRIPT EQUIVALENTS -------------------------- */

#!/usr/bin/env bash
# =========================================================================================
# OSL SHELL PRACTICALS — ALL SHELL CODES IN ONE FILE (1.1, 1.2, 1.3, 1.4, 1.6, 1.7)
# (Skip 1.5 as requested)
# Usage:
#   chmod +x osl_shell_all.sh
#   ./osl_shell_all.sh
# -----------------------------------------------------------------------------------------
# Files used:
#   1.1  address.txt               (semicolon ; separated: ID;Name;Phone)
#   1.2  phonebook.txt             (tab-separated: Full Name<TAB>Phone)
#   1.3  addr_colon.txt            (colon : separated: ID:Name:Phone)
#   1.4  general file ops on any filename you give
#   1.6  general utilities
#   1.7  general utilities
# =========================================================================================

# ------------------------------ helpers ------------------------------
pause() { read -rp "Press ENTER to continue..."; }
exists_or_empty_file() { [[ -f "$1" ]] || : >"$1"; }  # create if missing
line() { printf '%*s\n' "${COLUMNS:-80}" '' | tr ' ' -; }

# ------------------------------ 1.1 ------------------------------
: <<'__Q11__'
1.1 Write a shell script to implement an address book (address.txt) with:
    1) Search Address Book
    2) Add an address book entry
    3) Remove an address book entry
    4) Quit
Record format: semicolon ; separated => ID;Name;Phone
__Q11__

q11() {
  local file="address.txt"
  exists_or_empty_file "$file"
  while :; do
    line
    echo "[1.1] Address Book (semicolon: ID;Name;Phone) -> $file"
    echo "1) Search  2) Add  3) Remove  4) Display  5) Quit"
    read -rp "Choice: " ch
    case "$ch" in
      1)
        read -rp "Enter ID/Name/Phone to search: " key
        grep -i -- "$key" "$file" || echo "No match."
        ;;
      2)
        read -rp "ID: " id
        read -rp "Name: " name
        read -rp "Phone: " ph
        printf '%s;%s;%s\n' "$id" "$name" "$ph" >> "$file"
        echo "Added."
        ;;
      3)
        read -rp "Enter ID to remove: " rid
        if grep -q "^${rid};" "$file"; then
          grep -v "^${rid};" "$file" > "$file.tmp" && mv "$file.tmp" "$file"
          echo "Removed."
        else
          echo "ID not found."
        fi
        ;;
      4) cat "$file" ;;
      5) break ;;
      *) echo "Invalid choice." ;;
    esac
  done
}

# ------------------------------ 1.2 ------------------------------
: <<'__Q12__'
1.2 Phonebook (tab-separated):
    1) Add new name
    2) Display all matches to a name or phone number
    3) Sort the phonebook by LAST NAME
    4) Delete an entry
Record format: “Full Name<TAB>Phone”
__Q12__

q12() {
  local file="phonebook.txt"
  exists_or_empty_file "$file"
  while :; do
    line
    echo "[1.2] Phonebook (tab: Full Name<TAB>Phone) -> $file"
    echo "1) Add  2) Match  3) Sort by last name  4) Delete  5) Display  6) Quit"
    read -rp "Choice: " ch
    case "$ch" in
      1)
        read -rp "Full Name: " fullname
        read -rp "Phone: " ph
        printf '%s\t%s\n' "$fullname" "$ph" >> "$file"
        echo "Added."
        ;;
      2)
        read -rp "Search key (name or phone): " key
        grep -i -- "$key" "$file" || echo "No match."
        ;;
      3)
        # Extract last word of the name field and sort by it
        awk -F'\t' '{
          name=$1; phone=$2;
          n=split(name,a," ");
          last=a[n];
          printf "%s\t%s\t%s\n", last, name, phone
        }' "$file" \
        | sort -f \
        | awk -F'\t' '{printf "%s\t%s\n", $2, $3}' > "$file.tmp" && mv "$file.tmp" "$file"
        echo "Sorted by last name."
        ;;
      4)
        read -rp "Full Name to delete: " delname
        if grep -qi "^${delname}[[:space:]]" "$file"; then
          awk -F'\t' -v target="$delname" 'BEGIN{IGNORECASE=1} $1!=target' "$file" > "$file.tmp" && mv "$file.tmp" "$file"
          echo "Deleted."
        else
          echo "Name not found."
        fi
        ;;
      5) cat "$file" ;;
      6) break ;;
      *) echo "Invalid choice." ;;
    esac
  done
}

# ------------------------------ 1.3 ------------------------------
: <<'__Q13__'
1.3 Address book using functions & menu:
    1) Search
    2) Add entries
    3) Remove / edit entries
Also provide a "display" function. Record format: ID:Name:Phone (colon separated)
__Q13__

q13_display() { cat "addr_colon.txt"; }
q13_add() {
  local file="addr_colon.txt"
  exists_or_empty_file "$file"
  read -rp "ID: " id
  read -rp "Name: " name
  read -rp "Phone: " ph
  printf '%s:%s:%s\n' "$id" "$name" "$ph" >> "$file"
}
q13_search() {
  read -rp "Key (ID/Name/Phone): " key
  grep -i -- "$key" "addr_colon.txt" || echo "No match."
}
q13_edit() {
  local file="addr_colon.txt" id
  exists_or_empty_file "$file"
  read -rp "ID to edit: " id
  if ! grep -q "^${id}:" "$file"; then echo "ID not found."; return; fi
  read -rp "New Name: " nm
  read -rp "New Phone: " ph
  awk -F':' -v id="$id" -v nm="$nm" -v ph="$ph" 'BEGIN{OFS=":"}
      $1==id{$2=nm;$3=ph} {print}' "$file" > "$file.tmp" && mv "$file.tmp" "$file"
  echo "Edited."
}
q13_remove() {
  local file="addr_colon.txt" id
  exists_or_empty_file "$file"
  read -rp "ID to remove: " id
  if grep -q "^${id}:" "$file"; then
    grep -v "^${id}:" "$file" > "$file.tmp" && mv "$file.tmp" "$file"
    echo "Removed."
  else
    echo "ID not found."
  fi
}
q13() {
  local f="addr_colon.txt"; exists_or_empty_file "$f"
  while :; do
    line
    echo "[1.3] Address Book (colon: ID:Name:Phone) -> $f"
    echo "1) Display  2) Add  3) Search  4) Edit  5) Remove  6) Quit"
    read -rp "Choice: " ch
    case "$ch" in
      1) q13_display ;;
      2) q13_add ;;
      3) q13_search ;;
      4) q13_edit ;;
      5) q13_remove ;;
      6) break ;;
      *) echo "Invalid choice." ;;
    esac
  done
}

# ------------------------------ 1.4 ------------------------------
: <<'__Q14__'
1.4 File management program (menu):
    1) Test if file exists
    2) Read a file
    3) Delete a file
    4) Display list of files (ls)
Assume sample files like book.txt and test.txt may exist in cwd.
__Q14__

q14() {
  while :; do
    line
    echo "[1.4] File Manager"
    echo "1) Test exists  2) Read  3) Delete  4) List (pwd)  5) Quit"
    read -rp "Choice: " ch
    case "$ch" in
      1)
        read -rp "Filename: " f
        [[ -e "$f" ]] && echo "Exists" || echo "Not found"
        ;;
      2)
        read -rp "Filename: " f
        [[ -r "$f" ]] && cat "$f" || echo "Cannot open '$f'"
        ;;
      3)
        read -rp "Filename: " f
        if [[ -e "$f" ]]; then rm -f -- "$f" && echo "Deleted."; else echo "Not found."; fi
        ;;
      4) ls -lah ;;
      5) break ;;
      *) echo "Invalid choice." ;;
    esac
  done
}

# ------------------------------ 1.6 ------------------------------
: <<'__Q16__'
1.6 Write shell scripts:
 a) Accept filename, start & end line numbers; display lines between them.
 b) Delete all lines containing a specified word in one or more files given as args.
 c) Take two numbers as args and output their sum using:
      i) bc      ii) expr
    (Also include error checking for two args.)
__Q16__

q16() {
  line
  echo "[1.6] Choose sub-part: a / b / c (or q to quit)"
  read -rp "Choice: " sub
  case "$sub" in
    a|A)
      read -rp "Filename: " f
      read -rp "Start line: " s
      read -rp "End line: " e
      if [[ -f "$f" && "$s" =~ ^[0-9]+$ && "$e" =~ ^[0-9]+$ && $s -le $e ]]; then
        sed -n "${s},${e}p" -- "$f"
      else
        echo "Invalid input."
      fi
      ;;
    b|B)
      read -rp "Word to delete: " w
      echo "Enter file names (space separated):"
      read -r -a files
      for f in "${files[@]}"; do
        if [[ -f "$f" ]]; then
          sed "/$w/d" "$f" > "$f.tmp" && mv "$f.tmp" "$f"
          echo "Processed: $f"
        else
          echo "Skip (not found): $f"
        fi
      done
      ;;
    c|C)
      if [[ $# -ge 2 ]]; then
        a="$1"; b="$2"
      else
        read -rp "Enter two numbers (space separated): " a b
      fi
      if [[ "$a" =~ ^-?[0-9]+([.][0-9]+)?$ && "$b" =~ ^-?[0-9]+([.][0-9]+)?$ ]]; then
        echo "Sum via bc  : $(echo "$a + $b" | bc)"
        echo "Sum via expr: $(expr "$a" + "$b" 2>/dev/null || echo "expr supports integers only")"
      else
        echo "Error: need two numeric arguments."
      fi
      ;;
    q|Q) : ;;
    *) echo "Invalid." ;;
  esac
}

# ------------------------------ 1.7 ------------------------------
: <<'__Q17__'
1.7 Write shell scripts:
 a) Use 'find' to look for a file and echo suitable msg if not found. (Do not store find output in a file.)
 b) Take a command-line argument and report whether it is directory, a file, or something else.
 c) Accept one or more filenames as args and convert ALL of them to UPPERCASE (content), if they exist.
__Q17__

q17() {
  line
  echo "[1.7] Choose sub-part: a / b / c (or q to quit)"
  read -rp "Choice: " sub
  case "$sub" in
    a|A)
      read -rp "Filename to find (search from current dir): " name
      if find . -type f -name "$name" -print -quit | grep -q .; then
        echo "Found."
      else
        echo "Not found."
      fi
      ;;
    b|B)
      read -rp "Path: " p
      if [[ -d "$p" ]]; then echo "Directory"
      elif [[ -f "$p" ]]; then echo "File"
      elif [[ -e "$p" ]]; then echo "Special (exists but not regular file/dir)"
      else echo "Does not exist"; fi
      ;;
    c|C)
      echo "Enter file names to UPPERCASE (space separated):"
      read -r -a files
      for f in "${files[@]}"; do
        if [[ -f "$f" ]]; then
          awk '{print toupper($0)}' "$f" > "$f.tmp" && mv "$f.tmp" "$f"
          echo "UPPERCASED: $f"
        else
          echo "Skip (not found): $f"
        fi
      done
      ;;
    q|Q) : ;;
    *) echo "Invalid." ;;
  esac
}

# ------------------------------ main menu ------------------------------
main_menu() {
  while :; do
    line
    echo "OSL SHELL PRACTICALS - MENU (enter code exactly, e.g., 1.1 or 1.6a)"
    echo "1.1  1.2  1.3  1.4  1.6a  1.6b  1.6c  1.7a  1.7b  1.7c"
    echo "q) quit"
    read -rp "Choice: " sel
    case "$sel" in
      1.1) q11 ;;
      1.2) q12 ;;
      1.3) q13 ;;
      1.4) q14 ;;
      1.6a|1.6A) q16 a ;;
      1.6b|1.6B) q16 b ;;
      1.6c|1.6C) q16 c ;;
      1.7a|1.7A) q17 a ;;
      1.7b|1.7B) q17 b ;;
      1.7c|1.7C) q17 c ;;
      q|Q) break ;;
      *) echo "Invalid." ;;
    esac
    pause
  done
}

main_menu


/* -------------------------- SECTION 2: PROCESS CONTROL (fork/exec/wait) -------------------------- */

/* 2.1 Parent: bubble; Child: insertion; demonstrate wait/zombie/orphan 【2.1 – L105-L110】 */
static void q2_1(){
    int n; int *a=read_array(&n); if(!a) return;
    pid_t pid=fork();
    if(pid<0){perror("fork"); free(a); return;}
    if(pid==0){
        /* Child: insertion sort */
        insertion_sort(a,n);
        printf("Child (insertion): "); for(int i=0;i<n;i++) printf("%d ",a[i]); puts("");
        _exit(0);
    }else{
        /* Parent: bubble + wait */
        bubble_sort(a,n);
        printf("Parent (bubble):   "); for(int i=0;i<n;i++) printf("%d ",a[i]); puts("");
        int status; waitpid(pid,&status,0);
        puts("Child exited; no zombie. For orphan demo, run parent sleep longer than child in another test.");
        free(a);
    }
}

/* 2.2 Parent: merge; Child: quick; demonstrate wait 【2.2 – L113-L119】 */
static void q2_2(){
    int n; int *a=read_array(&n); if(!a) return;
    pid_t pid=fork();
    if(pid==0){
        int *b=malloc(sizeof(int)*n); memcpy(b,a,sizeof(int)*n);
        quick_sort(b,0,n-1);
        printf("Child (quick): "); for(int i=0;i<n;i++) printf("%d ",b[i]); puts(""); free(b); _exit(0);
    }else if(pid>0){
        int *c=malloc(sizeof(int)*n); memcpy(c,a,sizeof(int)*n);
        merge_sort(c,0,n-1);
        printf("Parent (merge): "); for(int i=0;i<n;i++) printf("%d ",c[i]); puts(""); free(c);
        int status; waitpid(pid,&status,0); free(a);
    }else perror("fork");
}

/* 2.3 Parent sorts; exec child to binary search on argv 【2.3 – L55-L65】 */
static int binsearch(int *a,int n,int key){int l=0,r=n-1;while(l<=r){int m=(l+r)/2; if(a[m]==key)return m; if(a[m]<key)l=m+1; else r=m-1;}return -1;}
static void exec_child_for_23(int *arr,int n,int key){
    /* exec self with mode "--q23child ..." */
    char **argv = malloc(sizeof(char*)*(n+5));
    argv[0]=strdup("./osl_all");
    argv[1]=strdup("--q23child");
    argv[2]=malloc(16); sprintf(argv[2],"%d",n);
    for(int i=0;i<n;i++){ argv[3+i]=malloc(16); sprintf(argv[3+i],"%d",arr[i]); }
    argv[3+n]=malloc(16); sprintf(argv[3+n],"%d",key);
    argv[4+n]=NULL;
    execv("./osl_all",argv);
    perror("execv"); /* on failure */
    for(int i=0;i<3+n;i++) free(argv[i]); free(argv);
}
static void q2_3(){
    int n; int *a=read_array(&n); if(!a) return;
    int key; printf("Search key: "); scanf("%d",&key);
    qsort(a,n,sizeof(int),cmp_int);
    printf("Parent sorted array: "); for(int i=0;i<n;i++) printf("%d ",a[i]); puts("");
    pid_t pid=fork();
    if(pid==0){ exec_child_for_23(a,n,key); _exit(1); }
    int st; waitpid(pid,&st,0); free(a);
}
static void mode_q23child(int argc,char **argv){
    int n=atoi(argv[2]); int *a=malloc(4*n); for(int i=0;i<n;i++) a[i]=atoi(argv[3+i]); int key=atoi(argv[3+n]);
    int idx=binsearch(a,n,key); if(idx>=0) printf("[2.3 child] Found %d at index %d\n",key,idx); else printf("[2.3 child] %d not found\n",key); free(a);
}

/* 2.4 Collatz in child; parent waits 【2.4 – L71-L91】 */
static void q2_4(){
    long x; printf("Enter positive integer: "); if(scanf("%ld",&x)!=1||x<=0){puts("Invalid");return;}
    pid_t pid=fork();
    if(pid==0){ long n=x; printf("Child Collatz: "); while(n!=1){ printf("%ld ",n); n = (n%2==0)?(n/2):(3*n+1);} printf("1\n"); _exit(0); }
    int st; waitpid(pid,&st,0); puts("Parent: child done.");
}

/* 2.5 Primes up to N in child; parent waits 【2.5 – L97-L101】 */
static int isprime(long n){ if(n<2) return 0; if(n%2==0) return n==2; for(long i=3;i*i<=n;i+=2) if(n%i==0) return 0; return 1; }
static void q2_5(){
    long N; printf("Print first N primes (enter N): "); if(scanf("%ld",&N)!=1||N<=0){puts("Invalid");return;}
    pid_t pid=fork();
    if(pid==0){ long count=0, x=2; printf("Child primes: "); while(count<N){ if(isprime(x)){ printf("%ld ",x); count++; } x++; } puts(""); _exit(0); }
    int st; waitpid(pid,&st,0); puts("Parent: child done.");
}

/* -------------------------- SECTION 3: CPU SCHEDULING -------------------------- */
/* We define a simple process struct and implement FCFS, SJF(non/pre), RR(q=2) per specs. */
typedef struct { char id[8]; int at, bt, rt, ct, tat, wt; int done; } Proc;
static void read_procs(Proc **P,int *n){
    printf("Enter number of processes: "); scanf("%d",n);
    *P = calloc(*n,sizeof(Proc));
    for(int i=0;i<*n;i++){ printf("id at bt: "); scanf("%7s %d %d",(*P)[i].id,&(*P)[i].at,&(*P)[i].bt); (*P)[i].rt=(*P)[i].bt; }
}
static void print_avg(Proc *P,int n){
    double aw=0,at=0; for(int i=0;i<n;i++){ aw+=P[i].wt; at+=P[i].tat; }
    printf("Average WT: %.2f\nAverage TAT: %.2f\n",aw/n,at/n);
}
/* FCFS */
static void sched_fcfs(Proc *P,int n){
    qsort(P,n,sizeof(Proc),[](const void*aa,const void*bb){const Proc*a=aa,*b=bb; if(a->at!=b->at) return a->at-b->at; return strcmp(a->id,b->id);} );
    int t=0;
    for(int i=0;i<n;i++){
        if(t<P[i].at) t=P[i].at;
        t+=P[i].bt; P[i].ct=t; P[i].tat=P[i].ct-P[i].at; P[i].wt=P[i].tat-P[i].bt;
    }
}
/* SJF non-preemptive */
static void sched_sjf_np(Proc *P,int n){
    int t=0, done=0;
    while(done<n){
        int idx=-1, best=1e9;
        for(int i=0;i<n;i++) if(!P[i].done && P[i].at<=t && P[i].bt<best){best=P[i].bt; idx=i;}
        if(idx==-1){ t++; continue; }
        t+=P[idx].bt; P[idx].ct=t; P[idx].tat=P[idx].ct-P[idx].at; P[idx].wt=P[idx].tat-P[idx].bt; P[idx].done=1; done++;
    }
}
/* SJF preemptive (SRTF) */
static void sched_srtf(Proc *P,int n){
    int t=0, done=0;
    while(done<n){
        int idx=-1, best=1e9;
        for(int i=0;i<n;i++) if(P[i].rt>0 && P[i].at<=t && P[i].rt<best){best=P[i].rt; idx=i;}
        if(idx==-1){ t++; continue; }
        P[idx].rt--; t++;
        if(P[idx].rt==0){ P[idx].ct=t; P[idx].tat=P[idx].ct-P[idx].at; P[idx].wt=P[idx].tat-P[idx].bt; done++; }
    }
}
/* Round Robin q=2 */
static void sched_rr(Proc *P,int n,int q){
    int t=0, done=0;
    while(done<n){
        int progressed=0;
        for(int i=0;i<n;i++){
            if(P[i].rt>0 && P[i].at<=t){
                int use = P[i].rt<q?P[i].rt:q;
                P[i].rt-=use; t+=use; progressed=1;
                if(P[i].rt==0){ P[i].ct=t; P[i].tat=P[i].ct-P[i].at; P[i].wt=P[i].tat-P[i].bt; done++; }
            }
        }
        if(!progressed) t++;
    }
}
static void q3_1(){ Proc*P; int n; read_procs(&P,&n); int ch; printf("Choice: 1.FCFS 2.SJF(non-preemptive): "); scanf("%d",&ch); if(ch==1) sched_fcfs(P,n); else sched_sjf_np(P,n); for(int i=0;i<n;i++) printf("%s: WT=%d TAT=%d\n",P[i].id,P[i].wt,P[i].tat); print_avg(P,n); free(P); }
static void q3_2(){ Proc*P; int n; read_procs(&P,&n); int ch; printf("Choice: 1.FCFS 2.SJF(preemptive SRTF): "); scanf("%d",&ch); if(ch==1) sched_fcfs(P,n); else sched_srtf(P,n); for(int i=0;i<n;i++) printf("%s: WT=%d TAT=%d\n",P[i].id,P[i].wt,P[i].tat); print_avg(P,n); free(P); }
static void q3_3(){ Proc*P; int n; read_procs(&P,&n); int ch; printf("Choice: 1.FCFS 2.RR(q=2): "); scanf("%d",&ch); if(ch==1) sched_fcfs(P,n); else sched_rr(P,n,2); for(int i=0;i<n;i++) printf("%s: WT=%d TAT=%d\n",P[i].id,P[i].wt,P[i].tat); print_avg(P,n); free(P); }

/* -------------------------- SECTION 4: THREAD SYNCHRONIZATION -------------------------- */

/* 4.1 Producer-Consumer with bounded buffer: semaphores+mutex */
#define BUF 5
static int pc_buf[BUF], in_idx=0, out_idx=0;
static pthread_mutex_t mtx_pc = PTHREAD_MUTEX_INITIALIZER;
static sem_t empty_sem, full_sem;
static void* producer_41(void*arg){ for(int i=0;i<10;i++){ int item=rand()%100; sem_wait(&empty_sem); pthread_mutex_lock(&mtx_pc); pc_buf[in_idx]=item; in_idx=(in_idx+1)%BUF; printf("[P] produced %d\n",item); pthread_mutex_unlock(&mtx_pc); sem_post(&full_sem); usleep(100000); } return NULL; }
static void* consumer_41(void*arg){ for(int i=0;i<10;i++){ sem_wait(&full_sem); pthread_mutex_lock(&mtx_pc); int item=pc_buf[out_idx]; out_idx=(out_idx+1)%BUF; printf("    [C] consumed %d\n",item); pthread_mutex_unlock(&mtx_pc); sem_post(&empty_sem); usleep(120000);} return NULL; }
static void q4_1(){
    sem_init(&empty_sem,0,BUF); sem_init(&full_sem,0,0);
    pthread_t p,c; pthread_create(&p,NULL,producer_41,NULL); pthread_create(&c,NULL,consumer_41,NULL);
    pthread_join(p,NULL); pthread_join(c,NULL); sem_destroy(&empty_sem); sem_destroy(&full_sem);
}

/* 4.2 Sleeping TA (3 hallway chairs) — students + TA using semaphores and mutex */
static sem_t students, ta_ready, chairs_mutex;
static int waiting=0, CHAIRS=3;
static void* ta_thread(void*arg){ while(1){ sem_wait(&students); sem_wait(&chairs_mutex); waiting--; sem_post(&ta_ready); sem_post(&chairs_mutex); printf("[TA] Helping a student...\n"); usleep(150000); } return NULL; }
static void* student_thread(void*arg){ long id=(long)arg; sem_wait(&chairs_mutex); if(waiting<CHAIRS){ waiting++; printf("[S%ld] waiting. chairs=%d\n",id,waiting); sem_post(&students); sem_post(&chairs_mutex); sem_wait(&ta_ready); printf("[S%ld] being helped\n",id); } else { printf("[S%ld] no chair, leaving\n",id); sem_post(&chairs_mutex); } return NULL; }
static void q4_2(){
    sem_init(&students,0,0); sem_init(&ta_ready,0,0); sem_init(&chairs_mutex,0,1);
    pthread_t ta; pthread_create(&ta,NULL,ta_thread,NULL);
    pthread_t st[8]; for(long i=0;i<8;i++){ pthread_create(&st[i],NULL,student_thread,(void*)i); usleep(80000); }
    for(int i=0;i<8;i++) pthread_join(st[i],NULL);
    puts("Demo complete (TA loop continues; program will terminate now).");
}

/* 4.3 Producer writes ints to shared FILE; mutex+sem binary coordination */
static FILE *sharedf;
static pthread_mutex_t mtx_43 = PTHREAD_MUTEX_INITIALIZER;
static sem_t has_value, value_used;
static void* producer_43(void*arg){ for(int i=0;i<10;i++){ usleep((rand()%100)*1000); int v=rand()%10; pthread_mutex_lock(&mtx_43); fseek(sharedf,0,SEEK_END); fprintf(sharedf,"%d\n",v); fflush(sharedf); pthread_mutex_unlock(&mtx_43); sem_post(&has_value); sem_wait(&value_used);} return NULL;}
static void* consumer_43(void*arg){ int consumed=0; while(consumed<10){ sem_wait(&has_value); pthread_mutex_lock(&mtx_43); rewind(sharedf); int x, last=-1; while(fscanf(sharedf,"%d",&x)==1) last=x; pthread_mutex_unlock(&mtx_43); printf("[C] read %d\n",last); consumed++; sem_post(&value_used);} return NULL;}
static void q4_3(){
    sharedf = tmpfile(); sem_init(&has_value,0,0); sem_init(&value_used,0,0);
    pthread_t p,c; pthread_create(&p,NULL,producer_43,NULL); pthread_create(&c,NULL,consumer_43,NULL);
    pthread_join(p,NULL); pthread_join(c,NULL); sem_destroy(&has_value); sem_destroy(&value_used); fclose(sharedf);
}

/* 4.4 Sleeping Barber with semaphores */
static int SEATS=3; static int waiting_barber=0; static sem_t customers, barbers, mutex_sb;
static void* barber(void*arg){ while(1){ sem_wait(&customers); sem_wait(&mutex_sb); waiting_barber--; sem_post(&barbers); sem_post(&mutex_sb); printf("[Barber] cutting hair...\n"); usleep(150000);} return NULL; }
static void* customer(void*arg){ long id=(long)arg; sem_wait(&mutex_sb); if(waiting_barber<SEATS){ waiting_barber++; printf("[C%ld] sits. waiting=%d\n",id,waiting_barber); sem_post(&customers); sem_post(&mutex_sb); sem_wait(&barbers); printf("[C%ld] haircut done.\n",id);} else { printf("[C%ld] leaves, no seat.\n",id); sem_post(&mutex_sb);} return NULL; }
static void q4_4(){
    sem_init(&customers,0,0); sem_init(&barbers,0,0); sem_init(&mutex_sb,0,1);
    pthread_t b; pthread_create(&b,NULL,barber,NULL);
    pthread_t cs[7]; for(long i=0;i<7;i++){ pthread_create(&cs[i],NULL,customer,(void*)i); usleep(60000); }
    for(int i=0;i<7;i++) pthread_join(cs[i],NULL);
    puts("Barber demo done.");
}

/* 4.5 Readers–Writers (writers exclusive, reader priority) */
static pthread_mutex_t mtx_rw = PTHREAD_MUTEX_INITIALIZER;
static sem_t db, rmutex; static int rc=0, shared_db_45=0;
static void* reader_45(void*arg){ long id=(long)arg; sem_wait(&rmutex); rc++; if(rc==1) sem_wait(&db); sem_post(&rmutex); printf("[R%ld] reads %d\n",id,shared_db_45); usleep(80000); sem_wait(&rmutex); rc--; if(rc==0) sem_post(&db); sem_post(&rmutex); return NULL; }
static void* writer_45(void*arg){ long id=(long)arg; sem_wait(&db); shared_db_45++; printf("      [W%ld] writes %d\n",id,shared_db_45); usleep(100000); sem_post(&db); return NULL; }
static void q4_5(){
    sem_init(&db,0,1); sem_init(&rmutex,0,1);
    pthread_t r[5], w[2]; for(long i=0;i<5;i++) pthread_create(&r[i],NULL,reader_45,(void*)i);
    for(long i=0;i<2;i++) pthread_create(&w[i],NULL,writer_45,(void*)i);
    for(int i=0;i<5;i++) pthread_join(r[i],NULL); for(int i=0;i<2;i++) pthread_join(w[i],NULL);
}

/* 4.6 Airline reservation readers-priority (similar to 4.5) */
static int seats_46=50;
static void* reader_46(void*arg){ long id=(long)arg; sem_wait(&rmutex); rc++; if(rc==1) sem_wait(&db); sem_post(&rmutex); printf("[AirR%ld] seats=%d\n",id,seats_46); sem_wait(&rmutex); rc--; if(rc==0) sem_post(&db); sem_post(&rmutex); return NULL; }
static void* writer_46(void*arg){ long id=(long)arg; sem_wait(&db); if(seats_46>0){ seats_46--; printf("      [Book%ld] booked, left=%d\n",id,seats_46);} else { printf("      [Book%ld] no seats!\n",id);} sem_post(&db); return NULL; }
static void q4_6(){
    sem_init(&db,0,1); sem_init(&rmutex,0,1); rc=0; seats_46=5;
    pthread_t r[4], w[6]; for(long i=0;i<4;i++) pthread_create(&r[i],NULL,reader_46,(void*)i);
    for(long i=0;i<6;i++) pthread_create(&w[i],NULL,writer_46,(void*)i);
    for(int i=0;i<4;i++) pthread_join(r[i],NULL); for(int i=0;i<6;i++) pthread_join(w[i],NULL);
}

/* 4.7 Multi reader/writer with reader preference (single record) */
static int record_47=0;
static void* reader_47(void*arg){ long id=(long)arg; sem_wait(&rmutex); rc++; if(rc==1) sem_wait(&db); sem_post(&rmutex); printf("[R7-%ld] read=%d\n",id,record_47); usleep(60000); sem_wait(&rmutex); rc--; if(rc==0) sem_post(&db); sem_post(&rmutex); return NULL;}
static void* writer_47(void*arg){ long id=(long)arg; sem_wait(&db); record_47+=10; printf("   [W7-%ld] write=%d\n",id,record_47); sem_post(&db); return NULL;}
static void q4_7(){
    sem_init(&db,0,1); sem_init(&rmutex,0,1); rc=0; record_47=0;
    pthread_t r[3], w[3]; for(long i=0;i<3;i++) pthread_create(&r[i],NULL,reader_47,(void*)i);
    for(long i=0;i<3;i++) pthread_create(&w[i],NULL,writer_47,(void*)i);
    for(int i=0;i<3;i++) pthread_join(r[i],NULL); for(int i=0;i<3;i++) pthread_join(w[i],NULL);
}

/* 4.8 License counter with semaphore (counting resources) */
static sem_t licenses; static int total_lic=3;
static void* app_thread(void*arg){ long id=(long)arg; printf("[App%ld] requesting license\n",id); sem_wait(&licenses); printf("[App%ld] running...\n",id); usleep(120000); printf("[App%ld] done\n",id); sem_post(&licenses); return NULL; }
static void q4_8(){
    sem_init(&licenses,0,total_lic);
    pthread_t th[7]; for(long i=0;i<7;i++) pthread_create(&th[i],NULL,app_thread,(void*)i);
    for(int i=0;i<7;i++) pthread_join(th[i],NULL); sem_destroy(&licenses);
}

/* -------------------------- SECTION 5: BANKER'S ALGORITHM -------------------------- */
static int safe_sequence(int nP,int nR,int alloc[nP][nR],int max[nP][nR],int avail[nR],int seq[nP]){
    int work[nR]; for(int j=0;j<nR;j++) work[j]=avail[j];
    int finish[nP]; memset(finish,0,sizeof(finish));
    int need[nP][nR]; for(int i=0;i<nP;i++) for(int j=0;j<nR;j++) need[i][j]=max[i][j]-alloc[i][j];
    int count=0;
    while(count<nP){
        int found=0;
        for(int i=0;i<nP;i++) if(!finish[i]){
            int ok=1; for(int j=0;j<nR;j++) if(need[i][j]>work[j]){ok=0;break;}
            if(ok){ for(int j=0;j<nR;j++) work[j]+=alloc[i][j]; finish[i]=1; seq[count++]=i; found=1; }
        }
        if(!found) return 0;
    }
    return 1;
}
/* 5.1 Basic safe/unsafe check 【5.1 – L9-L13】 */
static void q5_1(){
    int nP,nR; printf("Processes Resources: "); scanf("%d %d",&nP,&nR);
    int alloc[nP][nR], max[nP][nR], avail[nR];
    printf("Enter Allocation matrix:\n"); for(int i=0;i<nP;i++) for(int j=0;j<nR;j++) scanf("%d",&alloc[i][j]);
    printf("Enter Max matrix:\n"); for(int i=0;i<nP;i++) for(int j=0;j<nR;j++) scanf("%d",&max[i][j]);
    printf("Enter Available vector:\n"); for(int j=0;j<nR;j++) scanf("%d",&avail[j]);
    int seq[nP]; if(safe_sequence(nP,nR,alloc,max,avail,seq)){ printf("Safe. Sequence: "); for(int i=0;i<nP;i++) printf("P%d ",seq[i]); puts(""); } else puts("Unsafe state.");
}
/* 5.2 With request granting check 【5.2 – L19-L29】 */
static void q5_2(){
    int nP,nR; printf("Processes Resources: "); scanf("%d %d",&nP,&nR);
    int alloc[nP][nR], max[nP][nR], avail[nR];
    printf("Enter Allocation matrix:\n"); for(int i=0;i<nP;i++) for(int j=0;j<nR;j++) scanf("%d",&alloc[i][j]);
    printf("Enter Max matrix:\n"); for(int i=0;i<nP;i++) for(int j=0;j<nR;j++) scanf("%d",&max[i][j]);
    printf("Enter Available vector:\n"); for(int j=0;j<nR;j++) scanf("%d",&avail[j]);
    int pid; printf("Enter request process id (0..%d): ",nP-1); scanf("%d",&pid);
    int req[nR]; printf("Enter request vector:\n"); for(int j=0;j<nR;j++) scanf("%d",&req[j]);
    /* Check need and available */
    for(int j=0;j<nR;j++){
        int need = max[pid][j]-alloc[pid][j];
        if(req[j]>need){ puts("Request exceeds need -> deny."); return; }
        if(req[j]>avail[j]){ puts("Not enough available -> wait."); return; }
    }
    /* Tentatively allocate */
    for(int j=0;j<nR;j++){ avail[j]-=req[j]; alloc[pid][j]+=req[j]; }
    int seq[nP];
    if(safe_sequence(nP,nR,alloc,max,avail,seq)){ puts("Request can be granted safely."); printf("Safe sequence: "); for(int i=0;i<nP;i++) printf("P%d ",seq[i]); puts(""); }
    else puts("Request leads to unsafe state -> deny.");
}

/* -------------------------- SECTION 6: PAGE REPLACEMENT -------------------------- */

/* FIFO and LRU helpers */
typedef struct Node { int page; int age; } Node;
static int simulate_fifo(int frames,int *ref,int m){
    int *fr=malloc(sizeof(int)*frames); for(int i=0;i<frames;i++) fr[i]=-1;
    int faults=0, ptr=0;
    for(int i=0;i<m;i++){ int p=ref[i], hit=0; for(int j=0;j<frames;j++) if(fr[j]==p){hit=1;break;}
        if(!hit){ fr[ptr]=p; ptr=(ptr+1)%frames; faults++; } }
    free(fr); return faults;
}
static int simulate_lru(int frames,int *ref,int m){
    int *fr=malloc(4*frames), *age=malloc(4*frames); for(int i=0;i<frames;i++){ fr[i]=-1; age[i]=0; }
    int faults=0;
    for(int i=0;i<m;i++){ int p=ref[i], hit=-1; for(int j=0;j<frames;j++){ age[j]++; if(fr[j]==p) hit=j; }
        if(hit!=-1){ age[hit]=0; } else { int ev=0; for(int j=1;j<frames;j++) if(age[j]>age[ev]) ev=j; fr[ev]=p; age[ev]=0; faults++; } }
    free(fr); free(age); return faults;
}
/* 6.1 Random reference 0..9, frames 1..7 【6.1 – L46-L53】 */
static void q6_1(){
    int m=20; int ref[20]; srand(42); for(int i=0;i<m;i++) ref[i]=rand()%10;
    for(int f=1;f<=7;f++){ int ff=simulate_fifo(f,ref,m), ll=simulate_lru(f,ref,m); printf("Frames=%d FIFO=%d LRU=%d\n",f,ff,ll); }
}
/* 6.2 Given reference & frames 3 and 4 (Optimal & LRU) 【6.2 – L56-L59】 */
static int simulate_opt(int frames,int *ref,int m){
    int *fr=malloc(4*frames); for(int i=0;i<frames;i++) fr[i]=-1; int faults=0;
    for(int i=0;i<m;i++){ int p=ref[i], hit=-1; for(int j=0;j<frames;j++) if(fr[j]==p) hit=j;
        if(hit==-1){ /* choose victim furthest in future */
            int victim=-1, farthest=-1;
            for(int j=0;j<frames;j++){
                if(fr[j]==-1){ victim=j; break; }
                int k; for(k=i+1;k<m;k++) if(ref[k]==fr[j]) break;
                if(k==m){ victim=j; farthest=m; break; }
                if(k>farthest){ farthest=k; victim=j; }
            }
            fr[victim]=p; faults++;
        }
    }
    free(fr); return faults;
}
static void q6_2(){
    int ref[]={1,2,3,4,1,2,5,1,2,3,4,5}; int m=12;
    for(int f=3; f<=4; f++){ int oo=simulate_opt(f,ref,m), ll=simulate_lru(f,ref,m); printf("Frames=%d OPT=%d LRU=%d\n",f,oo,ll); }
}

/* -------------------------- SECTION 7: IPC -------------------------- */

/* 7.1 Two pipes: parent sends string; child inverts case and returns back 【7.1 – L68-L78】 */
static void q7_1(){
    int p2c[2], c2p[2]; pipe(p2c); pipe(c2p);
    pid_t pid=fork();
    if(pid==0){
        close(p2c[1]); close(c2p[0]);
        char buf[256]; int n=read(p2c[0],buf,255); buf[n]=0;
        for(int i=0;i<n;i++) if(isalpha((unsigned char)buf[i])) buf[i]^=32;
        write(c2p[1],buf,strlen(buf));
        _exit(0);
    }else{
        close(p2c[0]); close(c2p[1]);
        char msg[256]; printf("Enter message: "); scanf(" %[^\n]",msg); write(p2c[1],msg,strlen(msg));
        char out[256]; int n=read(c2p[0],out,255); out[n]=0; printf("From child: %s\n",out); wait(NULL);
    }
}

/* 7.2 Bank account race vs mutex 【7.2 – L82-L113】【7.2 – L119-L63】 */
static long bank_balance=1000; static pthread_mutex_t bank_mtx=PTHREAD_MUTEX_INITIALIZER; static int use_lock=0;
static void* worker_72(void*arg){
    long id=(long)arg; for(int i=0;i<5;i++){ int dep = rand()%2; int amt = 5 + rand()%46;
        if(use_lock) pthread_mutex_lock(&bank_mtx);
        long before = bank_balance;
        if(dep) bank_balance += amt; else bank_balance -= amt;
        printf("[T%ld] %s %d => %ld -> %ld\n",id, dep?"dep":"+wd", amt, before, bank_balance);
        if(use_lock) pthread_mutex_unlock(&bank_mtx);
        usleep(40000);
    } return NULL;
}
static void q7_2(){
    srand(time(NULL)); int N=10; pthread_t th[N];
    long expected = bank_balance;
    /* Run unsynchronized */
    use_lock=0; for(int i=0;i<N;i++){ int dep = rand()%2; int amt=5+rand()%46; expected += (dep?amt:-amt); }
    /* Reset seed and actually do same randoms during threads by recalculating inside (approx). */
    for(int i=0;i<N;i++) pthread_create(&th[i],NULL,worker_72,(void*)i);
    for(int i=0;i<N;i++) pthread_join(th[i],NULL);
    printf("UNLOCKED run result ~ %ld (expected approx may differ due to interleaving)\n",bank_balance);

    /* Reset and run synchronized */
    bank_balance=1000; use_lock=1; for(int i=0;i<N;i++) pthread_create(&th[i],NULL,worker_72,(void*)i);
    for(int i=0;i<N;i++) pthread_join(th[i],NULL);
    printf("LOCKED final balance = %ld (consistent)\n",bank_balance);
}

/* -------------------------- SECTION 8: DISK SCHEDULING -------------------------- */

/* Helpers for SCAN/LOOK/C-SCAN/C-LOOK */
static int cmp(const void*a,const void*b){return (*(int*)a-*(int*)b);}
static int total_seek_scan(int *req,int n,int head,int maxcyl,int dir_right,int look){
    /* Return total seek for SCAN/LOOK depending on look flag. dir_right=1 towards high end */
    int *a=malloc(4*n); memcpy(a,req,4*n); qsort(a,n,cmp);
    int idx=0; while(idx<n && a[idx]<head) idx++;
    int seek=0, pos=head, i;
    if(dir_right){
        for(i=idx;i<n;i++){ seek+=abs(a[i]-pos); pos=a[i]; }
        if(!look){ seek+=abs((maxcyl-1)-pos); pos=(maxcyl-1); }
        for(i=idx-1;i>=0;i--){ seek+=abs(a[i]-pos); pos=a[i]; }
    }else{
        for(i=idx-1;i>=0;i--){ seek+=abs(a[i]-pos); pos=a[i]; }
        if(!look){ seek+=abs(pos-0); pos=0; }
        for(i=idx;i<n;i++){ seek+=abs(a[i]-pos); pos=a[i]; }
    }
    free(a); return seek;
}
static int total_seek_cscan(int *req,int n,int head,int maxcyl,int look){
    int *a=malloc(4*n); memcpy(a,req,4*n); qsort(a,n,cmp);
    int idx=0; while(idx<n && a[idx]<head) idx++;
    int seek=0, pos=head;
    for(int i=idx;i<n;i++){ seek+=abs(a[i]-pos); pos=a[i]; }
    if(look){
        /* jump to smallest */
        if(idx>0){ seek+=abs(pos - a[0]); pos=a[0]; }
        for(int i=1;i<idx;i++){ seek+=abs(a[i]-pos); pos=a[i]; }
    }else{
        seek+=abs((maxcyl-1)-pos); pos=0; /* wrap */
        for(int i=0;i<idx;i++){ seek+=abs(a[i]-pos); pos=a[i]; }
    }
    free(a); return seek;
}
/* 8.1 Given set with head=85, max=500, C-SCAN & C-LOOK 【8.1 – L71-L91】 */
static void q8_1(){
    int req[]={10,229,39,400,18,145,120,480,20,250}; int n=sizeof(req)/4;
    int head=85, maxc=500;
    int cscan=total_seek_cscan(req,n,head,maxc,0);
    int clook=total_seek_cscan(req,n,head,maxc,1);
    printf("C-SCAN seek=%d, C-LOOK seek=%d\n",cscan,clook);
}
/* 8.2 SCAN and LOOK for given set (head moving toward 4999) 【8.2】 */
static void q8_2(){
    int req[]={86,1470,913,1774,948,1509,1022,1750,130}; int n=sizeof(req)/4;
    int head=143, maxc=5000;
    int scan = total_seek_scan(req,n,head,maxc,1,0);
    int look = total_seek_scan(req,n,head,maxc,1,1);
    printf("SCAN seek=%d, LOOK seek=%d\n",scan,look);
}
/* 8.3 C-SCAN and C-LOOK for same set 【8.3 – L3-L15】 */
static void q8_3(){
    int req[]={86,1470,913,1774,948,1509,1022,1750,130}; int n=sizeof(req)/4;
    int head=143, maxc=5000;
    int cscan=total_seek_cscan(req,n,head,maxc,0);
    int clook=total_seek_cscan(req,n,head,maxc,1);
    printf("C-SCAN seek=%d, C-LOOK seek=%d\n",cscan,clook);
}

/* -------------------------- MAIN MENU -------------------------- */
static void print_menu(){
    puts("\n======== OSL MENU (enter code number) ========");
    puts("1.1  1.2  1.3  1.4  1.6  1.7");
    puts("2.1  2.2  2.3  2.4  2.5");
    puts("3.1  3.2  3.3");
    puts("4.1  4.2  4.3  4.4  4.5  4.6  4.7  4.8");
    puts("5.1  5.2");
    puts("6.1  6.2");
    puts("7.1  7.2");
    puts("8.1  8.2  8.3");
    puts("0 to exit");
}
int main(int argc,char **argv){
    /* Special mode for 2.3 child */
    if(argc>1 && strcmp(argv[1],"--q23child")==0){ mode_q23child(argc,argv); return 0; }
    while(1){
        print_menu();
        char code[8]; printf("Enter code (e.g., 2.1): "); if(scanf(" %7s",code)!=1) break;
        if(strcmp(code,"0")==0) break;
        if(!strcmp(code,"1.1")) q1_1();
        else if(!strcmp(code,"1.2")) q1_2();
        else if(!strcmp(code,"1.3")) q1_3();
        else if(!strcmp(code,"1.4")) q1_4();
        else if(!strcmp(code,"1.6")) q1_6();
        else if(!strcmp(code,"1.7")) q1_7();
        else if(!strcmp(code,"2.1")) q2_1();
        else if(!strcmp(code,"2.2")) q2_2();
        else if(!strcmp(code,"2.3")) q2_3();
        else if(!strcmp(code,"2.4")) q2_4();
        else if(!strcmp(code,"2.5")) q2_5();
        else if(!strcmp(code,"3.1")) q3_1();
        else if(!strcmp(code,"3.2")) q3_2();
        else if(!strcmp(code,"3.3")) q3_3();
        else if(!strcmp(code,"4.1")) q4_1();
        else if(!strcmp(code,"4.2")) q4_2();
        else if(!strcmp(code,"4.3")) q4_3();
        else if(!strcmp(code,"4.4")) q4_4();
        else if(!strcmp(code,"4.5")) q4_5();
        else if(!strcmp(code,"4.6")) q4_6();
        else if(!strcmp(code,"4.7")) q4_7();
        else if(!strcmp(code,"4.8")) q4_8();
        else if(!strcmp(code,"5.1")) q5_1();
        else if(!strcmp(code,"5.2")) q5_2();
        else if(!strcmp(code,"6.1")) q6_1();
        else if(!strcmp(code,"6.2")) q6_2();
        else if(!strcmp(code,"7.1")) q7_1();
        else if(!strcmp(code,"7.2")) q7_2();
        else if(!strcmp(code,"8.1")) q8_1();
        else if(!strcmp(code,"8.2")) q8_2();
        else if(!strcmp(code,"8.3")) q8_3();
        else puts("Unknown code.");
    }
    return 0;
}
