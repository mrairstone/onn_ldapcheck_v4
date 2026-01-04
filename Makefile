CC      = cc
CFLAGS  = -O2 -Wall
LIBS    = -lldap -llber -lsasl2 -lssl -lcrypto

OBJS = main.o ldap_pwd_ok.o

all: ldapcheck

ldapcheck: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	rm -f ldapcheck *.o
