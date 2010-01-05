/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
#ifndef _LOG_H_
#define _LOG_H_
#include <stdarg.h>

#define ENABLE_LOG	1
#define ENABLE_DEBUG	1

void	bot_debug(char *fmt, ...);
void	bot_warning(char *fmt, ...);

#endif /* _LOG_H_ */
