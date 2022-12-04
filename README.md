# lstatus

_lstatus_ is (supposed to be) a simple helper to tool to fetch certain trivial info on your Linux* system.

Following loosely the [Suckless](https://suckless.org) philosophy, _lstatus_ is configured by changing the values inside the `config.h` header and recompiling it.

(*) Yes, only Linux is supported so far, mostly due to my familiarity and _lstatus_'s heavy reliance on the system's directory structure.

## Why?
Why not?

Seriouly though, I had the idea to build this when, while tinkering with `dwmstatus`, I realized how much work goes into solutions like it and thought to myself, _"I can do that!"_.

## Usage

Currently, `lstatus` can be invoked in shell or in a script.

### Example: Using it to show data in the dwm bar

```bash
#!/bin/bash

while [ true ]
do
  xsetroot -name "$(lstatus)"
  sleep 1
done

# If you actually use this, make sure to
# invoke this script with '/path/to/script.sh &'
# to run it in a new thread

```

Result when executing the script:

![result1](assets/result1.png)
