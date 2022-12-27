#! /bin/sh

# Create whatis database.

cd $1
shift

find $* -type f -exec cat {} \; | sed -n '
    /^\.TH/ {
      w /dev/tty
      p
    }
    /^\.SH NAME/ {
      N
      p
    }
  ' | /usr/man/manroff | sed -n '
    s;.;;g
    s;_;;g
    /(\([a-zA-Z0-9]*\).*UNIX Programmer/ {
      s;.*(\([a-zA-Z0-9]*\).*UNIX.*;\1;
      h
      d
    }
    /^NAME/!d
    :name
      s;.*;;
      N
      s;\n;;
      /^[^	 ]/b print
      H
      b name
    :print
      x
      s;\n;;g
      /-/!d
      s;\([a-z][A-z]\)-[	 ][	 ]*;\1;
      s;\([a-zA-Z0-9,]\)[	 ][	 ]*;\1 ;g
      s;[^a-zA-Z0-9]*\([a-zA-Z0-9]*\)[^a-zA-Z0-9]*\(.*\) - \(.*\);\2 (\1) - \3;
      p
      d
  ' | sort -u
