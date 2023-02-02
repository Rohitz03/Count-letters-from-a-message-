#!/bin/bash

name=$1
string=$*

name_letters=`echo $name | tr -d [:space:] | wc -m`
total_letters=`echo $string | tr -d [:space:] | wc -m`

myvar=$(expr $total_letters - $name_letters)

echo "The string has $myvar letters." > $name.txt

exit $myvar
