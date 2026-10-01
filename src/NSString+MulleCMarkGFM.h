#import "import.h"

#import "NSData+MulleCMarkGFM.h"


@interface NSString( MulleCMarkGFM)

/* the backend-neutral API shared with MulleHoedown */
- (NSString *) markdownedString;
- (NSString *) markdownedStringWithHTMLFlags:(NSUInteger) flags
                                  extensions:(NSUInteger) extensions
                                   tocIndent:(NSUInteger) indent;

/* cmark-gfm specific API */
- (NSString *) cmarkGFMedString;
- (NSString *) cmarkGFMedStringWithHTMLFlags:(NSUInteger) flags
                                  extensions:(NSUInteger) extensions
                                   tocIndent:(NSUInteger) indent;

/* MulleHoedown API compatibility */
- (NSString *) hoedownedString;
- (NSString *) hoedownedStringWithHTMLFlags:(NSUInteger) flags
                                 extensions:(NSUInteger) extensions
                                  tocIndent:(NSUInteger) indent;

@end
