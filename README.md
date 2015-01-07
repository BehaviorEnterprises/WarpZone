
<span style="color: red;">NOTE: WarpZone is currently incomplete.  Please
standby for updates.  This has been posted to github to share work between
development systems.</span>

WarpZone
========
Dynamic Time Warping, without a  DeLorean


Description
-----------

WarpZone implements a dynamic domain warping algorithm to compare arbitrary
series data.  It is primarily tested as a companion to FEX for which it is used
to compare audio samples for similarity of structure.

Usage
-----

WarpZone accepts two or more filenames and performes all pair-wise comparisons
on the set of files.  Each file must be raw binary data comprised of floating
point values (C type 'double' in the appropriate endianness for the machine on
which WarpZone is running).

FEX will soon have the ability to export suitable files for analysis with
WarpZone.

Warpzone will produce a csv spreadsheet of the final distance values for each
pair-wise comparisons as well as two png files for each pair-wise comparison:
one of the cost matrix, another of the cummulative cost matrix.  All newly
created files will be saved in the current working directory.

```
$ WarpZone file1 file2 ...
```

Packaging
---------

WarpZone includes a Makefile that accepts PREFIX, DESTDIR, and other standard
compiler/linker flags

```bash
git clone ...
cd warpzone
make
sudo make install
```

Known Bugs & Todo List
----------------------

### Bugs

None - bugs all currently reside in the 'unknown' category.  Let me know when
you find them.

### Todo

1. Create spreadsheet output
1. Add peak-frequency data export function to FEX
1. Write man page

