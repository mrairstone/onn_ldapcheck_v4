#ifndef LDAPCHECK_H
#define LDAPCHECK_H

/* LDAP Modi */
#define LDAP_PLAIN     0
#define LDAP_STARTTLS  1
#define LDAP_LDAPS     2

int ldap_pwd_ok(const char *ldap_uri,
                int ldap_mode,
                const char *username,
                const char *password);

#endif
