Raytracer
=========

Porting my raytracer to linux and then messing with it.

Raytracer works now, SSE is re-enabled and made to work better, more efficiently, and have better compile flags. It would be non-trivial to port this makefile to MSVC again.

The Scene has class has been rewritten to use vectors because that's much simpler and more efficient. During this I discovered that AO doesn't actually work, so it's disabled for now pending a complete rewrite.
