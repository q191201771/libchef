#!/bin/bash

set -x

# modme
tag='v1.7.16'
echo "New tag ${tag}"

files=$(ls include/chef_base/*.hpp)
for file in $files
do
  echo $file

  grep -n '* @tag      v' $file | grep '2:'
  if [ $? -ne 0 ];then
    echo "Tag not found. append new tag ${tag}..."
    gsed -i "2i\ * @tag      ${tag}" $file
  else
    echo "Tag already exist. modify to ${tag}..."
    gsed -i "2c\ * @tag      ${tag}" $file
  fi
done

# TODO nothing push but retag.
./ut.sh && \
git add . && \
git commit -m ":bookmark: pretag ${tag}" && \
git push && \
git tag ${tag} && \
git push --tags
