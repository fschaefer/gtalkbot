/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
#ifndef _PLUGIN_H_
#define _PLUGIN_H_

/* 返回插件要求的参数个数，-1表示没有这个插件 */
int				bot_get_plugin(const char *plugin_name, char **exec_path);
/* 运行并获取结果 */
const char*		bot_run_plugin(const char *plugin_name, const char *args);

#endif /* _PLUGIN_H_ */
