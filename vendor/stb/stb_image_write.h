/* stb_image_write - v1.16 - public domain - http://nothings.org/stb
   writes out PNG/BMP/TGA/JPEG/HDR images to C stdio - Sean Barrett 2010-2021
*/
#ifndef INCLUDE_STB_IMAGE_WRITE_H
#define INCLUDE_STB_IMAGE_WRITE_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_STATIC
#endif

#ifdef STB_IMAGE_WRITE_STATIC
#define STBIDEF static
#else
#define STBIDEF extern
#endif

STBIDEF int stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes);

#ifdef STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

static unsigned int stbi__crc32(unsigned char *buffer, int len)
{
   static unsigned int crc_table[256];
   static int crc_table_computed = 0;
   unsigned int c = 0xffffffff;
   int n, k;

   if (!crc_table_computed) {
      for (n = 0; n < 256; n++) {
         c = (unsigned int) n;
         for (k = 0; k < 8; k++) {
            if (c & 1) c = 0xedb88320L ^ (c >> 1);
            else c = c >> 1;
         }
         crc_table[n] = c;
      }
      crc_table_computed = 1;
   }

   for (n = 0; n < len; n++) {
      c = crc_table[(c ^ buffer[n]) & 0xff] ^ (c >> 8);
   }
   return c ^ 0xffffffff;
}

static void stbi__write_chunk(FILE *f, char const *type, unsigned char *data, int len)
{
   unsigned char header[8];
   unsigned int crc;
   header[0] = (unsigned char)((len >> 24) & 0xff);
   header[1] = (unsigned char)((len >> 16) & 0xff);
   header[2] = (unsigned char)((len >>  8) & 0xff);
   header[3] = (unsigned char)((len >>  0) & 0xff);
   header[4] = (unsigned char)type[0];
   header[5] = (unsigned char)type[1];
   header[6] = (unsigned char)type[2];
   header[7] = (unsigned char)type[3];
   fwrite(header, 1, 8, f);
   if (len) fwrite(data, 1, len, f);

   crc = stbi__crc32((unsigned char*)type, 4);
   if (len) {
      static unsigned int crc_table[256];
      static int crc_table_computed = 0;
      unsigned int c = crc ^ 0xffffffff;
      int n, k;
      if (!crc_table_computed) {
         for (n = 0; n < 256; n++) {
            c = (unsigned int) n;
            for (k = 0; k < 8; k++) {
               if (c & 1) c = 0xedb88320L ^ (c >> 1);
               else c = c >> 1;
            }
            crc_table[n] = c;
         }
         crc_table_computed = 1;
      }
      for (n = 0; n < len; n++) {
         c = crc_table[(c ^ data[n]) & 0xff] ^ (c >> 8);
      }
      crc = c ^ 0xffffffff;
   }
   header[0] = (unsigned char)((crc >> 24) & 0xff);
   header[1] = (unsigned char)((crc >> 16) & 0xff);
   header[2] = (unsigned char)((crc >>  8) & 0xff);
   header[3] = (unsigned char)((crc >>  0) & 0xff);
   fwrite(header, 1, 4, f);
}

static unsigned char *stbi__zlib_compress(unsigned char *data, int data_len, int *out_len, int quality)
{
   (void)quality;
   *out_len = data_len + 6;
   unsigned char *out = (unsigned char*) malloc(*out_len);
   if (!out) return NULL;
   out[0] = 0x78; out[1] = 0x01;
   out[2] = 1;
   out[3] = (unsigned char)(data_len & 0xff);
   out[4] = (unsigned char)((data_len >> 8) & 0xff);
   out[5] = (unsigned char)((~data_len) & 0xff);
   out[6] = (unsigned char)(((~data_len) >> 8) & 0xff);
   memcpy(out + 7, data, data_len);

   unsigned int s1 = 1, s2 = 0;
   for (int i = 0; i < data_len; ++i) {
      s1 = (s1 + data[i]) % 65521;
      s2 = (s2 + s1) % 65521;
   }
   unsigned int adler = (s2 << 16) | s1;
   out[data_len + 7] = (unsigned char)((adler >> 24) & 0xff);
   out[data_len + 8] = (unsigned char)((adler >> 16) & 0xff);
   out[data_len + 9] = (unsigned char)((adler >>  8) & 0xff);
   out[data_len + 10] = (unsigned char)((adler >>  0) & 0xff);
   *out_len = data_len + 11;
   return out;
}

STBIDEF int stbi_write_png(char const *filename, int x, int y, int comp, const void *data, int stride_bytes)
{
   FILE *f;
   if (stride_bytes == 0) stride_bytes = x * comp;
   f = fopen(filename, "wb");
   if (!f) return 0;

   unsigned char sig[8] = { 137,80,78,71,13,10,26,10 };
   fwrite(sig, 1, 8, f);

   unsigned char ihdr[13];
   ihdr[0] = (unsigned char)((x >> 24) & 0xff);
   ihdr[1] = (unsigned char)((x >> 16) & 0xff);
   ihdr[2] = (unsigned char)((x >>  8) & 0xff);
   ihdr[3] = (unsigned char)((x >>  0) & 0xff);
   ihdr[4] = (unsigned char)((y >> 24) & 0xff);
   ihdr[5] = (unsigned char)((y >> 16) & 0xff);
   ihdr[6] = (unsigned char)((y >>  8) & 0xff);
   ihdr[7] = (unsigned char)((y >>  0) & 0xff);
   ihdr[8] = 8;
   ihdr[9] = (unsigned char)(comp == 1 ? 0 : comp == 2 ? 4 : comp == 3 ? 2 : 6);
   ihdr[10] = 0; ihdr[11] = 0; ihdr[12] = 0;
   stbi__write_chunk(f, "IHDR", ihdr, 13);

   int line_len = x * comp + 1;
   unsigned char *filt = (unsigned char*) malloc(line_len * y);
   if (!filt) { fclose(f); return 0; }
   for (int j = 0; j < y; ++j) {
      filt[j * line_len] = 0;
      memcpy(filt + j * line_len + 1, (unsigned char*)data + j * stride_bytes, x * comp);
   }

   int zlen;
   unsigned char *zlib = stbi__zlib_compress(filt, line_len * y, &zlen, 8);
   free(filt);
   if (!zlib) { fclose(f); return 0; }

   stbi__write_chunk(f, "IDAT", zlib, zlen);
   free(zlib);

   stbi__write_chunk(f, "IEND", NULL, 0);
   fclose(f);
   return 1;
}

#endif // STB_IMAGE_WRITE_IMPLEMENTATION

#ifdef __cplusplus
}
#endif

#endif // INCLUDE_STB_IMAGE_WRITE_H
