# SAMBA PACKAGE FOR FREERADIUS IN PFSENSE 2.1

If You want to use Freeradius in PfSense 2.1 and log in users using Active Directory authentication this package is as far as I know necessary.


**LICENSE**
GNU GPL

**REQUIRMENTS**
PFSense 2.1 amd64
At least 170 MB of free space.
Other machine with Apache installed

**INSTALLATION INSTRUCTION**
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

Paste samba directory into config and you need to edit samba.xml file!

5. 
 


To be continued...
