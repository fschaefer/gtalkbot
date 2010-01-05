/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#define BOT_VERSION "0.1"

#define CONF_DIR	"etc"

#define PLUGIN_CONF	CONF_DIR"/plugins.conf"
#define XMPP_CONF	CONF_DIR"/xmpp.conf"

/* 获取 */
const char	*bot_get_username();
const char	*bot_get_password();

#endif /* _SETTINGS_H_ */
