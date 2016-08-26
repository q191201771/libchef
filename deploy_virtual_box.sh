#!/bin/bash

sh clean.sh
tar zcvf ../starry-night.tgz ../starry-night
scp -P 4022 ../starry-night.tgz chef@localhost:~/
ssh -p 4022 chef@localhost "cd /home/chef && tar zxvf starry-night.tgz && cd /home/chef/starry-night && scons && ./unit_test.sh"
