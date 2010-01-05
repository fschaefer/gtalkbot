/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
#include <stdio.h>
#include "log.h"

void
bot_debug(char *fmt, ...)
{
	va_list ap;
	if (!ENABLE_DEBUG) return;
	fprintf(stdout, "[DEBUG] ");
	va_start(ap, fmt);
	vfprintf(stdout, fmt, ap);
	va_end(ap);
	fprintf(stdout, "\n");
}

void
bot_warning(char *fmt, ...)
{
	va_list ap;
	if (!ENABLE_DEBUG) return;
	fprintf(stderr, "[WARNING] ");
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
}


/* int main(int argc, char *argv[]) */
/* { */
/* 	bot_debug("hello"); */
/* 	return 0; */
/* } */
