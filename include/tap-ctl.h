/* 
 * Copyright (C) Citrix Systems Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2.1 only
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __TAP_CTL_H__
#define __TAP_CTL_H__

#include <syslog.h>
#include <errno.h>
#include <sys/time.h>
#include <tapdisk-message.h>
#include <list.h>

extern int tap_ctl_debug;

#ifdef TAPCTL
#define DBG(_f, _a...)				\
	do {					\
		if (tap_ctl_debug)		\
			printf(_f, ##_a);	\
	} while (0)

#define DPRINTF(_f, _a...) syslog(LOG_INFO, _f, ##_a)
#define EPRINTF(_f, _a...) syslog(LOG_ERR, "tap-err:%s: " _f, __func__, ##_a)
#define  PERROR(_f, _a...) syslog(LOG_ERR, "tap-err:%s: " _f ": %s", __func__, ##_a, \
				  strerror(errno))
#endif

void tap_ctl_version(int *major, int *minor);
int tap_ctl_kernel_version(int *major, int *minor);

int tap_ctl_check_blktap(const char **message);
int tap_ctl_check_version(const char **message);
int tap_ctl_check(const char **message);

int tap_ctl_connect(const char *path, int *socket);
int tap_ctl_connect_id(int id, int *socket);
int tap_ctl_read_raw(int fd, void *buf, size_t sz, struct timeval *timeout);
int tap_ctl_read_message(int fd, tapdisk_message_t *message,
			 struct timeval *timeout);
int tap_ctl_write_message(int fd, tapdisk_message_t *message,
			  struct timeval *timeout);
int tap_ctl_send_and_receive(int fd, tapdisk_message_t *message,
			     struct timeval *timeout);
int tap_ctl_connect_send_and_receive(int id,
				     tapdisk_message_t *message,
				     struct timeval *timeout);
char *tap_ctl_socket_name(int id);

typedef struct {
	pid_t       pid;
	int         minor;
	int         state;
	char       *type;
	char       *path;

	struct list_head entry;
} tap_list_t;

#define tap_list_for_each_entry(_pos, _head) \
	list_for_each_entry(_pos, _head, entry)

#define tap_list_for_each_entry_safe(_pos, _n, _head) \
	list_for_each_entry_safe(_pos, _n, _head, entry)

int tap_ctl_list(struct list_head *list);
int tap_ctl_list_pid(pid_t pid, struct list_head *list);
void tap_ctl_list_free(struct list_head *list);

int tap_ctl_find_minor(const char *type, const char *path);

int tap_ctl_allocate(int *minor, char **devname);
int tap_ctl_free(const int minor);

int tap_ctl_create(const char *params, char **devname, int flags, 
		int prt_minor, char *secondary, int timeout);
int tap_ctl_destroy(const int id, const int minor, int force,
		    struct timeval *timeout);

int tap_ctl_spawn(void);
pid_t tap_ctl_get_pid(const int id);

int tap_ctl_attach(const int id, const int minor);
int tap_ctl_detach(const int id, const int minor);

int tap_ctl_open(const int id, const int minor, const char *params, int flags,
		const int prt_minor, const char *secondary, int timeout);
int tap_ctl_close(const int id, const int minor, const int force,
		  struct timeval *timeout);

int tap_ctl_pause(const int id, const int minor, struct timeval *timeout);
int tap_ctl_unpause(const int id, const int minor, const char *params,
		int flags, char *secondary);

ssize_t tap_ctl_stats(pid_t pid, int minor, char *buf, size_t size);
int tap_ctl_stats_fwrite(pid_t pid, int minor, FILE *out);

int tap_ctl_blk_major(void);

#endif
