#import "NSString+MulleCMarkGFM.h"

#import "NSData+MulleCMarkGFM.h"


@implementation NSString( MulleCMarkGFM)

- (NSString *) markdownedStringWithHTMLFlags:(NSUInteger) flags
                                  extensions:(NSUInteger) extensions
                                   tocIndent:(NSUInteger) indent
{
   NSData     *data;
   NSString   *s;

   data = [[self dataUsingEncoding:NSUTF8StringEncoding]
             cmarkGFMedDataWithHTMLFlags:flags
                              extensions:extensions
                               tocIndent:indent];
   s    = [[[NSString alloc] initWithData:data
                                 encoding:NSUTF8StringEncoding] autorelease];
   return( s);
}


- (NSString *) markdownedString
{
   NSData     *data;
   NSString   *s;

   data = [[self dataUsingEncoding:NSUTF8StringEncoding] markdownedData];
   s    = [[[NSString alloc] initWithData:data
                                 encoding:NSUTF8StringEncoding] autorelease];
   return( s);
}


- (NSString *) cmarkGFMedStringWithHTMLFlags:(NSUInteger) flags
                                  extensions:(NSUInteger) extensions
                                   tocIndent:(NSUInteger) indent
{
   return( [self markdownedStringWithHTMLFlags:flags
                                    extensions:extensions
                                     tocIndent:indent]);
}


- (NSString *) cmarkGFMedString
{
   NSData     *data;
   NSString   *s;

   data = [[self dataUsingEncoding:NSUTF8StringEncoding] cmarkGFMedData];
   s    = [[[NSString alloc] initWithData:data
                                 encoding:NSUTF8StringEncoding] autorelease];
   return( s);
}


- (NSString *) hoedownedStringWithHTMLFlags:(NSUInteger) flags
                                 extensions:(NSUInteger) extensions
                                  tocIndent:(NSUInteger) indent
{
   return( [self markdownedStringWithHTMLFlags:flags
                                    extensions:extensions
                                     tocIndent:indent]);
}


- (NSString *) hoedownedString
{
   NSData     *data;
   NSString   *s;

   data = [[self dataUsingEncoding:NSUTF8StringEncoding] hoedownedData];
   s    = [[[NSString alloc] initWithData:data
                                 encoding:NSUTF8StringEncoding] autorelease];
   return( s);
}

@end
