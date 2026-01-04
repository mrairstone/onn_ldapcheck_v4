#include <ldap.h>
#include <string.h>
#include <syslog.h>
#include <stdio.h>
#include "config.h"
#include "ldapcheck.h"

int ldap_pwd_ok(const char *ldap_uri,
                int ldap_mode,
                const char *username,
                const char *password)
{
    LDAP *ld;
    char userdn[512];
    struct berval cred;
    int rc, version = LDAP_VERSION3;

    snprintf(userdn, sizeof(userdn), LDAP_USER_DN, username);

    if (ldap_mode != LDAP_PLAIN) {
        ldap_set_option(NULL, LDAP_OPT_X_TLS_REQUIRE_CERT,
                        &(int){LDAP_TLS_REQUIRE_CERT});
        ldap_set_option(NULL, LDAP_OPT_X_TLS_CACERTFILE,
                        LDAP_TLS_CACERT_FILE);
    }

    rc = ldap_initialize(&ld, ldap_uri);
    if (rc != LDAP_SUCCESS) {
        syslog(LOG_ERR, "ldap_initialize failed: %s",
               ldap_err2string(rc));
        return 0;
    }

    ldap_set_option(ld, LDAP_OPT_PROTOCOL_VERSION, &version);

    if (ldap_mode == LDAP_STARTTLS) {
        syslog(LOG_DEBUG, "using LDAP + StartTLS");
        rc = ldap_start_tls_s(ld, NULL, NULL);
        if (rc != LDAP_SUCCESS) {
            syslog(LOG_ERR, "StartTLS failed: %s",
                   ldap_err2string(rc));
            ldap_unbind_ext_s(ld, NULL, NULL);
            return 0;
        }
    } else if (ldap_mode == LDAP_LDAPS) {
        syslog(LOG_DEBUG, "using LDAPS");
    } else {
        syslog(LOG_DEBUG, "using LDAP plaintext");
    }

    cred.bv_val = (char *)password;
    cred.bv_len = strlen(password);

    rc = ldap_sasl_bind_s(
        ld,
        userdn,
        LDAP_SASL_SIMPLE,
        &cred,
        NULL, NULL, NULL
    );

    ldap_unbind_ext_s(ld, NULL, NULL);
    return (rc == LDAP_SUCCESS);
}
