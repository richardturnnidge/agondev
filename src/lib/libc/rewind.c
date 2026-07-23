/* rewind
   ------

The C library function void rewind(FILE *stream) sets the file position to the
beginning of the stream and clears its error indicator.
*/

#include <stdio.h>

void rewind(FILE *stream)
{
    if (stream == NULL)
        return;

    (void)fseek(stream, 0L, SEEK_SET);
    stream->err = 0;
}
