# cmark-gfm-craftinfo

Craftinfo for mulle-sde to build [cmark-gfm](//github.com/<|GITHUB_USER|>/cmark-gfm)

``` bash
mulle-sde dependency add --github <|GITHUB_USER|> cmark-gfm
mulle-sde dependency mark cmark-gfm singlephase
# mulle-sde dependency set cmark-gfm aliases cmark-gfm-other
# mulle-sde dependency set cmark-gfm include cmark-gfm-other.h
# mulle-sde environment --global set MULLE_CRAFT_USE_SCRIPTS cmark-gfm-build
```

## Tips

* If you want to define `CFLAGS` use `definition/set/append/CFLAGS` instead of `definition/set/CFLAGS` so that you can still add flags with the environment variable `CFLAGS`. Or use `mulle-sde dependency craftinfo  set --append cmark-gfm CFLAGS "-DX=0`
* use `CPPFLAGS` instead of `CFLAGS` to match C++, C and Objective-C
* you can use `definition/set/append0` to append a string without an intervening space
* you can use `definition/set/remove` to remove a definition
