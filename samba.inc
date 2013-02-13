<?php
/* copyright */
/* ========================================================================== */
/*
	samba.inc
	mail - pszafer@gmail.com
	part of pfSense (http://www.pfSense.com)
	Copyright (C) 2011 - 2012 Alexander Wilke <nachtfalkeaw@web.de>
	All rights reserved.

	Based on m0n0wall (http://m0n0.ch/wall)
	Copyright (C) 2003-2006 Manuel Kasper <mk@neon1.net>.
	All rights reserved.
									      */
/* ========================================================================== */
/*
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice,
	   this list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
	INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
	AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
	AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
	ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
									      */
/* ========================================================================== */

require_once('config.inc');
require_once('service-utils.inc');
require_once("util.inc");
require_once("functions.inc");
require_once("pkg-utils.inc");
require_once("globals.inc");
require_once("filter.inc");
require_once("services.inc");

// Check pfSense version
$pfs_version = substr(trim(file_get_contents("/etc/version")),0,3);
switch ($pfs_version) {
	case "1.2":
	case "2.0":
		define('SAMBA_BASE', '/usr/local/samba');
		break;
	default:
		#define('SAMBA_BASE', '/usr/local/samba-' . php_uname("m"));
		define('SAMBA_BASE', '/usr/local/');
}
// End: Check pfSense version

function samba_deinstall_command() {
	if (substr(trim(file_get_contents("/etc/version")),0,3) == "2.0") {
		#exec("cd /var/db/pkg && pkg_delete `ls | grep freeradius`");
		#exec("rm -rf " . FREERADIUS_BASE . "/etc/raddb");
		#exec("rm -rf /var/run/radiusd/");
	}
}

function samba_install_command() {
	exec("touch /usr/pbi/test");
	global $config;
	conf_mount_rw();
	#exec("pkg_add /tmp/samba36-3.6.12.tbz");
	// put the constant to a variable
	$varSAMBA_BASE = SAMBA_BASE;
	exec("echo " . SAMBA_BASE . ">> /usr/pbi/test");
	exec("mkdir " . SAMBA_BASE);
	exec("mkdir /var/run/samba/");
	link_samba_libraries();
	
	samba_settings_resync();
	
	conf_mount_rw();
	$rcfile = array();
	$rcfile['file'] = 'samba.sh';
	$rcfile['start'] = '/usr/local/etc/rc.d/samba onestart';
	$rcfile['stop'] = '/usr/local/etc/rc.d/samba onestop';
	write_rcfile($rcfile);
	conf_mount_ro();
	start_service("samba");
	#$rcfile = array();
	#$rcfile['file'] = 'radiusd.sh';
	##$rcfile['start'] = "$varFREERADIUS_BASE" . '/etc/rc.d/radiusd onestart';
	#$rcfile['stop'] = "$varFREERADIUS_BASE" . '/etc/rc.d/radiusd onestop';
	#write_rcfile($rcfile);
	#	conf_mount_ro();
	#start_service("radiusd");
}

function samba_settings_resync() {
	global $config;
	$conf = '';

	// put the constant to a variable
	$varSAMBA_BASE = SAMBA_BASE;
	
	#we need libraries from samba

	$varsettings = $config['installedpackages']['samba']['config'][0];
	
	// Variables: General configuration
	$varrealm = ($varsettings['realm']?$varsettings['realm']:'');
	$varworkgroup = ($varsettings['workgroup']?$varsettings['workgroup']:'');
	$varpasswordserver = ($varsettings['passserver']?$varsettings['passserver']:'*');

	$conf .= <<<EOD
# This is the main Samba configuration file. You should read the
# smb.conf(5) manual page in order to understand the options listed
# here. Samba has a huge number of configurable options (perhaps too
# many!) most of which are not shown in this example
#
# For a step to step guide on installing, configuring and using samba, 
# read the Samba-HOWTO-Collection. This may be obtained from:
#  http://www.samba.org/samba/docs/Samba-HOWTO-Collection.pdf
#
# Many working examples of smb.conf files can be found in the 
# Samba-Guide which is generated daily and can be downloaded from: 
#  http://www.samba.org/samba/docs/Samba-Guide.pdf
#
# Any line which starts with a ; (semi-colon) or a # (hash) 
# is a comment and is ignored. In this example we will use a #
# for commentry and a ; for parts of the config file that you
# may wish to enable
#
# NOTE: Whenever you modify this file you should run the command "testparm"
# to check that you have not made any basic syntactic errors. 
#
#======================= Global Settings =====================================
[global]
# workgroup = NT-Domain-Name or Workgroup-Name, eg: MIDEARTH
   workgroup = $varworkgroup

# server string is the equivalent of the NT Description field
#   server string = Samba Server

# Security mode. Defines in which mode Samba will operate. Possible 
# values are share, user, server, domain and ads. Most people will want 
# user level security. See the Samba-HOWTO-Collection for details.
   security = ADS 

# This option is important for security. It allows you to restrict
# connections to machines which are on your local network. The
# following example restricts access to two C class networks and
# the "loopback" interface. For more examples of the syntax see
# the smb.conf man page
;   hosts allow = 192.168.1. 192.168.2. 127. 10.11.0. 10.8.0. 192.168.90.

# If you want to automatically load your printer list rather
# than setting them up individually then you'll need this
#   load printers = yes

# you may wish to override the location of the printcap file
;   printcap name = /etc/printcap

# on SystemV system setting printcap name to lpstat should allow
# you to automatically obtain a printer list from the SystemV spool
# system
;   printcap name = lpstat

# It should not be necessary to specify the print system type unless
# it is non-standard. Currently supported print systems include:
# bsd, cups, sysv, plp, lprng, aix, hpux, qnx
;   printing = cups

# Uncomment this if you want a guest account, you must add this to /etc/passwd
# otherwise the user "nobody" is used
;  guest account = pcguest

# this tells Samba to use a separate log file for each machine
# that connects
;   log file = /usr/local/samba/var/log.%m

syslog = 3
syslog only = yes

# Put a capping on the size of the log files (in Kb).
;   max log size = 50

# Use password server option only with security = server
# The argument list may include:
#   password server = My_PDC_Name [My_BDC_Name] [My_Next_BDC_Name]
# or to auto-locate the domain controller/s
#   password server = *
password server = $varpasswordserver

# Use the realm option only with security = ads
# Specifies the Active Directory realm the host is part of
realm = $varrealm

# Backend to store user information in. New installations should 
# use either tdbsam or ldapsam. smbpasswd is available for backwards 
# compatibility. tdbsam requires no further configuration.
;   passdb backend = tdbsam

# Using the following line enables you to customise your configuration
# on a per machine basis. The %m gets replaced with the netbios name
# of the machine that is connecting.
# Note: Consider carefully the location in the configuration file of
#       this line.  The included file is read at that point.
;   include = /usr/local/samba/lib/smb.conf.%m

# Configure Samba to use multiple interfaces
# If you have multiple network interfaces then you must list them
# here. See the man page for details.
;   interfaces = 192.168.12.2/24 192.168.13.2/24 

# Browser Control Options:
# set local master to no if you don't want Samba to become a master
# browser on your network. Otherwise the normal election rules apply
;   local master = no

# OS Level determines the precedence of this server in master browser
# elections. The default value should be reasonable
;   os level = 33

# Domain Master specifies Samba to be the Domain Master Browser. This
# allows Samba to collate browse lists between subnets. Don't use this
# if you already have a Windows NT domain controller doing this job
;   domain master = yes 

# Preferred Master causes Samba to force a local browser election on startup
# and gives it a slightly higher chance of winning the election
;   preferred master = yes

# Enable this if you want Samba to be a domain logon server for 
# Windows95 workstations. 
;   domain logons = yes

# if you enable domain logons then you may want a per-machine or
# per user logon script
# run a specific logon batch file per workstation (machine)
;   logon script = %m.bat
# run a specific logon batch file per username
;   logon script = %U.bat

# Where to store roving profiles (only for Win95 and WinNT)
#        %L substitutes for this servers netbios name, %U is username
#        You must uncomment the [Profiles] share below
;   logon path = \\%L\Profiles\%U

# Windows Internet Name Serving Support Section:
# WINS Support - Tells the NMBD component of Samba to enable it's WINS Server
;   wins support = yes

# WINS Server - Tells the NMBD components of Samba to be a WINS Client
#	Note: Samba can be either a WINS Server, or a WINS Client, but NOT both
;   wins server = w.x.y.z

# WINS Proxy - Tells Samba to answer name resolution queries on
# behalf of a non WINS capable client, for this to work there must be
# at least one	WINS Server on the network. The default is NO.
;   wins proxy = yes

# DNS Proxy - tells Samba whether or not to try to resolve NetBIOS names
# via DNS nslookups. The default is NO.
   dns proxy = no 
   
EOD;

$filename = SAMBA_BASE . '/etc/smb.conf';
	conf_mount_rw();
	file_put_contents($filename, $conf);
	chmod($filename, 0640);
	conf_mount_ro();
	samba_settings_resync_service();
}

function samba_settings_resync_service(){
	global $config;
	$conf = '';

	$conf .= <<<EOD
#!/bin/sh
#
# $FreeBSD: ports/net/samba36/files/samba.in,v 1.8 2013/01/31 16:45:56 svnexp Exp $
#

# PROVIDE: nmbd smbd
# PROVIDE: winbindd
# REQUIRE: NETWORKING SERVERS DAEMON ldconfig resolv
# BEFORE: LOGIN
# KEYWORD: shutdown
#
# Add the following lines to /etc/rc.conf.local or /etc/rc.conf
# to enable this service:
#
samba_enable="YES"
# You can disable/enable any of the Samba daemons by specifying:
#nmbd_enable="NO"
#smbd_enable="NO"
# You need to enable winbindd separately, by adding:
winbindd_enable="YES"
#
# Configuration file can be set with:
#samba_config="/usr/local/etc/smb.conf"
#

. /etc/rc.subr

name="samba"
rcvar=samba_enable
# Custom commands
extra_commands="reload status"
start_precmd="samba_start_precmd"
start_cmd="samba_cmd"
stop_cmd="samba_cmd"
status_cmd="samba_cmd"
restart_precmd="samba_checkconfig"
reload_precmd="samba_checkconfig"
reload_cmd="samba_reload_cmd"
rcvar_cmd="samba_rcvar_cmd"
# Defaults
samba_config_default="/usr/local/etc/smb.conf"
testparm_command="/usr/local/bin/testparm"
smbcontrol_command="/usr/local/bin/smbcontrol"
samba_parm="\${testparm_command} -s -v --parameter-name"
pid_extra=
#
samba_daemons="nmbd smbd"
samba_daemons="\${samba_daemons} winbindd"

samba_checkconfig() {
    echo -n "Performing sanity check on Samba configuration: "
    if \${testparm_command} -s \${samba_config:+"\${samba_config}"} >/dev/null 2>&1; then #"
	echo "OK"
    else
	echo "FAILED"
	return 1
    fi
    return 0
}

samba_start_precmd() {
    # Make sure rundir exists
    if [ ! -d "/var/run/samba" -a ! -e "/var/run/samba" ]; then
	install -m 0755 -d "/var/run/samba"
    fi
    # XXX: Never delete winbindd_idmap, winbindd_cache and group_mapping
    if [ -n "\${samba_lockdir}" -a -d "\${samba_lockdir}" ]; then
	echo -n "Removing stale Samba tdb files: "
	for file in brlock.tdb browse.dat connections.tdb gencache.tdb \
		    locking.tdb messages.tdb namelist.debug sessionid.tdb \
		    unexpected.tdb
	do
	    rm "\${samba_lockdir}/\${file}" </dev/null 2>/dev/null && echo -n '.'
	done
	echo " done"
    fi
}

samba_rcvar_cmd() {
    local name rcvar
    rcvar=\${name}_enable
    # Prevent recursive calling
    unset "\${rc_arg}_cmd" "\${rc_arg}_precmd" "\${rc_arg}_postcmd"
    # Check master variable
    run_rc_command "\${_rc_prefix}\${rc_arg}" \${rc_extra_args}
    # Check dependent variables
    for name in \${samba_daemons}; do
	# XXX
	rcvars=''; v=''
	rcvar=\${name}_enable
	run_rc_command "\${_rc_prefix}\${rc_arg}" \${rc_extra_args}
    done
}

samba_reload_cmd() {
    local name rcvar command pidfile force_run
    # Prevent recursive calling
    unset "\${rc_arg}_cmd" "\${rc_arg}_precmd" "\${rc_arg}_postcmd"
    # Ignore rcvar and run command
    if [ -n "\${_rc_prefix}" -a "\${_rc_prefix}" = "one" ] || [ -n "\${rc_force}" ] || [ -n "\${rc_fast}" ]; then
	force_run=yes
    fi
    # Apply to all daemons
    for name in \${samba_daemons}; do
	rcvar=\${name}_enable
	command="/usr/local/sbin/\${name}"
	pidfile="/var/run/samba/\${name}\${pid_extra}.pid"
	# Daemon should be enabled and running
	if ( [ -n "\${rcvar}" ] && checkyesno "\${rcvar}" ) || [ -n "$force_run" ]; then
	    if [ -n "$(check_pidfile "\${pidfile}" "\${command}")" ]; then
		debug "reloading \${name} configuration"
		echo "Reloading \${name}."
		# XXX: Hack with pid_extra
		\${smbcontrol_command} "\${name}\${pid_extra}" 'reload-config' \${command_args} >/dev/null 2>&1
	    fi
	fi
    done
}

samba_cmd() {
    local name rcvar rcvars v command pidfile samba_daemons result force_run
    # Prevent recursive calling
    unset "\${rc_arg}_cmd" "\${rc_arg}_precmd" "\${rc_arg}_postcmd"
    # Stop processes in the reverse order
    if [ "\${rc_arg}" = "stop" ] ; then
	samba_daemons=$(reverse_list \${samba_daemons})
    fi
    # Ignore rcvar and run command
    if [ -n "\${_rc_prefix}" -a "\${_rc_prefix}" = "one" ] || [ -n "\${rc_force}" ] || [ -n "\${rc_fast}" ]; then
	force_run=yes
    fi
    # Assume success
    result=0
    # Apply to all daemons
    for name in \${samba_daemons}; do
	# XXX
	rcvars=''; v=''
	rcvar=\${name}_enable
	command="/usr/local/sbin/\${name}"
	pidfile="/var/run/samba/\${name}\${pid_extra}.pid"
	# Daemon should be enabled and running
	if ( [ -n "\${rcvar}" ] && checkyesno "\${rcvar}" ) || [ -n "$force_run" ]; then
	    run_rc_command "\${_rc_prefix}\${rc_arg}" \${rc_extra_args}
	    # If any of the commands failed, take it as a global result
	    result=$((\${result} || $?))
	fi
    done
    return \${result}
}

samba_config_init() {
    local name
    # Load configuration
    load_rc_config "\${name}"
    for name in \${samba_daemons}; do
	load_rc_config "\${name}"
    done
    # Location of the config
    samba_config=\${samba_config="\${samba_config_default}"} #"
    # XXX: Hack to work around name change of pid file with non-default config
    if [ -n "\${samba_config}" -a "\${samba_config}" != "\${samba_config_default}" ]; then
	pid_extra="-$(basename "\${samba_config}")"
    fi
    # Setup dependent variables
    if [ -n "\${rcvar}" ] && checkyesno "\${rcvar}"; then
	nmbd_enable=\${nmbd_enable=YES}
	smbd_enable=\${smbd_enable=YES}
    fi
    # Defaults
    samba_enable=\${samba_enable:=NO}
    nmbd_enable=\${nmbd_enable:=NO}
    smbd_enable=\${smbd_enable:=NO}
    # Winbindd
    if [ -z "\${winbind_enable}" ]; then
	samba_idmap=$(\${samba_parm} 'idmap uid' "\${samba_config}" 2>/dev/null)
	# Check that winbindd is actually configured
	if [ -n "\${samba_idmap}" ]; then
		winbindd_enable="YES"
	fi
	winbindd_enable=\${winbindd_enable:=NO}
    fi
}

# Load configuration variables
samba_config_init
# Where is the lockdir
samba_lockdir=$(\${samba_parm} 'lock directory' "\${samba_config}" 2>/dev/null)
# Common flags
command_args=\${samba_config:+-s "\${samba_config}"} #"
nmbd_flags=\${nmbd_flags="-D"}
smbd_flags=\${smbd_flags="-D"}
winbindd_flags=\${winbindd_flags=''}
# Requirements
required_files="\${samba_config}"
required_dirs="\${samba_lockdir}"

run_rc_command "$1"
   
EOD;

$filename = SAMBA_BASE . '/etc/rc.d/samba';
	conf_mount_rw();
	file_put_contents($filename, $conf);
	chmod($filename, 0755);
	conf_mount_ro();
}

function link_samba_libraries(){
	exec("/bin/sh /usr/local/pkg/libraries.sh");
	$varSAMBA_BASE = SAMBA_BASE;
	exec("ldconfig -m ".$varSAMBA_BASE."/lib");
}

function samba_join_command() {
	if ($_POST['domainjoin'] === "join_yes") {
		$varadminname = $_POST['username'];
		$varadminpassword = $_POST['password'];
		join_domain($varadminname, $varadminpassword);
	}
	elseif ($_POST['domainjoin'] === "join_leave"){
		$varadminname = $_POST['username'];
		$varadminpassword = $_POST['password'];
		leave_domain($varadminname, $varadminpassword);
	}
	else {
		print("You have to choose 'Yes' from selection menu if you want join domain. <br /><input type=\"button\" onclick=\"history.back();\" value=\"Back\">");
	}
}
	
function join_domain($admin, $password){
	$command = 'net ads join -U \''.$admin.'\'%\''.$password.'\'';
	print("<b>RESULT of joining:</b>");
	system($command);
	print("<br /><input type=\"button\" onclick=\"history.back();\" value=\"Back\">");
	start_service("samba");
	exit;
}

function leave_domain($admin, $password){
	$command = 'net ads leave -U \''.$admin.'\'%\''.$password.'\'';
	print("<b>RESULT of leaving:</b>");
	system($command);
	print("<br /><input type=\"button\" onclick=\"history.back();\" value=\"Back\">");
	exit;
}

?>
