#!/bin/bash

src="/home/bruno/Documents/drupal7/dfsi"
export src
dest="/etc/drupal7/all/modules/contrib/dfsi"
export dest 

rsync --stats -avz --exclude "*.*~"  --exclude "*.sql"   $src/* $dest/

cd $dest
chown root:root *
find . -type d -exec chmod a+rx {} \;
find . -exec chmod a+r {} \;

chown root:root dfsiexport/*
find ./dfsiexport -exec chmod a+r {} \;
