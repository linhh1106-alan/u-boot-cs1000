# $HOME/.login* or similar files may first set $DEBUGINFOD_URLS.
# If $DEBUGINFOD_URLS is not set there, we set it from system *.url files.
# $HOME/.*rc or similar files may then amend $DEBUGINFOD_URLS.
# See also [man debuginfod-client-config] for other environment variables
# such as $DEBUGINFOD_MAXSIZE, $DEBUGINFOD_MAXTIME, $DEBUGINFOD_PROGRESS.

if (! $?DEBUGINFOD_URLS) then
    set prefix="/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/recipe-sysroot-native/usr"
    set DEBUGINFOD_URLS=`sh -c 'cat "$0"/*.urls; :' "/work/build-cs1000/tmp/work/corstone1000_fvp-poky-linux-musl/u-boot/1_2022.07-r0/recipe-sysroot-native/etc/debuginfod" 2>/dev/null | tr '\n' ' '`
    if ( "$DEBUGINFOD_URLS" != "" ) then
        setenv DEBUGINFOD_URLS "$DEBUGINFOD_URLS"
    else
        unset DEBUGINFOD_URLS
    endif
    unset prefix
endif
