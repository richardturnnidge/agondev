/* fseek
   -----

The C library function int fseek(FILE *stream, long int offset, int origin)
moves the file pointer to a specified location.
*/

#include <stdio.h>
#include <agon/mos.h>
#include <errno.h>

int fseek(FILE *stream, long int offset, int origin)
{
    FIL *file_struct;
    long int target;
    int result;

    if (stream == NULL || stream->fhandle >= FH_STDIN)
    {
        errno = EINVAL;
        return -1;
    }

    file_struct = mos_getfil(stream->fhandle);
    if (file_struct == NULL)
    {
        errno = EINVAL;
        return -1;
    }

    switch (origin)
    {
        case SEEK_SET:
            target = offset;
            break;

        case SEEK_CUR:
            target = (long int)file_struct->fptr;

            /* A pushed-back byte has been physically read but not logically consumed. */
            if (stream->has_unget_char && target > 0)
                target--;

            target += offset;
            break;

        case SEEK_END:
            target = (long int)file_struct->obj.objsize + offset;
            break;

        default:
            errno = EINVAL;
            return -1;
    }

    if (target < 0)
    {
        errno = EINVAL;
        return -1;
    }

    result = mos_flseek(stream->fhandle, target);

    /* Only discard logical stream state after a successful reposition. */
    if (result == 0)
    {
        stream->unget_char = 0;
        stream->has_unget_char = 0;
        stream->eof = 0;
    }

    return result;
}
