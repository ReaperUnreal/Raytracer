#!/bin/bash
for i in {1..10}
do
   rtssse3old >>ssse3old.txt
done
for i in {1..10}
do
   rtsse4old >>sse4old.txt
done
for i in {1..10}
do
   rtssse3 >>ssse3.txt
done
for i in {1..10}
do
   rtsse4 >>sse4.txt
done
