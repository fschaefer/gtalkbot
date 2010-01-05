/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "handler.h"
#include "log.h"
#include "settings.h"
#include "plugin.h"


static int
bot_ver_handler(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata)
{
	xmpp_stanza_t *reply, *query, *name, *version, *text;
	char *ns;
	xmpp_ctx_t *ctx = (xmpp_ctx_t*)userdata;
	bot_debug("Version request from: %s\n", xmpp_stanza_get_attribute(stanza, "from"));
	
	reply = xmpp_stanza_new(ctx);
	xmpp_stanza_set_name(reply, "iq");
	xmpp_stanza_set_type(reply, "result");
	xmpp_stanza_set_id(reply, xmpp_stanza_get_id(stanza));
	xmpp_stanza_set_attribute(reply, "to", xmpp_stanza_get_attribute(stanza, "from"));
	
	query = xmpp_stanza_new(ctx);
	xmpp_stanza_set_name(query, "query");
	ns = xmpp_stanza_get_ns(xmpp_stanza_get_children(stanza));
	if (ns) {
		xmpp_stanza_set_ns(query, ns);
	}

	name = xmpp_stanza_new(ctx);
	xmpp_stanza_set_name(name, "name");
	xmpp_stanza_add_child(query, name);

	/* gtalkbot的信息 */
	text = xmpp_stanza_new(ctx);
	xmpp_stanza_set_text(text, "gtalkbot, a simple bot with plugin support");
	xmpp_stanza_add_child(name, text);

	/* gtalkbot的版本 */
	version = xmpp_stanza_new(ctx);
	xmpp_stanza_set_name(version, "version");
	xmpp_stanza_add_child(query, version);
	
	text = xmpp_stanza_new(ctx);
	xmpp_stanza_set_text(text, BOT_VERSION);
	xmpp_stanza_add_child(version, text);
	
	xmpp_stanza_add_child(reply, query);

	xmpp_send(conn, reply);
	xmpp_stanza_release(reply);
	return 1;
}

static int
bot_msg_handler(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata)
{
	xmpp_ctx_t *ctx;	
	xmpp_stanza_t *reply, *body, *text;
	char *intext, *replytext;
	char *command, *args;
	ctx = (xmpp_ctx_t*)userdata;
	
	/* 收到的消息无内容或接收出错则不处理 */
	if(!xmpp_stanza_get_child_by_name(stanza, "body")) return 1;
	if(!strcmp(xmpp_stanza_get_attribute(stanza, "type"), "error")) return 1;


	/* 记录收到消息 */
	intext = xmpp_stanza_get_text(xmpp_stanza_get_child_by_name(stanza, "body"));
	bot_debug("Incoming from %s: %s ", xmpp_stanza_get_attribute(stanza, "from"), intext);

	/* 设置回复对象 */
	reply = xmpp_stanza_new(ctx);
	xmpp_stanza_set_name(reply, "message");
	xmpp_stanza_set_type(reply, xmpp_stanza_get_type(stanza)?xmpp_stanza_get_type(stanza):"chat");
	xmpp_stanza_set_attribute(reply, "to", xmpp_stanza_get_attribute(stanza, "from"));

	/* 设置回复内容 */
	body = xmpp_stanza_new(ctx);
	xmpp_stanza_set_name(body, "body");
	/* 判断命令是否有插件支持 */
	command = strtok(intext, " \t");
	if (bot_get_plugin(command) < 0) {	/* 不支持该命令，列出所有支持的命令 */
		replytext = strdup("command not support!\n");
	} else {							/* 尝试运行 */
		args = strtok(NULL, ";\n");
		replytext = (char *)bot_run_plugin(command, args);
	}
	
	text = xmpp_stanza_new(ctx);
	xmpp_stanza_set_text(text, replytext);
	xmpp_stanza_add_child(body, text);
	xmpp_stanza_add_child(reply, body);

	/* 回复 */
	xmpp_send(conn, reply);
	xmpp_stanza_release(reply);
	free(replytext);
	return 1;
}

void
bot_conn_handler(xmpp_conn_t * const conn, const xmpp_conn_event_t status, 
		      const int error, xmpp_stream_error_t * const stream_error,
		      void * const userdata)
{
	xmpp_ctx_t *ctx = (xmpp_ctx_t *)userdata;
	
	if (status == XMPP_CONN_CONNECT) {
		xmpp_stanza_t* pres;
		bot_debug("Connected");

		/* version handler */
		xmpp_handler_add(conn,bot_ver_handler, "jabber:iq:version", "iq", NULL, ctx);
		/* message handler */
		xmpp_handler_add(conn,bot_msg_handler, NULL, "message", NULL, ctx);
	
		/* 给联系人发送在线信息 */
		pres = xmpp_stanza_new(ctx);
		xmpp_stanza_set_name(pres, "presence");
		xmpp_send(conn, pres);
		xmpp_stanza_release(pres);
	}
	else {
		bot_debug("Disconnected");
		xmpp_stop(ctx);
	}
}
