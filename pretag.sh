#!/bin/bash

set -x

# modme
tag='v1.9.17'
echo "New tag ${tag}"

files=$(ls include/chef_base/*.hpp)
for file in $files
do
  echo $file

  grep -n '* @tag      v' $file | grep '3:'
  if [ $? -ne 0 ];then
    echo "Tag not found. append new tag ${tag}..."
    gsed -i "3i\ * @tag      ${tag}" $file
  else
    echo "Tag already exist. modify to ${tag}..."
    gsed -i "3c\ * @tag      ${tag}" $file
  fi
done

# TODO nothing push but retag.
./ut.sh && \
git add . && \
git commit -m ":bookmark: pretag ${tag}" && \
git push && \
git tag ${tag} && \
git push --tags
