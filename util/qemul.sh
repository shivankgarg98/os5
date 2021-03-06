#!/bin/bash

# if tmux info | grep $TTY ; then
if [[ -n $TMUX ]]; then
  echo > serial.out
  # tmux split-window -h 'qemu-system-i386 -kernel build/kernel/kernel -initrd "build/tarfs.tar" -curses -monitor telnet:localhost:4444,server -s -S -serial file:serial.out'
  # tmux split-window -h 'qemu-system-i386 -hda image.img -curses -monitor telnet:localhost:4444,server -s -S -serial file:serial.out'
  tmux split-window -dh -t 0 'sleep 1; i586-elf-gdb'
  tmux split-window -dv -t 0 'tail -f serial.out | util/colorize.sh'
  # qemu-system-i386 -hda image.img -vnc :1 -monitor stdio -s -S -serial file:serial.out
  qemu-system-i386 -hda image.img -vnc :5500,reverse -monitor stdio -s -S -serial file:serial.out
  tmux kill-pane -a -t 0

else
  qemu-system-i386 -kernel build/kernel/kernel -initrd "build/tarfs.tar" -display curses -monitor stdio -s -S
fi
