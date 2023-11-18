#!/usr/bin/env bash

fail () {
    set +x
    echo
    echo "*** Fail ***"
    echo "$1"
    echo
    exit 1
}

cwd=$(pwd)
export PATH=cwd/debug:$PATH

DEV=/dev/pmem0
MPT=/mnt/tagfs

CLI="sudo debug/tagfs"

source test_funcs.sh
set -x

verify_mounted $DEV $MPT "test2.sh"

${CLI} fsck $MPT || fail "fsck should not fail when nothing cloned"

N="10"
FILE="bigtest$N"
${CLI} clone $MPT/${FILE} $MPT/${FILE}_clone         || fail "clone $F "
${CLI} clone $MPT/${FILE} $MPT/${FILE}_clone1        || fail "clone $F "

${CLI} fsck $MPT && fail "fsck should fail after cloning "
${CLI} verify -S $N -f $MPT/${FILE}_clone  || fail "${FILE}_clone mismatch"
${CLI} verify -S $N -f $MPT/${FILE}_clone1 || fail "${FILE}_clone1 mismatch"

sudo umount $MPT || fail "umount"
verify_not_mounted $DEV $MPT "test1.sh"
full_mount $DEV $MPT "test1.sh"
verify_mounted $DEV $MPT "test1.sh"

# TODO: make logplay drop files that have allocation collisions
#test -f $MPT/bigtest0_clone && fail "cloned file should disappear after remount"

${CLI} fsck $MPT && fail "fsck if a clone has ever happened should fail"


set +x
echo "*************************************************************************************"
echo "Test_errors completed successfully"
echo "*************************************************************************************"