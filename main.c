#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <syslog.h>

#include "config.h"
#include "ldapcheck.h"

/* führende Whitespaces entfernen */
static char *ltrim(char *s)
{
    while (*s && isspace((unsigned char)*s))
        s++;
    return s;
}

/*
 * -s Auswertung:
 *   host                 -> LDAP_PLAIN
 *   ldap://host          -> LDAP_PLAIN
 *   ldap+tls://host      -> LDAP_STARTTLS
 *   ldaps://host         -> LDAP_LDAPS
 */
static const char *build_ldap_uri(const char *arg, int *mode)
{
    static char uri[512];

    *mode = LDAP_PLAIN;

    if (!arg || !*arg)
        return LDAP_DEFAULT_URI;

    if (!strncmp(arg, "ldaps://", 8)) {
        *mode = LDAP_LDAPS;
        return arg;
    }

    if (!strncmp(arg, "ldap+tls://", 11)) {
        *mode = LDAP_STARTTLS;
        snprintf(uri, sizeof(uri), "ldap://%s", arg + 11);
        return uri;
    }

    if (!strncmp(arg, "ldap://", 7)) {
        *mode = LDAP_PLAIN;
        return arg;
    }

    /* nur Hostname → unverschlüsseltes LDAP */
    snprintf(uri, sizeof(uri), "ldap://%s", arg);
    return uri;
}

int main(int argc, char **argv)
{
    char line[1024];
    char user[512] = "";
    char pass[512] = "";

    const char *ldap_uri = LDAP_DEFAULT_URI;
    int ldap_mode = LDAP_PLAIN;
    int debug = 0;
    int opt;

    /* getopt still */
    opterr = 0;

    while ((opt = getopt(argc, argv, ":s:d")) != -1) {
        switch (opt) {
        case 's':
            ldap_uri = build_ldap_uri(optarg, &ldap_mode);
            break;
        case 'd':
            debug = 1;
            break;
        default:
            break;
        }
    }

    /* syslog / journald */
    openlog("onn_ldapcheck", LOG_PID, LOG_AUTH);

    if (debug) {
        syslog(LOG_DEBUG, "starting onn_ldapcheck");
        syslog(LOG_DEBUG, "ldap uri=%s mode=%d", ldap_uri, ldap_mode);
    }

    /* INN-Input lesen */
    while (fgets(line, sizeof(line), stdin)) {
        if (debug)
            syslog(LOG_DEBUG, "stdin: %s", line);

        if (!strncmp(line, "ClientAuthname:", 15)) {
            strncpy(user, ltrim(line + 15), sizeof(user) - 1);
            user[sizeof(user) - 1] = '\0';
        } else if (!strncmp(line, "ClientPassword:", 15)) {
            strncpy(pass, ltrim(line + 15), sizeof(pass) - 1);
            pass[sizeof(pass) - 1] = '\0';
        }
    }

    /* nur Zeilenenden entfernen */
    user[strcspn(user, "\r\n")] = '\0';
    pass[strcspn(pass, "\r\n")] = '\0';

    if (debug) {
        syslog(LOG_DEBUG, "final username='%s'", user);
        syslog(LOG_DEBUG, "final password='%s'", pass);
    }

    if (!*user || !*pass) {
        syslog(LOG_WARNING, "missing username or password");
        return 1;
    }

    if (ldap_pwd_ok(ldap_uri, ldap_mode, user, pass)) {
        /* INN-Pflichtausgabe */
        printf("User:%s\n", user);
        fflush(stdout);

        syslog(LOG_INFO, "authentication SUCCESS for %s", user);
        return 0;
    }

    syslog(LOG_INFO, "authentication FAILED for %s", user);
    return 1;
}
