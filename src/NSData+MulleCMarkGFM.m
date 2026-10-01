#import "NSData+MulleCMarkGFM.h"

#import "import-private.h"

#include <string.h>


static void
   rewrite_local_links( cmark_node *node, cmark_mem *mem)
{
   cmark_node   *child;
   const char   *url;
   char         *new_url;
   size_t       length;
   size_t       suffix;

   if( cmark_node_get_type( node) == CMARK_NODE_LINK)
   {
      url = cmark_node_get_url( node);
      if( url)
      {
         length = strlen( url);
         suffix = sizeof( ".md") - 1;
         if( length > suffix &&
             strncmp( url + length - suffix, ".md", suffix) == 0 &&
             strstr( url, "://") == NULL)
         {
            new_url = mem->calloc( 1, length + 3);
            if( new_url)
            {
               memcpy( new_url, url, length - suffix);
               memcpy( new_url + length - suffix, ".html", sizeof( ".html"));
               cmark_node_set_url( node, new_url);
               mem->free( new_url);
            }
         }
      }
   }

   for( child = cmark_node_first_child( node); child; child = cmark_node_next( child))
      rewrite_local_links( child, mem);
}


static void
   attach_extension_if_found( cmark_parser *parser, const char *name)
{
   cmark_syntax_extension   *extension;

   extension = cmark_find_syntax_extension( name);
   if( extension)
      cmark_parser_attach_syntax_extension( parser, extension);
}


@implementation NSData( MulleCMarkGFM)

- (NSData *) cmarkGFMedDataWithHTMLFlags:(NSUInteger) flags
                              extensions:(NSUInteger) extensions
                               tocIndent:(NSUInteger) indent
{
   cmark_mem      *mem;
   cmark_node     *document;
   cmark_parser   *parser;
   char           *html;
   int            options;
   NSData         *data;

   (void) indent;

   options = CMARK_OPT_DEFAULT;

   if( ! (flags & MULLE_CMARK_GFM_HTML_SKIP_HTML))
      options |= CMARK_OPT_UNSAFE;
   if( flags & MULLE_CMARK_GFM_HTML_HARD_WRAP)
      options |= CMARK_OPT_HARDBREAKS;
   if( extensions & MULLE_CMARK_GFM_EXT_FOOTNOTES)
      options |= CMARK_OPT_FOOTNOTES;

   cmark_gfm_core_extensions_ensure_registered();

   parser = cmark_parser_new( options);

   if( extensions & MULLE_CMARK_GFM_EXT_TABLES)
      attach_extension_if_found( parser, "table");
   if( extensions & MULLE_CMARK_GFM_EXT_STRIKETHROUGH)
      attach_extension_if_found( parser, "strikethrough");
   if( extensions & MULLE_CMARK_GFM_EXT_AUTOLINK)
      attach_extension_if_found( parser, "autolink");
   if( extensions & MULLE_CMARK_GFM_EXT_TAGFILTER)
      attach_extension_if_found( parser, "tagfilter");
   if( flags & MULLE_CMARK_GFM_HTML_USE_TASK_LIST)
      attach_extension_if_found( parser, "tasklist");

   cmark_parser_feed( parser, (const char *) [self bytes], [self length]);
   document = cmark_parser_finish( parser);

   mem = cmark_get_default_mem_allocator();

   if( flags & MULLE_CMARK_GFM_HTML_LOCAL_LINKS)
      rewrite_local_links( document, mem);

   html = cmark_render_html( document,
                             options,
                             cmark_parser_get_syntax_extensions( parser));

   data = nil;
   if( html)
      data = [[[NSData alloc] initWithBytes:html
                                     length:strlen( html)] autorelease];

   mem->free( html);
   cmark_parser_free( parser);
   cmark_node_free( document);

   return( data);
}


- (NSData *) markdownedDataWithHTMLFlags:(NSUInteger) flags
                              extensions:(NSUInteger) extensions
                               tocIndent:(NSUInteger) indent
{
   return( [self cmarkGFMedDataWithHTMLFlags:flags
                                  extensions:extensions
                                   tocIndent:indent]);
}


- (NSData *) markdownedData
{
   return( [self cmarkGFMedData]);
}


- (NSData *) cmarkGFMedData
{
   return( [self cmarkGFMedDataWithHTMLFlags:MULLE_CMARK_GFM_HTML_USE_TASK_LIST |
                                              MULLE_CMARK_GFM_HTML_LOCAL_LINKS
                                  extensions:MULLE_CMARK_GFM_EXT_GFM
                                   tocIndent:0]);
}


- (NSData *) hoedownedDataWithHTMLFlags:(NSUInteger) flags
                             extensions:(NSUInteger) extensions
                              tocIndent:(NSUInteger) indent
{
   return( [self cmarkGFMedDataWithHTMLFlags:flags
                                  extensions:extensions
                                   tocIndent:indent]);
}


- (NSData *) hoedownedData
{
   return( [self hoedownedDataWithHTMLFlags:HOEDOWN_HTML_USE_TASK_LIST |
                                             MULLE_MARKDOWN_HTML_LOCAL_LINKS
                                 extensions:HOEDOWN_EXT_BLOCK | HOEDOWN_EXT_SPAN |
                                            HOEDOWN_EXT_NO_INTRA_EMPHASIS |
                                            HOEDOWN_EXT_SPACE_HEADERS |
                                            HOEDOWN_EXT_MATH_EXPLICIT
                                  tocIndent:0]);
}

@end
