/* fread
   -----

The C library function size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
reads data from the given stream into the array pointed to by ptr.
*/

#include <stdio.h>
#include <agon/mos.h>

size_t fread(void *ptr, size_t size, size_t count, FILE *__restrict stream)
{
    size_t nbytes;
    size_t len;

    if (size == 0 || count == 0)
        return 0;

    if (stream == NULL || stream == stdout || stream == stderr)
        return 0;

    len = size * count;

    /* Detect size_t multiplication overflow. */
    if (len / size != count)
    {
        stream->err = 1;
        return 0;
    }

    /*
     * Text streams and streams with pending pushback must go through
     * fgetc() so CR/LF conversion and logical stream ordering are honoured.
     */
    if (stream == stdin || stream->text_mode || stream->has_unget_char)
    {
        unsigned char *p = (unsigned char *)ptr;
        int c;

        for (nbytes = 0; nbytes < len; nbytes++)
        {
            c = fgetc(stream);
            if (c == EOF)
                break;

            *p++ = (unsigned char)c;
        }

        return nbytes / size;
    }

    /* Binary regular file: use the efficient MOS block-read operation. */
    nbytes = mos_fread(stream->fhandle, ptr, len);

    if (nbytes < len)
    {
        if (mos_feof(stream->fhandle))
            stream->eof = 1;
        else
            stream->err = 1;
    }

    return nbytes / size;
}
