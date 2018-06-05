# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.WavyPlane.Debug:
/Users/joe/github/cinder-projects/example/WavyPlane/Debug/WavyPlane.app/Contents/MacOS/WavyPlane:\
	/Users/joe/cinder_master/lib/macosx/Debug/libcinder.a\
	/Users/joe/cinder_master/lib/macosx/libboost_system.a\
	/Users/joe/cinder_master/lib/macosx/libboost_filesystem.a
	/bin/rm -f /Users/joe/github/cinder-projects/example/WavyPlane/Debug/WavyPlane.app/Contents/MacOS/WavyPlane


PostBuild.WavyPlane.Release:
/Users/joe/github/cinder-projects/example/WavyPlane/Release/WavyPlane.app/Contents/MacOS/WavyPlane:\
	/Users/joe/cinder_master/lib/macosx/Debug/libcinder.a\
	/Users/joe/cinder_master/lib/macosx/libboost_system.a\
	/Users/joe/cinder_master/lib/macosx/libboost_filesystem.a
	/bin/rm -f /Users/joe/github/cinder-projects/example/WavyPlane/Release/WavyPlane.app/Contents/MacOS/WavyPlane


PostBuild.WavyPlane.MinSizeRel:
/Users/joe/github/cinder-projects/example/WavyPlane/MinSizeRel/WavyPlane.app/Contents/MacOS/WavyPlane:\
	/Users/joe/cinder_master/lib/macosx/Debug/libcinder.a\
	/Users/joe/cinder_master/lib/macosx/libboost_system.a\
	/Users/joe/cinder_master/lib/macosx/libboost_filesystem.a
	/bin/rm -f /Users/joe/github/cinder-projects/example/WavyPlane/MinSizeRel/WavyPlane.app/Contents/MacOS/WavyPlane


PostBuild.WavyPlane.RelWithDebInfo:
/Users/joe/github/cinder-projects/example/WavyPlane/RelWithDebInfo/WavyPlane.app/Contents/MacOS/WavyPlane:\
	/Users/joe/cinder_master/lib/macosx/Debug/libcinder.a\
	/Users/joe/cinder_master/lib/macosx/libboost_system.a\
	/Users/joe/cinder_master/lib/macosx/libboost_filesystem.a
	/bin/rm -f /Users/joe/github/cinder-projects/example/WavyPlane/RelWithDebInfo/WavyPlane.app/Contents/MacOS/WavyPlane




# For each target create a dummy ruleso the target does not have to exist
/Users/joe/cinder_master/lib/macosx/Debug/libcinder.a:
/Users/joe/cinder_master/lib/macosx/libboost_filesystem.a:
/Users/joe/cinder_master/lib/macosx/libboost_system.a:
