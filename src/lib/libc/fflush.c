/* fflush
   ------

There is currently no stdio buffering, so output streams require no work.
For an input stream, discard the single pushed-back character.
*/

#include <stdio.h>

int fflush(FILE *stream)
{
    /* fflush(NULL) means all output streams; none are buffered here. */
    if (stream == NULL)
        return 0;

    stream->unget_char = 0;
    stream->has_unget_char = 0;
    return 0;
}
