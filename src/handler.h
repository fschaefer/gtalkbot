/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
#ifndef _HANDLER_H_
#define _HANDLER_H_

#include "strophe.h"

/* 连接事件的handler */
void	bot_conn_handler(xmpp_conn_t * const conn, const xmpp_conn_event_t status, 
			 const int error, xmpp_stream_error_t * const stream_error,
			 void * const userdata);

#endif /* _HANDLER_H_ */
