/* config.h - defines for ldapcheck, an authenticator for inn 2.7 and OpenLDAP 2.6 */

/* 2002 Bas Meijer */
/* 2016 Arnold Schiller*/
/* 2026 Timo Schaffert*/
/* change these for your setup */

#ifndef CONFIG_H
#define CONFIG_H

/* Default, wenn -s nicht gesetzt */
#define LDAP_DEFAULT_URI "ldap://auth.open-news-network.org"

/* DN */
#define LDAP_USER_DN "UID=%s,DC=open-news-network,DC=org"

/* TLS Defaults (für StartTLS & LDAPS) */
#define LDAP_TLS_REQUIRE_CERT LDAP_OPT_X_TLS_DEMAND
#define LDAP_TLS_CACERT_FILE "/etc/ssl/certs/ca-certificates.crt"

#endif
