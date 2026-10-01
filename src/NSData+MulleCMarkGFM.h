#import "import.h"


/*
 * MulleCMarkGFM converts markdown to HTML using cmark-gfm, GitHub's
 * CommonMark + GitHub Flavored Markdown implementation.
 *
 * The MULLE_MARKDOWN_* flags and the markdowned... methods are the API shared
 * with MulleHoedown, so source can be moved between the two. Each backend
 * renders with its own native rules, so the output is not meant to be
 * identical. The MULLE_CMARK_GFM_* and HOEDOWN_* spellings are aliases kept
 * for compatibility. All spellings use the same numeric values, so either can
 * be passed to the conversion methods.
 *
 * The definitions are guarded, so a translation unit that includes both this
 * header and MulleHoedown's can not get conflicting definitions.
 */
#ifndef MULLE_MARKDOWN_HTML_SKIP_HTML
# define MULLE_MARKDOWN_HTML_SKIP_HTML            (1UL << 0)
# define MULLE_MARKDOWN_HTML_ESCAPE               (1UL << 1)
# define MULLE_MARKDOWN_HTML_HARD_WRAP            (1UL << 2)
# define MULLE_MARKDOWN_HTML_USE_XHTML            (1UL << 3)
# define MULLE_MARKDOWN_HTML_USE_TASK_LIST        (1UL << 4)
# define MULLE_MARKDOWN_HTML_USE_EMOJI            (1UL << 5)

/* convert local markdown (.md) links to HTML links (.html) */
# define MULLE_MARKDOWN_HTML_LOCAL_LINKS          (1UL << 16)

/* document extensions */
# define MULLE_MARKDOWN_EXT_TABLES                (1UL << 0)
# define MULLE_MARKDOWN_EXT_FENCED_CODE           (1UL << 1)
# define MULLE_MARKDOWN_EXT_FOOTNOTES             (1UL << 2)
# define MULLE_MARKDOWN_EXT_AUTOLINK              (1UL << 3)
# define MULLE_MARKDOWN_EXT_STRIKETHROUGH         (1UL << 4)
# define MULLE_MARKDOWN_EXT_UNDERLINE             (1UL << 5)
# define MULLE_MARKDOWN_EXT_HIGHLIGHT             (1UL << 6)
# define MULLE_MARKDOWN_EXT_QUOTE                 (1UL << 7)
# define MULLE_MARKDOWN_EXT_SUPERSCRIPT           (1UL << 8)
# define MULLE_MARKDOWN_EXT_MATH                  (1UL << 9)
# define MULLE_MARKDOWN_EXT_EMOJI                 (1UL << 10)
# define MULLE_MARKDOWN_EXT_NO_INTRA_EMPHASIS     (1UL << 11)
# define MULLE_MARKDOWN_EXT_SPACE_HEADERS         (1UL << 12)
# define MULLE_MARKDOWN_EXT_MATH_EXPLICIT         (1UL << 13)
# define MULLE_MARKDOWN_EXT_DISABLE_INDENTED_CODE (1UL << 14)
# define MULLE_MARKDOWN_EXT_TAGFILTER             (1UL << 15)

# define MULLE_MARKDOWN_EXT_BLOCK \
   (MULLE_MARKDOWN_EXT_TABLES | MULLE_MARKDOWN_EXT_FENCED_CODE | \
    MULLE_MARKDOWN_EXT_FOOTNOTES)

# define MULLE_MARKDOWN_EXT_SPAN \
   (MULLE_MARKDOWN_EXT_AUTOLINK | MULLE_MARKDOWN_EXT_STRIKETHROUGH | \
    MULLE_MARKDOWN_EXT_UNDERLINE | MULLE_MARKDOWN_EXT_HIGHLIGHT | \
    MULLE_MARKDOWN_EXT_QUOTE | MULLE_MARKDOWN_EXT_SUPERSCRIPT | \
    MULLE_MARKDOWN_EXT_MATH | MULLE_MARKDOWN_EXT_EMOJI)

/* the GFM extensions understood by cmark-gfm */
# define MULLE_MARKDOWN_EXT_GFM \
   (MULLE_MARKDOWN_EXT_TABLES | MULLE_MARKDOWN_EXT_STRIKETHROUGH | \
    MULLE_MARKDOWN_EXT_AUTOLINK | MULLE_MARKDOWN_EXT_FOOTNOTES | \
    MULLE_MARKDOWN_EXT_TAGFILTER)
#endif


/* cmark-gfm specific spellings */
#ifndef MULLE_CMARK_GFM_HTML_SKIP_HTML
# define MULLE_CMARK_GFM_HTML_SKIP_HTML            MULLE_MARKDOWN_HTML_SKIP_HTML
# define MULLE_CMARK_GFM_HTML_ESCAPE               MULLE_MARKDOWN_HTML_ESCAPE
# define MULLE_CMARK_GFM_HTML_HARD_WRAP            MULLE_MARKDOWN_HTML_HARD_WRAP
# define MULLE_CMARK_GFM_HTML_USE_XHTML            MULLE_MARKDOWN_HTML_USE_XHTML
# define MULLE_CMARK_GFM_HTML_USE_TASK_LIST        MULLE_MARKDOWN_HTML_USE_TASK_LIST
# define MULLE_CMARK_GFM_HTML_USE_EMOJI            MULLE_MARKDOWN_HTML_USE_EMOJI
# define MULLE_CMARK_GFM_HTML_LOCAL_LINKS          MULLE_MARKDOWN_HTML_LOCAL_LINKS

# define MULLE_CMARK_GFM_EXT_TABLES                MULLE_MARKDOWN_EXT_TABLES
# define MULLE_CMARK_GFM_EXT_FENCED_CODE           MULLE_MARKDOWN_EXT_FENCED_CODE
# define MULLE_CMARK_GFM_EXT_FOOTNOTES             MULLE_MARKDOWN_EXT_FOOTNOTES
# define MULLE_CMARK_GFM_EXT_AUTOLINK              MULLE_MARKDOWN_EXT_AUTOLINK
# define MULLE_CMARK_GFM_EXT_STRIKETHROUGH         MULLE_MARKDOWN_EXT_STRIKETHROUGH
# define MULLE_CMARK_GFM_EXT_UNDERLINE             MULLE_MARKDOWN_EXT_UNDERLINE
# define MULLE_CMARK_GFM_EXT_HIGHLIGHT             MULLE_MARKDOWN_EXT_HIGHLIGHT
# define MULLE_CMARK_GFM_EXT_QUOTE                 MULLE_MARKDOWN_EXT_QUOTE
# define MULLE_CMARK_GFM_EXT_SUPERSCRIPT           MULLE_MARKDOWN_EXT_SUPERSCRIPT
# define MULLE_CMARK_GFM_EXT_MATH                  MULLE_MARKDOWN_EXT_MATH
# define MULLE_CMARK_GFM_EXT_EMOJI                 MULLE_MARKDOWN_EXT_EMOJI
# define MULLE_CMARK_GFM_EXT_NO_INTRA_EMPHASIS     MULLE_MARKDOWN_EXT_NO_INTRA_EMPHASIS
# define MULLE_CMARK_GFM_EXT_SPACE_HEADERS         MULLE_MARKDOWN_EXT_SPACE_HEADERS
# define MULLE_CMARK_GFM_EXT_MATH_EXPLICIT         MULLE_MARKDOWN_EXT_MATH_EXPLICIT
# define MULLE_CMARK_GFM_EXT_DISABLE_INDENTED_CODE MULLE_MARKDOWN_EXT_DISABLE_INDENTED_CODE
# define MULLE_CMARK_GFM_EXT_TAGFILTER             MULLE_MARKDOWN_EXT_TAGFILTER

# define MULLE_CMARK_GFM_EXT_BLOCK                 MULLE_MARKDOWN_EXT_BLOCK
# define MULLE_CMARK_GFM_EXT_SPAN                  MULLE_MARKDOWN_EXT_SPAN
# define MULLE_CMARK_GFM_EXT_GFM                   MULLE_MARKDOWN_EXT_GFM
#endif


/*
 * MulleHoedown compatibility spellings. Do not mix the MulleHoedown and
 * MulleCMarkGFM headers in the same translation unit.
 */
#ifndef HOEDOWN_EXT_BLOCK
# define HOEDOWN_HTML_SKIP_HTML        MULLE_MARKDOWN_HTML_SKIP_HTML
# define HOEDOWN_HTML_ESCAPE           MULLE_MARKDOWN_HTML_ESCAPE
# define HOEDOWN_HTML_HARD_WRAP        MULLE_MARKDOWN_HTML_HARD_WRAP
# define HOEDOWN_HTML_USE_XHTML        MULLE_MARKDOWN_HTML_USE_XHTML
# define HOEDOWN_HTML_USE_TASK_LIST    MULLE_MARKDOWN_HTML_USE_TASK_LIST
# define HOEDOWN_HTML_USE_EMOJI        MULLE_MARKDOWN_HTML_USE_EMOJI

# define HOEDOWN_EXT_TABLES            MULLE_MARKDOWN_EXT_TABLES
# define HOEDOWN_EXT_FENCED_CODE       MULLE_MARKDOWN_EXT_FENCED_CODE
# define HOEDOWN_EXT_FOOTNOTES         MULLE_MARKDOWN_EXT_FOOTNOTES
# define HOEDOWN_EXT_AUTOLINK          MULLE_MARKDOWN_EXT_AUTOLINK
# define HOEDOWN_EXT_STRIKETHROUGH     MULLE_MARKDOWN_EXT_STRIKETHROUGH
# define HOEDOWN_EXT_UNDERLINE         MULLE_MARKDOWN_EXT_UNDERLINE
# define HOEDOWN_EXT_HIGHLIGHT         MULLE_MARKDOWN_EXT_HIGHLIGHT
# define HOEDOWN_EXT_QUOTE             MULLE_MARKDOWN_EXT_QUOTE
# define HOEDOWN_EXT_SUPERSCRIPT       MULLE_MARKDOWN_EXT_SUPERSCRIPT
# define HOEDOWN_EXT_MATH              MULLE_MARKDOWN_EXT_MATH
# define HOEDOWN_EXT_EMOJI             MULLE_MARKDOWN_EXT_EMOJI
# define HOEDOWN_EXT_NO_INTRA_EMPHASIS MULLE_MARKDOWN_EXT_NO_INTRA_EMPHASIS
# define HOEDOWN_EXT_SPACE_HEADERS     MULLE_MARKDOWN_EXT_SPACE_HEADERS
# define HOEDOWN_EXT_MATH_EXPLICIT     MULLE_MARKDOWN_EXT_MATH_EXPLICIT
# define HOEDOWN_EXT_DISABLE_INDENTED_CODE \
                                      MULLE_MARKDOWN_EXT_DISABLE_INDENTED_CODE

# define HOEDOWN_EXT_BLOCK             MULLE_MARKDOWN_EXT_BLOCK
# define HOEDOWN_EXT_SPAN              MULLE_MARKDOWN_EXT_SPAN
# define HOEDOWN_EXT_FLAGS \
   (MULLE_MARKDOWN_EXT_NO_INTRA_EMPHASIS | MULLE_MARKDOWN_EXT_SPACE_HEADERS | \
    MULLE_MARKDOWN_EXT_MATH_EXPLICIT)
# define HOEDOWN_EXT_NEGATIVE          MULLE_MARKDOWN_EXT_DISABLE_INDENTED_CODE
#endif


@interface NSData( MulleCMarkGFM)

/* the backend-neutral API shared with MulleHoedown */
- (NSData *) markdownedData;

/*
 * tocIndent is accepted for source compatibility with MulleHoedown and
 * ignored: cmark-gfm has no table of contents renderer.
 */
- (NSData *) markdownedDataWithHTMLFlags:(NSUInteger) flags
                              extensions:(NSUInteger) extensions
                               tocIndent:(NSUInteger) indent;

/* cmark-gfm specific API */
- (NSData *) cmarkGFMedData;
- (NSData *) cmarkGFMedDataWithHTMLFlags:(NSUInteger) flags
                              extensions:(NSUInteger) extensions
                               tocIndent:(NSUInteger) indent;

/* MulleHoedown API compatibility */
- (NSData *) hoedownedData;
- (NSData *) hoedownedDataWithHTMLFlags:(NSUInteger) flags
                             extensions:(NSUInteger) extensions
                              tocIndent:(NSUInteger) indent;

@end
