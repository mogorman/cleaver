#!/bin/bash
#this is where continuous integration script will live
Project="cleaver"

copy_files() {
    local new_copy="$1"
    local old_copy="$2"
    local versioned=`git rev-parse --short HEAD`"_$1"

    if diff "$new_copy" "$old_copy" 2>/dev/null ; then
	echo Same file nothing to do
    else
	echo File changed make copies.
	cp "$1" "$2"
	cp "$1" "/home/gitlab_ci_runner/artifacts/${Project}/build/"`git rev-parse --short HEAD`_"$1"
    fi
    if [ ! -f "$2" ]; then
	echo File changed make copies.
	cp "$1" "$2"
	cp "$1" "/home/gitlab_ci_runner/artifacts/${Project}/build/"`git rev-parse --short HEAD`_"$1"
    fi
}

git submodule update --init
ls -la
cd hardware
make clean
make CUSTOMPCB="/usr/src/pcb/src/pcb" \
     CUSTOMPCB_ARGS="--photo-mask-colour red \
     --photo-silk-colour white --photo-plating  tinned"
make CUSTOMPCB="/usr/src/pcb/src/pcb" \
     CUSTOMPCB_ARGS="--photo-mask-colour red \
     --photo-silk-colour white  --photo-plating  tinned" photos
ls black_board.png board.png bom order xy schematic.png schematic.pdf gerbers/${Project}_make.bottom.gbr > /dev/null 2>&1
if [ "$?" != "0" ]; then
    echo "I failed to create all the files I should have. build failed"
    exit 1
fi
New_project=`git diff HEAD^ HEAD -- ${Project}.pcb|wc -l`
New_schematic=`git diff HEAD^ HEAD -- ${Project}.sch|wc -l`

if [ "$New_project" == "0" ]; then
    echo "No changes to ${Project} don't bother saving image."
else
    copy_files board.png ~/artifacts/${Project}/board.png
    File=`ls *.zip|grep -v oshpark`
    cp "${File}" gerbers.zip
    copy_files gerbers.zip ~/artifacts/${Project}/gerbers.zip
fi

if [ "$New_schematic" == "0" ]; then
    echo "No changes to schematic don't bother saving the image."
else
    copy_files schematic.png ~/artifacts/${Project}/schematic.png
fi
make clean
cd ../firmware
make
make clean
