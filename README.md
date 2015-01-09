# cppeleven

This is de facto an application skeleton in progress.

It integrates and showcases (to ... erm ... at least some extent) some tools to conveniently make a C++-11 based application with Test-Driven-Development (TDD/BDD).

- CMake
- Valgrind
- GCov
- C++ 11 inkluding semi automatic compiler selection
- Doxygen
- Google(tm) Mock Framework
- Cucumber (preferred)
- shunit2 (fallback, rather brittle)
- Vim sporting a bunch of helping modules suggested, like
- - c-support (the very original)
- - Vim-Projectionist
- - [my https://github.com/meichholz/devsupport.git](devsupport) wiring
- - [my dotvim wiring](https://github.com/meichholz/dotvim.git)
- Rake master control by [my devsupport](https://github.com/meichholz/devsupport.git).

Whoa, that's a lot of tools, aren't they?

# Building

Then You `git submodule init` and `git submodule update` to check the stuff out.

Get the google test frameworks manually (into `googletest`) or by *Rake*:

    rake pull:gtest

You may symlink instead, or whatever, since we need this tree building the test
versions of the program read only.

Of course You have to install any of the tools, tune Your Vim environment to get the
projectionist for ease of use, and stuff like that.

After all You might be able to run a full cycle by simply

    rake edit
    rake check
    rake test:grind
    rake doc:all

Note, that You can do the `rake check` part with some C support in Vim by
simply pressing *F11* or even better: *F10* with quicklist access to the
defecting lines.

The test suites do some filtering and formatting to allow for just that: To navigate from an error reported by the Non-C-Frontend into the defective Source area by just hitting *Enter*.

And that is the BDD/TDD-Cycle: **Make it red! Make it green! Refactor!**

# License

(MIT License)

Copyright (c) 2013

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


