#ifndef __MY_DISKMONITOR_SPACE_H
#define __MY_DISKMONITOR_SPACE_H

#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

void start_disk_monitor(gboolean check_now);
void stop_disk_monitor(void);

#ifdef __cplusplus
}
#endif

#endif /* __MY_DISKMONITOR_SPACE_H */
