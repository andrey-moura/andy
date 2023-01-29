# uva
Set of all uva's libraries + some facilities to include and work with them

# Debug Level

The debug level is set 0 in Realease and 1 in Debug. The macro `__UVA_DEBUG_LEVEL_DEFAULT__` contains the default value.
The actual value to be set is a global constexpr called `uva_debug_level` defined in ~~uva.hpp~~ core.hpp. If you're contributting to `UVA`, you can add to preprocessor
`__UVA_OVERRIDE_DEBUG_LEVEL__` with the value set to one of the levels defined above.

### Level summary
> **_NOTE:_**  The Level are an hierarchy. Level 1 contains level 0, level 2 contains level 1 & 0, and so on.

| LEVEL | Description                                    | On Failure
|-------|------------------------------------------------|-----------------------------------|
| 0     | Release, no extra checks                       | -
| 1     | Debug, integral types checks (char, int, bool) | log_warning

### Checks summary

| CHECK LEVEL| CHECK                          | Description                                                                                 |
|------------|--------------------------------|---------------------------------------------------------------------------------------------|
| 1          | UVA_CHECK_RESERVED_BUFFER      | Buffer reserved are checked if the reserved len was enough to fit it's content              |
