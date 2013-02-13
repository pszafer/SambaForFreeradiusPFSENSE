#!/bin/sh
cd /usr/local/lib/ && fetch http://e-sac.siteseguro.ws/pfsense/8/amd64/All/ldd/libasn1.so.10
cd /usr/local/lib/ && fetch http://e-sac.siteseguro.ws/pfsense/8/amd64/All/ldd/libgssapi.so.10
cd /usr/local/lib/ && fetch http://e-sac.siteseguro.ws/pfsense/8/amd64/All/ldd/libheimntlm.so.10
cd /usr/local/lib/ && fetch http://e-sac.siteseguro.ws/pfsense/8/amd64/All/ldd/libhx509.so.10
cd /usr/local/lib/ && fetch http://e-sac.siteseguro.ws/pfsense/8/amd64/All/ldd/libkrb5.so.10
cd /usr/local/lib/ && fetch http://e-sac.siteseguro.ws/pfsense/8/amd64/All/ldd/libroken.so.10
chmod 0755 /usr/local/lib/libasn1.so.10
chmod 0755 /usr/local/lib/libgssapi.so.10
chmod 0755 /usr/local/lib/libheimntlm.so.10
chmod 0755 /usr/local/lib/libhx509.so.10
chmod 0755 /usr/local/lib/libkrb5.so.10
chmod 0755 /usr/local/lib/libroken.so.10
