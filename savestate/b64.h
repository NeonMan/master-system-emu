/**
 *  @file b64.h
 *  @brief header for b64.c
 *
 *  Uses same license as B64.c
 */
#ifndef __B64_H
#define __B64_H

#ifdef __cplusplus
extern "C" {
#endif

void b64_decodeblock(unsigned char *in, unsigned char *out);

void b64_encodeblock(unsigned char *in, unsigned char *out, int len);

#ifdef __cplusplus
}
#endif

#endif