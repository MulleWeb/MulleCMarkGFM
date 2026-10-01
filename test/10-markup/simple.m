#import <MulleCMarkGFM/MulleCMarkGFM.h>


int   main( void)
{
   NSString   *markdown;

   markdown = @"# Headline\n"
               "\n"
               "Some *emphasis* and ~~strikethrough~~.\n"
               "\n"
               "| a | b |\n"
               "|---|---|\n"
               "| 1 | 2 |\n"
               "\n"
               "- [x] done\n"
               "- [ ] open\n"
               "\n"
               "See [guide](guide.md).\n"
               "\n"
               "<script>alert('xss')</script>\n";

   mulle_printf( "%@\n", [markdown markdownedString]);
   mulle_printf( "%@\n", [markdown cmarkGFMedString]);
   mulle_printf( "%@\n", [markdown hoedownedString]);
   return( 0);
}
