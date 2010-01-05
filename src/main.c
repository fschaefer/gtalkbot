/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strophe.h"
#include "handler.h"
#include "settings.h"


int main(int argc, char *argv[])
{
	xmpp_ctx_t *ctx;
	xmpp_conn_t *conn;
	xmpp_log_t *log;
	const char *username;
	const char *password;

	username = bot_get_username();
	password = bot_get_password();

	xmpp_initialize();

	/* context */
	log = xmpp_get_default_logger(XMPP_LEVEL_INFO);
	ctx = xmpp_ctx_new(NULL, log);

	/* 创建连接 */
	conn = xmpp_conn_new(ctx);

	/* 设置认证信息 */
	xmpp_conn_set_jid(conn, username);
	xmpp_conn_set_pass(conn, password);

	/* 初始化连接 */
	xmpp_connect_client(conn, NULL, 0, bot_conn_handler, ctx);

	/* 进入主循环 */
	xmpp_run(ctx);

	/* 断开连接和释放资源 */
	xmpp_conn_release(conn);
	xmpp_ctx_free(ctx);

	xmpp_shutdown();
	
	return 0;
}
