#!/usr/bin/zsh
cd $( dirname -- "$0"; )/FlagDir || exit
sed -e '1s/^/[\'$'\n''/' -e '$s/,$/\'$'\n'']/' *.o.json > ../compile_commands.json