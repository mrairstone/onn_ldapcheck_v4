========================================
1. Varianten und Ursprung
========================================

2002  Bas Meijer
2016  Arnold Schiller
2026  Timo Schaffert

========================================
2. Lizenz
========================================

Copyright (C) 2026
Bas Meijer
Arnold Schiller
Timo Schaffert

Dieses Programm ist freie Software: Sie können es unter den Bedingungen
der GNU General Public License, wie von der Free Software Foundation
veröffentlicht, weiterverbreiten und/oder modifizieren, entweder gemäß
Version 3 der Lizenz oder (nach Ihrer Wahl) jeder späteren Version.

Dieses Programm wird in der Hoffnung verbreitet, dass es nützlich sein
wird, jedoch OHNE JEGLICHE GARANTIE; sogar ohne die implizite Garantie
der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
Siehe die GNU General Public License für weitere Details.

Sie sollten eine Kopie der GNU General Public License zusammen mit
diesem Programm erhalten haben. Falls nicht, siehe:
https://www.gnu.org/licenses/

========================================
3. Allgemeines
========================================

`ldapcheck` ist ein externer Passwort-Authenticator für **INN / nnrpd**,
der Benutzer gegen ein LDAP-Verzeichnis authentifiziert.

Die vorliegende Version ist eine grundlegende Weiterentwicklung der
ursprünglichen Implementierungen von Bas Meijer (2002) und
Arnold Schiller (2016) und wurde für Debian 13 (trixie) modernisiert.

Neben technischer Bereinigung wurden funktionale Erweiterungen ergänzt,
insbesondere ein eigenständiger Debug-Modus sowie Unterstützung
verschiedener LDAP-Verbindungs- und Verschlüsselungsmodi.

========================================
4. Verbindung und Verschlüsselung
========================================

Die Art der LDAP-Verbindung wird über den Parameter `-s` gesteuert.

Unterstützte Modi:

# unverschlüsselte LDAP-Verbindung
ldapcheck -s ldap.example.org

# LDAP mit StartTLS
ldapcheck -s ldap+tls://ldap.example.org

# LDAP über SSL (LDAPS)
ldapcheck -s ldaps://ldap.example.org

Wird der Parameter `-s` nicht gesetzt, erfolgt automatisch ein
Verbindungsversuch über unverschlüsseltes `ldap://` zu dem in der Datei
`config.h` definierten Hostnamen (`LDAP_DEFAULT_URI`).

Achtung:
Bei ungültigen oder nicht vertrauenswürdigen Zertifikaten schlägt die
Verbindung fehl.

========================================
5. Einbindung in INN2 (readers.conf)
========================================

Beispielkonfiguration für die Datei `readers.conf`:

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
6. Abhängigkeiten und Kompilierung
========================================

Benötigte Pakete unter Debian 13 (trixie):

- ca-certificates
- make
- build-essential
- libldap2-dev
- libsasl2-dev
- libssl-dev
- inn2-dev

Kompilierung:

make clean
make

Installationspfad:

/usr/lib/news/bin/auth/passwd/ldapcheck

Dateirechte:

chmod 755 ldapcheck
chown root:news ldapcheck

========================================
7. Debug-Modus
========================================

Der Debug-Modus wird über den Parameter `-d` aktiviert.

Beispiel:

ldapcheck -s ldap.example.org -d

Im Debug-Modus werden zusätzliche Informationen zu jedem
Authentifizierungsversuch in das Syslog bzw. journald geschrieben.

Live-Ausgabe:

journalctl -f -t onn_ldapcheck

Wichtiger Hinweis:
Im Debug-Modus werden Benutzerkennungen und Passwörter im Klartext
protokolliert. Dieser Modus darf ausschließlich zu Test- und
Diagnosezwecken mit nicht-produktiven Zugangsdaten verwendet werden.

Beispielausgabe:

onn_ldapcheck[12345]: starting onn_ldapcheck
onn_ldapcheck[12345]: ldap uri=ldap+tls://ldap.example.org mode=1
onn_ldapcheck[12345]: final username='user@example.org'
onn_ldapcheck[12345]: final password='secret'
onn_ldapcheck[12345]: using LDAP + StartTLS
onn_ldapcheck[12345]: authentication SUCCESS for user@example.org

========================================
8. GPL-Erweiterung / Offenlegungspflicht
========================================

Alle weitergegebenen Kopien dieses Programms – unabhängig davon, ob sie
unverändert oder verändert sind – müssen vollständig unter den
Bedingungen der GNU General Public License in der Version 3 oder einer
späteren Version stehen.

Bei jeder Weitergabe ist der vollständige zugehörige Quellcode entweder
beizulegen oder auf eine für den Empfänger frei zugängliche Weise
bereitzustellen.

Es ist nicht zulässig, zusätzliche Einschränkungen oder weitergehende
Nutzungsbeschränkungen aufzuerlegen, die über die Bedingungen der
GNU General Public License hinausgehen.

Diese Regelung gilt ausdrücklich auch für abgeleitete Werke,
Modifikationen und Zusammenstellungen, sofern sie auf diesem Programm
basieren.
