# findex
Attach metadata to files, and retrieve it.

> Usage
```
findex <filename to search for> <tag to display>
```
You may want to run `findex_gen`  to make a database:
```
findex_gen <optional index filename>
```

> Documentation
First, change to the `src` directory, then run `make`.
Upon running `make install`, some manual pages will be provided for
the core findex programs. The key-value file format is fully documented
therein.

> Other OSes
findex supports other operating systems and C compilers as well. Support for
other C compilers is given by various Makefiles in the `src` directory. 
