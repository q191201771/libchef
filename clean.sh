#!/bin/bash

scons -c
find . -name '*.gch' | xargs rm
find build/ -name '*.log.chef' | xargs rm
