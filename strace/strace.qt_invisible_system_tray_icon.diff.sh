#!/usr/bin/env bash

a=strace.qt_invisible_system_tray_icon.bad.txt
b=strace.qt_invisible_system_tray_icon.good.txt

s=""
s+="s/0x[0-9a-f]+/0x0000/g;"
s+="s/[0-9]+/0000/g;"

grep_args=(
  -v
  -e 'ENOENT (No such file or directory)$'
  -e '^getrandom("'
  -e '^brk('
  -e '^futex('
  -e '^mmap('
  -e '^munmap('
  -e '^ioctl('
  -e '^rt_sigprocmask('
  -e '^close('
  -e '^fcntl('
  -e '^ftruncate('
  -e '^fcntl('
  -e '^memfd_create('
)

diff -u \
  <(cat "$a" | sed -E "$s" | grep "${grep_args[@]}") --label bad \
  <(cat "$b" | sed -E "$s" | grep "${grep_args[@]}") --label good
