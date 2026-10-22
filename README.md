# php-bailout-bug

`concept.c` shows how a `longjmp` that jumps out of a half-finished
initialization leaves a field unset, so a later read picks up whatever bytes
were already in that reused memory.

## Run

    cc concept.c -o concept && ./concept
