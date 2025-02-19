#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *mail = popen("/usr/sbin/sendmail -t", "w");
    if (mail == NULL) {
        perror("popen");
        return 1;
    }

    fprintf(mail, "To: gomezivannia02@gmail.com\n");
    fprintf(mail, "Subject: Test Email from C\n");
    fprintf(mail, "Hello, this is a test email sent from a C program!\n");
    pclose(mail);

    return 0;
}