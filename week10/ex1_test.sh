#!/bin/bash
mkdir folder1
echo "File Number 1\nLorem est sint voluptate aliqua." > folder1/f1.txt
echo "File Number 2\nAliqua enim duis duis magna veniam laborum ad pariatur."> folder1/f2.txt
ls folder1
cat folder1/f1.txt
echo "Hello" >> folder1/f2.txt
touch folder1/f1.txt
rm folder1/f1.txt
chmod u=w folder1
rm -r folder1
