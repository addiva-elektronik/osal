/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2017 rt-labs AB, Sweden.
 *
 * This software is licensed under the terms of the BSD 3-clause
 * license. See the file LICENSE distributed with this software for
 * full license information.
 ********************************************************************/

#include "osal_log.h"

#include <stdarg.h>
#ifndef USE_SYSLOG
#include <stdio.h>
#include <time.h>
#else
#include <syslog.h>
#endif

void os_log (uint8_t type, const char * fmt, ...)
{
   va_list list;
#ifndef USE_SYSLOG
   time_t rawtime;
   struct tm timestruct;
   char timestamp[10];

   time (&rawtime);
   localtime_r (&rawtime, &timestruct);
   strftime (timestamp, sizeof (timestamp), "%H:%M:%S", &timestruct);

   switch (LOG_LEVEL_GET (type))
   {
   case LOG_LEVEL_DEBUG:
      printf ("[%s DEBUG] ", timestamp);
      break;
   case LOG_LEVEL_INFO:
      printf ("[%s INFO ] ", timestamp);
      break;
   case LOG_LEVEL_WARNING:
      printf ("[%s WARN ] ", timestamp);
      break;
   case LOG_LEVEL_ERROR:
      printf ("[%s ERROR] ", timestamp);
      break;
   case LOG_LEVEL_FATAL:
      printf ("[%s FATAL] ", timestamp);
      break;
   default:
      break;
   }

   va_start (list, fmt);
   vprintf (fmt, list);
   va_end (list);
   fflush (stdout);
#else
   int prio;

   switch (LOG_LEVEL_GET (type))
   {
   case LOG_LEVEL_DEBUG:
      prio = LOG_DEBUG;
      break;
   case LOG_LEVEL_INFO:
      prio = LOG_INFO;
      break;
   case LOG_LEVEL_WARNING:
      prio = LOG_WARNING;
      break;
   case LOG_LEVEL_ERROR:
      prio = LOG_ERR;
      break;
   case LOG_LEVEL_FATAL:
      prio = LOG_CRIT;;
      break;
   default:
      prio = LOG_NOTICE;
      break;
   }

   va_start (list, fmt);
   vsyslog (prio, fmt, list);
   va_end (list);
#endif
}
