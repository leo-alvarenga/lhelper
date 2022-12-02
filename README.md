# lhelper

_lhelper_ is (supposed to be) a simple helper to tool to fetch certain trivial info on your Linux* system.

(*) Yes, only Linux is supported so far, mostly due to my familiarity and _lhelper_'s heavy reliance on the system's directory structure.

## Why?
Why not?

Seriouly though, I had the idea to build this when, while tinkering with `dwmstatus`, I realized how much work one can have every time they want a new 'module' (or whatever you call it for that matter) to their `dwm` bar, for example.

Following loosely the [Suckless](https://suckless.org) philosophy, _lhelper_ is configured by changing the values inside the `config.h` header and recompiling it.