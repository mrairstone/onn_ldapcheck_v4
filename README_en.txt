========================================
1. License
========================================

Copyright (C) 2026
(2002) Bas Meijer
(2016) Arnold Schiller
(2026) Timo Schaffert

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see:
https://www.gnu.org/licenses/

========================================
2. Overview
========================================

`ldapcheck` is an external password authenticator for **INN / nnrpd**
that authenticates users against an LDAP directory.

This version is a major revision and modernization of the original
implementations by Bas Meijer (2002) and Arnold Schiller (2016),
updated for Debian 13 (trixie).

In addition to general cleanup and modernization, functional
extensions were added, most notably a dedicated debug mode and
support for multiple LDAP connection and encryption methods.

========================================
3. Connection and Encryption
========================================

The LDAP connection mode is controlled using the `-s` parameter.

Supported modes:

# unencrypted LDAP
ldapcheck -s ldap.example.org

# LDAP with StartTLS
ldapcheck -s ldap+tls://ldap.example.org

# LDAP over SSL (LDAPS)
ldapcheck -s ldaps://ldap.example.org

If the `-s` parameter is not specified, ldapcheck will automatically
attempt an unencrypted `ldap://` connection to the hostname defined
in `config.h` as `LDAP_DEFAULT_URI`.

Warning:
Connections will fail if invalid or untrusted certificates are used.

========================================
4. Integration with INN2 (readers.conf)
========================================

Example configuration for `readers.conf`:

auth "opennewsldap" {
    hosts: "*"
    key: "opennewsldap"
    auth: "ldapcheck -s auth.open-news-network.org"
}

access "opennewsldap" {
    users: "*,!anonymous"
    key: "opennewsldap"
    newsgroups: "*, !internal.*, !special.*, !control.*"
    access: "RP"
}

========================================
5. Dependencies and Compilation
========================================

Required packages on Debian 13 (trixie):

- ca-certificates
- make
- build-essential
- libldap2-dev
- libsasl2-dev
- libssl-dev
- inn2-dev

Compilation:

make clean
make

Installation path:

/usr/lib/news/bin/auth/passwd/ldapcheck

File permissions:

chmod 755 ldapcheck
chown root:news ldapcheck

========================================
6. Debug Mode
========================================

Debug mode is enabled using the `-d` parameter.

Example:

ldapcheck -s ldap.example.org -d

When debug mode is enabled, additional information for each
authentication attempt is written to syslog / journald.

Live log output:

journalctl -f -t onn_ldapcheck

Important warning:
In debug mode, usernames and passwords are logged in clear text.
This mode must only be used for testing and troubleshooting with
non-production credentials.

Example output:

onn_ldapcheck[12345]: starting onn_ldapcheck
onn_ldapcheck[12345]: ldap uri=ldap+tls://ldap.example.org mode=1
onn_ldapcheck[12345]: final username='user@example.org'
onn_ldapcheck[12345]: final password='secret'
onn_ldapcheck[12345]: using LDAP + StartTLS
onn_ldapcheck[12345]: authentication SUCCESS for user@example.org

========================================
7. GPL Extension / Source Disclosure
========================================

All redistributed copies of this program — whether modified or
unmodified — must be licensed in their entirety under the terms of
the GNU General Public License, version 3 or any later version.

When redistributing this program, the complete corresponding source
code must be provided or made freely accessible to the recipient.

No additional restrictions or further usage limitations may be
imposed beyond those defined by the GNU General Public License.

This requirement explicitly applies to derived works, modifications,
and combined works, insofar as they are based on this program.
