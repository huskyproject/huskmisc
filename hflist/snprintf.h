/*
 * Copyright (c) 1995, 1996, 1997, 1998, 1999 Kungliga Tekniska H�gskolan
 * (Royal Institute of Technology, Stockholm, Sweden).
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by the Kungliga Tekniska
 *      H�gskolan and its contributors.
 *
 * 4. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* $Id$ */

#ifndef __SNPRINTF_H__
#define __SNPRINTF_H__

#ifndef HAS_snprintf
int snprintf (char *str, size_t sz, const char *format, ...)
     __attribute__ ((format (printf, 3, 4)));
#endif

#ifndef HAS_vsnprintf
int vsnprintf (char *str, size_t sz, const char *format, va_list ap)
     __attribute__((format (printf, 3, 0)));
#endif

#ifndef HAS_asprintf
int asprintf (char **ret, const char *format, ...)
     __attribute__ ((format (printf, 2, 3)));
#endif

#ifndef HAS_vasprintf
int vasprintf (char **ret, const char *format, va_list ap)
     __attribute__((format (printf, 2, 0)));
#endif

#ifndef HAS_asnprintf
int asnprintf (char **ret, size_t max_sz, const char *format, ...)
     __attribute__ ((format (printf, 3, 4)));
#endif

#ifndef HAS_vasnprintf
int vasnprintf (char **ret, size_t max_sz, const char *format, va_list ap)
     __attribute__((format (printf, 3, 0)));
#endif

#endif /*  __SNPRINTF_H__ */
