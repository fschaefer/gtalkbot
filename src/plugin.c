/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "plugin.h"
#include "log.h"

typedef struct plugins {
	char plugins[2048];
	char *content;
	size_t size;
	time_t mtime;
} Plugins;

static Plugins p;

static int
init_plugins_conf()
{
	int fd;
	FILE *fp;
	struct stat st;
	char buf[2048], *tmp;
	
	fd = open(PLUGIN_CONF, O_RDONLY);
	if (fd < 0) {
		perror("open");
		bot_warning("open %s failed", PLUGIN_CONF);
		return -1;
	}
	if (fstat(fd, &st) == -1) {
		perror("stat");
		bot_warning("state %s failed", PLUGIN_CONF);
		return -1;
	}

	/* 配置文件未被修改，不重新载入 */
	if (p.mtime == st.st_mtime) { 
		return 0;
	}

	if (p.content != NULL) {
		free(p.content);
	}
	p.content = (char *)malloc(st.st_size + 1);
	p.content[st.st_size] = '\0';
	if (read(fd, p.content, st.st_size) != st.st_size) {
		perror("read");
		bot_warning("read %s failed", PLUGIN_CONF);
		return -1;
	}


	p.size = st.st_size;
	p.mtime = st.st_mtime;

	fp = fmemopen(p.content, p.size, "r");
	while (fgets(buf, sizeof(buf), fp)) {
		tmp = strchr(buf, '#');
		if (tmp) *tmp = '\0';
		tmp = buf;
		while (isblank(*tmp)) tmp++;
		if (!strncasecmp(tmp, "plugins", 7)) {
			char *str = strtok(tmp, "\"");
			str = strtok(NULL, "\"");
			snprintf(p.plugins, sizeof(p.plugins), "%s", str);
			break;
		}
	}
	bot_debug("Plugins: %s", p.plugins);

	fclose(fp);
	close(fd);
	return 0;
}

/* 返回插件要求的参数个数，没有相应插件时返回-1 */
int
bot_get_plugin(const char *plugin_name, char **exec_path)
{
	int argc;
	char exec[2048];
	char buf[2048], *tmp;

	if (init_plugins_conf() < 0)
		return -1;

	argc = 0;
	*exec_path = NULL;
	
	if (strstr(p.plugins, plugin_name) == NULL)
		return -1;

	FILE *fp = fmemopen(p.content, p.size, "r");
	while (fgets(buf, sizeof(buf), fp)) {
		tmp = strchr(buf, '#');
		if (tmp) *tmp = '\0';
		tmp = buf;
		while (isblank(*tmp)) tmp++;
		if (!strncasecmp(tmp, plugin_name, strlen(plugin_name))) {
			if (!strncasecmp(tmp + strlen(plugin_name) + 1, "argc", 4)) {
				char *str = strtok(tmp, "\"");
				str = strtok(NULL, "\"");
				argc = atoi(str);
			}
			if (!strncasecmp(tmp + strlen(plugin_name) + 1, "exec", 4)) {
				char *str = strtok(tmp, "\"");
				str = strtok(NULL, "\"");
				snprintf(exec, sizeof(exec), "%s/%s", PLUGIN_DIR, str);
				*exec_path = exec;
			}
		}
	}
	fclose(fp);
	return argc;
}

/* 运行并获取结果 */
const char*
bot_run_plugin(const char *plugin_name, const char *args)
{
	int argc;
	char *buf;
	char *exec_path;
	FILE *pipe_bufeam;
	
	buf = (char *)malloc(2048);

	argc = bot_get_plugin(plugin_name, &exec_path);

	if (argc < 0 || exec_path == NULL) {
		snprintf(buf, sizeof(buf), "%s", CMD_NOT_FOUND);
		return buf;
	}

	bot_debug("plugin: %s", plugin_name);
	bot_debug("exec path: %s", exec_path);
	bot_debug("argc: %d", argc);

	if (argc == 0) {
		snprintf(buf, 2048, "%s", exec_path);
	} else {
		snprintf(buf, 2048, "%s %s", exec_path, args ?: "");
	}
	
	pipe_bufeam = popen(buf, "r");
	memset(buf, 0, 2048);
	fread(buf, 2048, 1, pipe_bufeam);
	pclose(pipe_bufeam);

	return buf;
}
