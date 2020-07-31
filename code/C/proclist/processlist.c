// Author: lixiang kobe24_lixiang@126.com
// sudo apt install libgtop2-dev
// g++ processlist.c -o run `pkg-config --cflags --libs glib-2.0 libgtop-2.0 libsystemd`

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <systemd/sd-login.h>

#include <glib.h>
#include <glibtop.h>
#include <glibtop/proclist.h>
#include <glib/gprintf.h>
#include <glibtop/mem.h>
#include <glibtop/procstate.h>
#include <glibtop/procmem.h>
#include <glibtop/proctime.h>
#include <glibtop/procuid.h>
#include <glibtop/procargs.h>
#include <glibtop/sysinfo.h>

#include <string>// std::string
#include <iostream>// std::cout
#include <algorithm>// std::sort
using namespace std;

string make_string(char *c_str)
{
    if (!c_str) {
        return string();
    }
    string s(c_str);
    g_free(c_str);

    return s;
}

gchar *get_process_name(const gchar *cmd, const GStrv args)
{
    gchar *process_name = NULL;
    if (args) {
        for (int i = 0; i != 2 && args[i]; ++i) {
            process_name = g_path_get_basename(args[i]);
            if (g_str_has_prefix(process_name, cmd)) {
                return process_name;
            }
        }
    }

    process_name = g_strdup(cmd);
    return process_name;
}


void print_process_info(pid_t pid)
{
    const pid_t m_pid = pid;
    string m_user;
    gchar *m_name = NULL;
    gchar *m_tooltip = NULL;
    gchar *m_arguments = NULL;
    gchar *m_unit = NULL;
    gchar *m_session = NULL;
    gchar *m_seat = NULL;
    guint64 m_cpu_time;
    gulong m_start_time;
    guint m_status;
    gint m_nice;
    gulong m_mem;
    float m_mem_rate;
    guint m_uid;

    glibtop_proc_state procstate;
    glibtop_proc_uid procuid;
    glibtop_proc_time proctime;
    glibtop_proc_args procargs;
    glibtop_proc_mem procmem;
    glibtop_mem memory;
    gchar** args;

    glibtop_get_proc_state(&procstate, m_pid);
    glibtop_get_proc_uid (&procuid, m_pid);
    glibtop_get_proc_time(&proctime, m_pid);
    glibtop_get_proc_mem(&procmem, m_pid);
    glibtop_get_mem(&memory);

    args = glibtop_get_proc_argv(&procargs, m_pid, 0);

    m_status = procstate.state;
    m_nice = procuid.nice;
    m_name = get_process_name(procstate.cmd, static_cast<const GStrv>(args));

    std::string tooltip = make_string(g_strjoinv(" ", args));
    if (tooltip.empty())
        tooltip = procstate.cmd;
    m_tooltip = g_markup_escape_text(tooltip.c_str(), -1);
    m_arguments = g_strescape(tooltip.c_str(), "\\\"");
    g_strfreev(args);

    m_cpu_time = proctime.rtime;
    m_start_time = proctime.start_time;

    m_mem = procmem.resident - procmem.share;
    m_mem_rate = 100*(procmem.resident) /((memory.total)*1.0);
    m_uid = procstate.uid;

    struct passwd* pwd;
    pwd = getpwuid(m_uid);
    if (pwd && pwd->pw_name)
        m_user = string(pwd->pw_name);
    else {
        char username[16];
        g_sprintf(username, "%u", m_uid);
        m_user = string(username);
    }

    if (access("/run/systemd/seats/", F_OK) >= 0) {
        sd_pid_get_unit(m_pid, &m_unit);
        sd_pid_get_session(m_pid, &m_session);
        if (m_session != NULL) {
            sd_session_get_seat(m_session, &m_seat);
        }
    }

    printf("\tpid:%d, uid:%d, name:%s, user:%s, tooltip:%s, arguments:%s, unit:%s, session:%s, m_seat:%s, nice:%d, status:%d, start_time:%lu, memory:%s, mem rate:%0.3f%%\n", m_pid, m_uid, m_name, m_user.c_str(), m_tooltip, m_arguments, m_unit, m_session, m_seat, m_nice, m_status, m_start_time, g_format_size_full(m_mem, G_FORMAT_SIZE_IEC_UNITS), m_mem_rate);

    if (m_name) {
        g_free(m_name);
        m_name = NULL;
    }
    if (m_tooltip) {
        g_free(m_tooltip);
        m_tooltip = NULL;
    }
    if (m_arguments) {
        g_free(m_arguments);
        m_arguments = NULL;
    }
    if (m_unit) {
        g_free(m_unit);
        m_unit = NULL;
    }
    if (m_unit) {
        g_free(m_session);
        m_session = NULL;
    }
    if (m_unit) {
        g_free(m_seat);
        m_seat = NULL;
    }
}

void refresh_process_list(char *whose_processes)
{
    pid_t* pid_list;
    glibtop_proclist proclist;
    int which = 0;
    int arg = 0;

    if (strcmp(whose_processes, "all") == 0) {
        which = GLIBTOP_KERN_PROC_ALL;
        arg = 0;
    }
    else if (strcmp(whose_processes, "active") == 0) {
        which = GLIBTOP_KERN_PROC_ALL | GLIBTOP_EXCLUDE_IDLE;
        arg = 0;
    }
    else if (strcmp(whose_processes, "user") == 0) {
        which = GLIBTOP_KERN_PROC_UID;
        arg = getuid();
    }

    // https://developer.gnome.org/libgtop/stable/libgtop-Process-List.html#glibtop-get-proclist
    pid_list = glibtop_get_proclist(&proclist, which, arg);

    std::sort(pid_list, pid_list + proclist.number);

    printf("Process List Info:\n");
    for (guint i = 0; i < proclist.number; ++i) {
        print_process_info(pid_list[i]);
    }

    g_free (pid_list);
}

void print_cpu_number()
{
    int cpu_number = 0;
    const glibtop_sysinfo *info;
    info = glibtop_get_sysinfo();
    cpu_number = info->ncpu;
    printf("cpu number: %d\n", cpu_number);
}

int main()
{
    char whose_process[] = "user";

    glibtop_init();

    print_cpu_number();

    refresh_process_list(whose_process);

    glibtop_close();

    return 0;
}
