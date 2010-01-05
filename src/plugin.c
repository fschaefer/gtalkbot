/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "plugin.h"
#include "log.h"

/* 返回插件要求的参数个数，-1表示没有这个插件 */
int
bot_get_plugin(const char *plugin_name)
{
	bot_debug("Plugin:%s", plugin_name);
	return 0;
	if (strcmp(plugin_name, "weather"))
		return -1;
	return 0;
}

/* 运行并获取结果 */
const char*
bot_run_plugin(const char *plugin_name, const char *args)
{
	char *str = (char *)malloc(2048);
	memset(str, 0, 2048);
	snprintf(str, 2048, "%s %s", plugin_name, args == NULL ? "" : args);
	bot_debug("Command:%s", str);
	FILE *f = popen(str, "r");
	memset(str, 0, 2048);
	fread(str, 1024, 1, f);
	pclose(f);
	return str;
}
