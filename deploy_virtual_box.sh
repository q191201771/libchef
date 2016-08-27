#!/bin/bash

sh clean.sh
rsync -a -e 'ssh -p 4022' --progress . --exclude .git localhost:/tmp/starry-night
ssh -p 4022 chef@localhost "cd /tmp/starry-night && scons && ./unit_test.sh"

