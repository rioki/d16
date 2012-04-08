
# DCPU-16 Assembler and Simulator

This ia an implementation of an assember and simulator for the DCPU-16 from 
[0x10c]. 

[0x10c]: http://0x10c.com/

A specification of the CPU can be found at: http://0x10c.com/doc/dcpu-16.txt

## Building

To build d16 you need a [C compiler], [make] and [flex].

[C compiler]: http://gcc.gnu.org/
[make]: https://www.gnu.org/software/make/
[flex]: http://flex.sourceforge.net/

## Open Issues

A number un unclear or undefined issues remain. Here is the list of things
I tripped over during development.

### Harvard or von Neumann Architechture

There are two different CPU common architechtures, the basic differences relate
to how the handle data and the program.

The [Harvard architechture] has two memory areas, the instruction memory and the 
data memory. As a result you have the full ammount of RAM for program data, but
on the other hand it is impossible or hard for the CPU to change the running
program.

[Harvard architechture]: https://en.wikipedia.org/wiki/Harvard_architecture

The [von Neumann architecture] uses one block of RAM and thus intermixes 
instructions and data. This has the advantage that operating systems can quite
flexibly change the program data, such as load new ones and remove old ones. 
The downside is that when writing the program you need to know where free memory
is. The commonly used PC architechture IA-32 and IA-64 is a von Neumann 
architechture.

[von Neumann architecture]: https://en.wikipedia.org/wiki/Von_Neumann_architecture

From the example program and the documentation, I deduced that the DCPU-16 is
a von Neumann architechture. But that might be wrong.

### CPU Initialisation

It is unclear what the CPU guarantees for an initial state. Is every thing zero
or undefined. The only obvious things are that PC is zero and SP is 0xFFFF.

### Overflow

What rules are applied when registers oveflow? It is somehow hinted that the 
normal registers will set O. But it seams that O is bit register and now a word
register. How is overflow with MUL handled? Especially how is overflow on PC and 
SP handled? 

I assumed that O is a bit register and the modulo rule applies. This may result
though in data loss when you MUL two large values.

In my current implementation PC wraps around, but is will walk the entiere 
program memory first. 

SP will truncate at the boundaries. It may make sense to trap when SP overflows
thouhg.

### RAM Access out of Range

What happens if RAM is accessed outside of the valid range? Does the CPU trap or
is the value silently wraped?

I implemented a CPU trap for that.

### NOOP

The NOOP instruction is not defined. I simply assumed NOOP to match 0x0, so
that the CPU does not trap when going over empy intructions.

### Traps and Interupts

Is there a way to handle traps and interupts programatically? Currently nothing
of this sort is defined, so I did not program anything.

## Copying

    Copyright (C) 2012 Sean "rioki" Farrell

    Permission is hereby granted, free of charge, to any person obtaining a copy 
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights 
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
    copies of the Software, and to permit persons to whom the Software is 
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in 
    all copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
    SOFTWARE.
