!#/bin/sh

echo "Push Name: "
read name
make clean
git add .
git commit -m "$name"
git push -u origin master
