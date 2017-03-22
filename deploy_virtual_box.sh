#!/bin/bash

sh clean.sh
rsync -rlpgoDvP -e 'ssh -p 4022' --progress . --exclude .gitignore chef@localhost:/tmp/starry-night
#ssh -p 4022 chef@localhost "cd /tmp/starry-night && scons mode=debug && ./unit_test.sh"
ssh -p 4022 chef@localhost "cd /tmp/starry-night/src/base && make clean && make"
#ssh -p 4022 chef@localhost "cd /tmp/starry-night && scons"
