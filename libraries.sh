#!/bin/sh
PATHDIR=/usr/local/lib/
LIBASN=libasn1.so.10
LIBGSSAPI=libgssapi.so.10
LIBHEIM=libheimntlm.so.10
LIBHX=libhx509.so.10
LIBKRB=libkrb5.so.10
LIBBROKEN=libroken.so.10
if [ ! -f $PATHDIR$LIBASN ] 
then 
	cd $PATHDIR && fetch http://e-sac.siteseguro.ws/pfsense/8/amd64/All/ldd/$LIBASN
	chmod 0755 $PATHDIR$LIBASN
fi
if [ ! -f $PATHDIR$LIBGSSAPI ] 
then 
	cd $PATHDIR && fetch http://e-sac.siteseguro.ws/pfsense/8/amd64/All/ldd/$LIBGSSAPI 
	chmod 0755 $PATHDIR$LIBGSSAPI
fi
if [ ! -f $PATHDIR$LIBHEIM ] 
then 
	cd $PATHDIR && fetch http://e-sac.siteseguro.ws/pfsense/8/amd64/All/ldd/$LIBHEIM
	chmod 0755 $PATHDIR$LIBHEIM
fi
if [ ! -f $PATHDIR$LIBHX ] 
then 
	cd $PATHDIR && fetch http://e-sac.siteseguro.ws/pfsense/8/amd64/All/ldd/$LIBHX
	chmod 0755 $PATHDIR$LIBHX
	echo "chmod 0755 $PATHDIR$LIBHX"
fi
if [ ! -f $PATHDIR$LIBKRB ] 
then 
	cd $PATHDIR && fetch http://e-sac.siteseguro.ws/pfsense/8/amd64/All/ldd/$LIBKRB
	chmod 0755 $PATHDIR$LIBKRB
fi
if [ ! -f $PATHDIR$LIBBROKEN ] 
then 
	cd $PATHDIR && fetch http://e-sac.siteseguro.ws/pfsense/8/amd64/All/ldd/$LIBBROKEN
	chmod 0755 $PATHDIR$LIBBROKEN
fi
