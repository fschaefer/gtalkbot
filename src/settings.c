/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "settings.h"
#include "log.h"

typedef struct settings {
	char user[1024];
	char pass[1024];
	int plugin_cnt;
	
} Settings;


static Settings s;

static void
init_settings() {
	static int inited = 0;
	char buf[2048];
	FILE * xmpp_conf;
	FILE * plugins_conf;
	
	if (inited) return;

	/* 用户配置 */
	xmpp_conf = fopen(XMPP_CONF, "r");
	while (fgets(buf, sizeof(buf), xmpp_conf)) {
		char *tmp;
		tmp = strchr(buf, '#');
		if (tmp) *tmp = '\0';
		tmp = buf;
		while (isblank(*tmp)) tmp++;
		if (!strncasecmp(tmp, "username", 8)) {
			char *str = strtok(tmp, "\"");
			str = strtok(NULL, "\"");
			snprintf(s.user, sizeof(s.user), "%s", str);
		} else if (!strncasecmp(tmp, "password", 8)) {
			char *str = strtok(tmp, "\"");
			str = strtok(NULL, "\"");
			snprintf(s.pass, sizeof(s.pass), "%s", str);
		}
	}

	bot_debug("username: %s", s.user);
	bot_debug("password: %s", s.pass);

	/* 插件配置 */
	plugins_conf = fopen(PLUGIN_CONF, "r");
	while (fgets(buf, sizeof(buf), plugins_conf)) {
		char *tmp;
		tmp = strchr(buf, '#');
		if (tmp) *tmp = '\0';
		tmp = buf;
		while (isblank(*tmp)) tmp++;
		if (!strncasecmp(tmp, "plugins", 6)) {
			char *str = strtok(tmp, "\"");
			str = strtok(NULL, "\"");
		}
	}
	

	
	inited = 1;
	return;
}

const char *
bot_get_username() {
	init_settings();
	return s.user;
}

const char *
bot_get_password() {
	init_settings();
	return s.pass;
}
