# SAMBA PACKAGE FOR FREERADIUS IN PFSENSE 2.1 and 2.2 beta

If You want to use Freeradius in PfSense 2.1/2.2 and log in users using Active Directory authentication this package is as far as I know only way to do that.


**LICENSE**

>GNU GPL

**REQUIRMENTS**

PFSense 2.1 amd64 or PFSense 2.2

At least 170 MB of free space.

Other machine with Apache installed

##INSTALLATION INSTRUCTION for PFSENSE 2.1
**(valid until it is not part of pfSense packages repository)**

By installing it user accepts that I'm not responsible for any damage, problems this software could make.

1. First step is to download tbz folder into your pfsense box.
2. In tbz folder execute:
```bash
$ pkg_add samba36-3.6.12.tbz
$ killall smbd nmbd winbindd
```
3. Now create your own package repository (tutorial - http://doc.pfsense.org/index.php/Creating_Your_Own_Package_Repository)
4. Add below code to your pkg_config.8.xml.amd64 file in packages section:
```xml
<package>
		<name>Samba</name>
		<pkginfolink>http://forum.pfsense.org/</pkginfolink>
		<descr><![CDATA[Samba package to use with freeradius]]></descr>
		<depends_on_package_base_url>http://${yourserverhostname}/packages/config/samba/</depends_on_package_base_url>
		<depends_on_package>samba36-3.6.12.tbz</depends_on_package>
		<category>Services</category>
		<version>0.1.1</version>
		<status>Alpha</status>
		<required_version>2.1</required_version>
		<config_file>http://${yourserverhostname}/packages/config/samba/samba.xml</config_file>
		<maintainer>pszafer@gmail.com</maintainer>
		<configurationfile>samba.xml</configurationfile>
</package>
```
where your server hostname is Apache server IP or hostname.
5. Paste samba package into config folder
6. Now it's time to edit samba.xml. Change value ${hostname} with hostname or ip address of Apache server.
7. If you haven't done it already, go to your pfSense webGUI and go to: http(s)://yourpfSenseIP/pkg_mgr_settings.php. Change base url to your repository server hostname/IP address.
8. Go to available packages and install Samba.
9. After installation completes, go to Services -> Samba. Complete fields in first tab, Settings and after that put credential tab, insert admin username and password, choose Join domain and click button.
10. It's done.

##INSTALLATION INSTRUCTION for PFSENSE 2.2
1. Install samba by new FreeBSD package manager executing:
```bash
$ pkg install samba41
$ killall smbd nmbd winbindd
```
2. Now create your own package repository (tutorial -http://doc.pfsense.org/index.php/Creating_Your_Own_Package_Repository). Follow this tutorial, but put everything in root directory since in pfSense 2.2 developers changed dir where pfSense 2.2 is looking for xmlrpc.php.
3. In repo create packages directory and pull it from github. You need also to edit xmlrpc.php file in line 124 to don't look for packages directory above -> $path_to_files = './packages/';
4. Add below code to your pkg_config.10.xml file in packages section:
```xml
<package>
		<name>Samba</name>
		<pkginfolink>http://forum.pfsense.org/</pkginfolink>
		<descr><![CDATA[Samba package to use with freeradius]]></descr>
		<depends_on_package_base_url>http://${yourserverhostname}/packages/config/samba/</depends_on_package_base_url>
		<depends_on_package>samba36-3.6.12.tbz</depends_on_package>
		<category>Services</category>
		<version>0.1.1</version>
		<status>Alpha</status>
		<required_version>2.0</required_version>
		<config_file>http://${yourserverhostname}/packages/config/samba/samba.xml</config_file>
		<maintainer>pszafer@gmail.com</maintainer>
		<configurationfile>samba.xml</configurationfile>
</package>
```
where your server hostname is Apache server IP or hostname.
5. Paste samba package into config folder
6. Now it's time to edit samba.xml. Change value ${hostname} with hostname or ip address of Apache server.
7. If you haven't done it already, go to your pfSense webGUI and go to: http(s)://yourpfSenseIP/pkg_mgr_settings.php. Change base url to your repository server hostname/IP address.
8. Go to available packages and install Samba.
9. After installation completes, go to Services -> Samba. Complete fields in first tab, Settings and after that put credential tab, insert admin username and password, choose Join domain and click button.
10. It's done.

###Freeradius Configuration

* You need to complete LDAP tab's and EAP.

In LDAP put your server settings like (insert normal user without special privileges):

>Server: server.company.org

>Identity: cn=addressbook,cn=Users,cn=company,cn=org (this username I've created in Win Server AD)

>Password: password

>BaseDN: could be empty, but I have something like: ou=Workers,cn=company,cn=org

>Filter: (SAMAccountName=%{mschap:User-Name})

>Base filter: (objectclass=radiusprofile)


In EAP tab:

>Default EAP type: mschapv2,

>in seciton EAP-PEAP choose EAP-type: mschapv2

##INSTALLATION INSTRUCTION for PFSENSE 2.1
**VERSION 1**

Please, remember that this method is not tested yet so create backups!

You can also try to apply prepared patched for freeradius
```bash
$ fetch http://pastebin.com/raw.php?i=CS8fupvz -o /root/freeradius.patch
$ patch /usr/local/pkg/freeradius.inc < /root/freeradius.patch
```
Now go to freeradius config, and click save anywhere. To check if patch applied correctly:
```bash
$ cat /usr/pbi/freeradius-amd64/etc/raddb/modules/mschap | grep "ntlm_auth = "
```
If it is showing /usr/local/bin/ntlm_auth everything is correct. Otherwise, please report mi it.

** VERSION 2 (needed to be applied every restart) - use it if version 2 is not working:**
```bash
$ cd /usr/local/pkg/
$ vi freeradius.inc
```

Find line with ntlm_auth=/path/to/ntlm_auth ....

Edit it as:

ntlm_auth=/usr/local/bin/ntlm_auth ...

***Be aware that last option "nt-response" is in next line, so correct so all command will be in one line.***


**Other info**
In this configuration, in Windows you have to disable 'Verify server certificate'!

I will prepare special instruction about working with certificates later.


*In Wireless configuration you have to choose in Advanced: Authorization using username, not computer.

**TROUBLESHOOTING**

If you have troubles, try to find your problem with doing one of following commands:

- Check system log :)

First check connection with DC:
```bash
$ ps ax | grep winbindd #is winbindd running?
$ winbindd -i #check if winbindd saying something
$ wbinfo -u #can You get information about users
$ net ads join -U adminusername # check if you can join domain
$ ping realmName # are You pinging your ADs server while pinging realm?
$ ldd /usr/local/sbin/winbindd # are all libraries available? if not run libraries.sh script, maybe fetch it manually.
```
Now with radius:
- first add in configuration your pfSense box local IP, for example 192.168.1.1, so you can test from the same shell.

- In below commands You can log in to shell twice (I prefer it, so $ will be shell one, and $$ will be second shell) or run radiusd in background.
```bash
$ killall radiusd # first kill radiusd
$ radiusd -X # run radiusd in verbose mode
$$ radtest -t mschap username password 192.168.1.1 1 passwordInConfig
```
Analyze radiusd -X output if Access-reject of radtest. In radtest passwords cannot be complex, so test on simple passwords!
Good luck!
 
# TODO LIST
***Maybe I will do it someday, but since almost nobody is interested in that, I won't work on it.***
1. remove unnecessary Samba files, leave only that files, necessary to run winbindd.
2. Apply patch to freeradius in samba.inc or to Freeradius developer.
3. Create full uninstallation procedure.

#Good luck!
