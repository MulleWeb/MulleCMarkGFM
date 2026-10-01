#import "import.h"
#import "NSData+MulleCMarkGFM.h"
#import "NSString+MulleCMarkGFM.h"

#include <stdint.h>

/*
 *  (c) <|YEAR|> nat <|ORGANIZATION|>
 *
 *  version:  major, minor, patch
 */
#define MULLE_C_MARK_GFM_VERSION  ((0UL << 20) | (0 << 8) | 1)


static inline unsigned int   MulleCMarkGFM_get_version_major( void)
{
   return( MULLE_C_MARK_GFM_VERSION >> 20);
}


static inline unsigned int   MulleCMarkGFM_get_version_minor( void)
{
   return( (MULLE_C_MARK_GFM_VERSION >> 8) & 0xFFF);
}


static inline unsigned int   MulleCMarkGFM_get_version_patch( void)
{
   return( MULLE_C_MARK_GFM_VERSION & 0xFF);
}


MULLE_C_MARK_GFM_GLOBAL
uint32_t   MulleCMarkGFM_get_version( void);

/*
   Add your library headers here for exposure to library
   consumers.
*/
// #import "MyClass.h"
