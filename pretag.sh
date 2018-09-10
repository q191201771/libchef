#!/bin/bash

tag='v1.3.4'
echo "New tag ${tag}"

files=`ls src/base/*.hpp`
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

#./ut.sh
#git add .
#git commit -m ":bookmark: pretag ${tag}"
#git push
#git tag ${tag}
#git push --tags
