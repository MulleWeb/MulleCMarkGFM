# MulleCMarkGFM Library Documentation for AI
<!-- Keywords: markdown, html, gfm, cmark, objective-c, nsdata, nsstring -->

## 1. Introduction & Purpose

MulleCMarkGFM converts [markdown](https://en.wikipedia.org/wiki/Markdown) to
HTML using [cmark-gfm](https://github.com/github/cmark-gfm), GitHub's CommonMark
implementation with GitHub Flavored Markdown extensions (tables, strikethrough,
autolinks, task lists, footnotes, tagfilter).

It solves the problem of rendering markdown documents to HTML from mulle-objc
code, without an external command line tool. It is a drop-in replacement for
`MulleHoedown`: it offers the same conversion API and additionally the
`cmarkGFM...` methods.

Key features at a high level:

- Category methods on `NSData` and `NSString` that return rendered HTML.
- A backend-neutral API shared with `MulleHoedown` (`markdowned...`).
- cmark-gfm specific methods (`cmarkGFMed...`).
- MulleHoedown compatibility methods and flag spellings (`hoedowned...`,
  `HOEDOWN_*`).
- A flag set (`MULLE_MARKDOWN_*`, aliased as `MULLE_CMARK_GFM_*`) controlling
  HTML output and document extensions.

It is a leaf rendering library built on top of the C `cmark-gfm` library and
`MulleFoundationBase` (for the Foundation/Objective-C classes it extends).

## 2. Key Concepts & Design Philosophy

- **Backend-neutral conversion API.** The `markdownedData` /
  `markdownedString` methods and the `MULLE_MARKDOWN_*` flags are the API shared
  with `MulleHoedown`, so source can be moved between the two backends. Each
  backend renders with its own native rules, so output is not meant to be
  identical.
- **Flag-driven configuration.** Conversion is configured by two `NSUInteger`
  bitmasks: `flags` (HTML output options) and `extensions` (document syntax
  extensions). All flag spellings use the same numeric values, so any spelling
  can be passed to any conversion method.
- **Three method families.** `markdowned...` is backend-neutral, `cmarkGFMed...`
  is the cmark-gfm native spelling, and `hoedowned...` reproduces MulleHoedown's
  default configuration. `hoedownedData` enables roughly the Hoedown block and
  span extensions; `cmarkGFMedData` enables the GFM extension set.
- **`tocIndent` is a compatibility no-op.** cmark-gfm has no table of contents
  renderer, so the argument is accepted for source compatibility with
  `MulleHoedown` and ignored.
- **Defensive sanitization.** `MULLE_MARKDOWN_HTML_SKIP_HTML` controls whether
  raw HTML in the markdown is skipped (escaped) or allowed. Without it, the
  library maps to `CMARK_OPT_UNSAFE` and raw HTML passes through.
- **Local link rewriting.** `MULLE_MARKDOWN_HTML_LOCAL_LINKS` rewrites local
  markdown links ending in `.md` to `.html`, leaving absolute URLs
  (containing `://`) alone.

## 3. Core API & Data Structures

### 3.1. `MulleCMarkGFM.h`

Umbrella header. Imports `import.h`, `NSData+MulleCMarkGFM.h` and
`NSString+MulleCMarkGFM.h`, and declares the library version.

#### Version information

- `#define MULLE_C_MARK_GFM_VERSION  ((0UL << 20) | (0 << 8) | 1)`
- `static inline unsigned int   MulleCMarkGFM_get_version_major( void)`
- `static inline unsigned int   MulleCMarkGFM_get_version_minor( void)`
- `static inline unsigned int   MulleCMarkGFM_get_version_patch( void)`
- `MULLE_C_MARK_GFM_GLOBAL uint32_t   MulleCMarkGFM_get_version( void);`

`MulleCMarkGFM_get_version` returns the packed version; the three inline
functions extract the major, minor and patch components.

### 3.2. `NSData+MulleCMarkGFM.h`

Category `NSData( MulleCMarkGFM)` that renders markdown bytes to HTML and
returns the result as an `NSData` of UTF-8 HTML.

The header also defines all flag constants. The `MULLE_MARKDOWN_*` definitions
are guarded by `#ifndef MULLE_MARKDOWN_HTML_SKIP_HTML`, the `MULLE_CMARK_GFM_*`
aliases by `#ifndef MULLE_CMARK_GFM_HTML_SKIP_HTML`, and the `HOEDOWN_*`
aliases by `#ifndef HOEDOWN_EXT_BLOCK`, so a translation unit including both
this header and `MulleHoedown`'s cannot get conflicting definitions.

#### HTML output flags (`flags`)

- `MULLE_MARKDOWN_HTML_SKIP_HTML` — `(1UL << 0)` skip/escape raw HTML.
- `MULLE_MARKDOWN_HTML_ESCAPE` — `(1UL << 1)` escape HTML.
- `MULLE_MARKDOWN_HTML_HARD_WRAP` — `(1UL << 2)` render line breaks as `<br>`.
- `MULLE_MARKDOWN_HTML_USE_XHTML` — `(1UL << 3)` emit XHTML.
- `MULLE_MARKDOWN_HTML_USE_TASK_LIST` — `(1UL << 4)` enable task list rendering.
- `MULLE_MARKDOWN_HTML_USE_EMOJI` — `(1UL << 5)` enable emoji.
- `MULLE_MARKDOWN_HTML_LOCAL_LINKS` — `(1UL << 16)` convert local markdown
  (`.md`) links to HTML (`.html`) links.

#### Document extension flags (`extensions`)

- `MULLE_MARKDOWN_EXT_TABLES` — `(1UL << 0)`
- `MULLE_MARKDOWN_EXT_FENCED_CODE` — `(1UL << 1)`
- `MULLE_MARKDOWN_EXT_FOOTNOTES` — `(1UL << 2)`
- `MULLE_MARKDOWN_EXT_AUTOLINK` — `(1UL << 3)`
- `MULLE_MARKDOWN_EXT_STRIKETHROUGH` — `(1UL << 4)`
- `MULLE_MARKDOWN_EXT_UNDERLINE` — `(1UL << 5)`
- `MULLE_MARKDOWN_EXT_HIGHLIGHT` — `(1UL << 6)`
- `MULLE_MARKDOWN_EXT_QUOTE` — `(1UL << 7)`
- `MULLE_MARKDOWN_EXT_SUPERSCRIPT` — `(1UL << 8)`
- `MULLE_MARKDOWN_EXT_MATH` — `(1UL << 9)`
- `MULLE_MARKDOWN_EXT_EMOJI` — `(1UL << 10)`
- `MULLE_MARKDOWN_EXT_NO_INTRA_EMPHASIS` — `(1UL << 11)`
- `MULLE_MARKDOWN_EXT_SPACE_HEADERS` — `(1UL << 12)`
- `MULLE_MARKDOWN_EXT_MATH_EXPLICIT` — `(1UL << 13)`
- `MULLE_MARKDOWN_EXT_DISABLE_INDENTED_CODE` — `(1UL << 14)`
- `MULLE_MARKDOWN_EXT_TAGFILTER` — `(1UL << 15)`

#### Flag groups

- `MULLE_MARKDOWN_EXT_BLOCK` = `TABLES | FENCED_CODE | FOOTNOTES`.
- `MULLE_MARKDOWN_EXT_SPAN` = `AUTOLINK | STRIKETHROUGH | UNDERLINE |
  HIGHLIGHT | QUOTE | SUPERSCRIPT | MATH | EMOJI`.
- `MULLE_MARKDOWN_EXT_GFM` = `TABLES | STRIKETHROUGH | AUTOLINK | FOOTNOTES |
  TAGFILTER` — the extensions understood by cmark-gfm.

#### Alias families

- `MULLE_CMARK_GFM_HTML_*` aliases every `MULLE_MARKDOWN_HTML_*`.
- `MULLE_CMARK_GFM_EXT_*` aliases every `MULLE_MARKDOWN_EXT_*`, plus
  `MULLE_CMARK_GFM_EXT_BLOCK`, `MULLE_CMARK_GFM_EXT_SPAN`,
  `MULLE_CMARK_GFM_EXT_GFM`.
- `HOEDOWN_HTML_*` aliases the HTML flags; `HOEDOWN_EXT_*` aliases the
  extension flags, plus `HOEDOWN_EXT_BLOCK`, `HOEDOWN_EXT_SPAN`,
  `HOEDOWN_EXT_FLAGS`, `HOEDOWN_EXT_NEGATIVE`.

#### Lifecycle / conversion methods

The backend-neutral API shared with MulleHoedown:

```objc
- (NSData *) markdownedData;
- (NSData *) markdownedDataWithHTMLFlags:(NSUInteger) flags
                              extensions:(NSUInteger) extensions
                               tocIndent:(NSUInteger) indent;
```

- `markdownedData` — renders with the cmark-gfm native default
  (`MULLE_CMARK_GFM_EXT_GFM` extensions, task list and local links enabled).
- `markdownedDataWithHTMLFlags:extensions:tocIndent:` — renders with the given
  flags; `tocIndent` is ignored.

cmark-gfm specific API:

```objc
- (NSData *) cmarkGFMedData;
- (NSData *) cmarkGFMedDataWithHTMLFlags:(NSUInteger) flags
                              extensions:(NSUInteger) extensions
                               tocIndent:(NSUInteger) indent;
```

MulleHoedown API compatibility:

```objc
- (NSData *) hoedownedData;
- (NSData *) hoedownedDataWithHTMLFlags:(NSUInteger) flags
                             extensions:(NSUInteger) extensions
                              tocIndent:(NSUInteger) indent;
```

- `hoedownedData` — reproduces MulleHoedown's default (Hoedown block+span
  extensions) so existing call sites keep their behavior.

All `...WithHTMLFlags:extensions:tocIndent:` methods funnel into
`cmarkGFMedDataWithHTMLFlags:extensions:tocIndent:`.

### 3.3. `NSString+MulleCMarkGFM.h`

Category `NSString( MulleCMarkGFM)`. Each method UTF-8 encodes the receiver,
renders the markdown, and returns the HTML as an `NSString`.

```objc
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
```

- `markdownedString` / `cmarkGFMedString` / `hoedownedString` — convenience
  no-argument defaults, equivalent to their `NSData` counterparts.
- `markdownedStringWithHTMLFlags:extensions:tocIndent:` — delegates to the
  `NSData` category using the receiver's UTF-8 data.
- `cmarkGFMedString...` and `hoedownedString...` are aliases of the
  corresponding `markdownedString...` methods.

## 4. Performance Characteristics

- Conversion is a full parse and render pass; cost is O(n) in the size of the
  input markdown, dominated by cmark-gfm's parser.
- `NSString` methods allocate an intermediate UTF-8 `NSData` (plus the HTML
  `NSData`) before creating the result `NSString`; prefer the `NSData` API when
  the rendered HTML is consumed as bytes.
- `MULLE_MARKDOWN_HTML_LOCAL_LINKS` performs an extra O(nodes) traversal of the
  document tree to rewrite `.md` links, but only when the flag is set.
- Not thread-safe in the sense of shared mutable state: there is none. Each
  call creates its own parser and document, so concurrent calls on distinct
  receivers/objects are safe. cmark-gfm core extension registration is done via
  `cmark_gfm_core_extensions_ensure_registered()` on every call.
- Returned objects are autoreleased; no manual memory management is required by
  callers.

## 5. AI Usage Recommendations & Patterns

**Best practices:**

- Import the umbrella header `<MulleCMarkGFM/MulleCMarkGFM.h>`; it pulls in both
  the `NSData` and `NSString` categories.
- Use `markdownedString` / `markdownedData` for the generic case and
  `cmarkGFMed...` when you explicitly want the cmark-gfm naming and defaults.
  Use `hoedowned...` only to preserve `MulleHoedown` behavior.
- Combine flags and extension groups with bitwise OR, e.g.
  `MULLE_MARKDOWN_HTML_USE_TASK_LIST | MULLE_MARKDOWN_EXT_GFM`.
- Do not include both `MulleCMarkGFM` and `MulleHoedown` headers in the same
  translation unit; the compatibility `HOEDOWN_*` spellings are guarded but the
  two backends are not meant to coexist in one TU.
- The result is autoreleased: treat it as a borrowed, autoreleased object and
  do not release it.

**Common pitfalls:**

- `tocIndent` does nothing. Passing a non-zero value is accepted but has no
  effect; there is no table of contents renderer.
- Raw HTML is emitted (unsafe) unless `MULLE_MARKDOWN_HTML_SKIP_HTML` is set.
  If rendering untrusted input, set `MULLE_MARKDOWN_HTML_SKIP_HTML` (or use
  `hoedownedData`, which sets it) to avoid script injection.
- `MULLE_MARKDOWN_EXT_GFM` deliberately covers only the extensions cmark-gfm
  understands; other `MULLE_MARKDOWN_EXT_*` flags are accepted but are not
  attached as cmark-gfm syntax extensions.
- There is no error return type; a failure to allocate HTML yields `nil`.

**Idiomatic usage (mulle-sde / library style):**

```objc
NSString   *markdown;
NSString   *html;

markdown = [NSString stringWithCString:"# Hi\n"
                              encoding:NSUTF8StringEncoding];
html     = [markdown markdownedString];
```

## 6. Integration Examples

### Example 1: Render markdown with the defaults

```objc
#import <MulleCMarkGFM/MulleCMarkGFM.h>

int   main( void)
{
   NSString   *markdown;
   NSString   *html;

   markdown = @"# Headline\n"
              "\n"
              "Some *emphasis* and ~~strikethrough~~.\n"
              "\n"
              "See [guide](guide.md).\n";

   html = [markdown markdownedString];
   mulle_printf( "%@\n", html);
   return( 0);
}
```

### Example 2: Render with explicit flags and extensions

```objc
#import <MulleCMarkGFM/MulleCMarkGFM.h>

NSString *render_tables_only( NSString *markdown)
{
   return( [markdown markdownedStringWithHTMLFlags:MULLE_MARKDOWN_HTML_SKIP_HTML |
                                                   MULLE_MARKDOWN_HTML_USE_TASK_LIST
                                       extensions:MULLE_MARKDOWN_EXT_TABLES |
                                                  MULLE_MARKDOWN_EXT_STRIKETHROUGH
                                        tocIndent:0]);
}
```

### Example 3: Render bytes and keep the result as NSData

```objc
#import <MulleCMarkGFM/MulleCMarkGFM.h>

NSData *render_data( NSData *markdownBytes)
{
   return( [markdownBytes cmarkGFMedDataWithHTMLFlags:MULLE_CMARK_GFM_HTML_USE_TASK_LIST |
                                                       MULLE_CMARK_GFM_HTML_LOCAL_LINKS
                                           extensions:MULLE_CMARK_GFM_EXT_GFM
                                            tocIndent:0]);
}
```

## 7. Dependencies

Direct `mulle-sde` library dependencies:

- `cmark-gfm` — the C markdown parser/renderer that does the actual work.
- `MulleFoundationBase` — provides the Objective-C Foundation classes
  (`NSData`, `NSString`) the categories extend.
- `mulle-objc-tools` — build/reflection tooling (not linked into consumers).

`cmark-gfm-extensions` is referenced with `none` nodetype and is not a real
library dependency.
