#include "run.h"
#include "defines.h"
#include "common.h"

#include <security/pam_appl.h>
#include <security/pam_misc.h>


static struct pam_conv conv = {
    .conv = misc_conv,
    .appdata_ptr = NULL
};

int main() {
    pam_handle_t *pamh;
    int r = pam_start(BSK_SERVICE_NAME, NULL, &conv, &pamh);
    if(r != PAM_SUCCESS)
        fail("Unable to start PAM: %d", r);
    if(!pamh)
        fail("Unable to start PAM");

    r = pam_set_item(pamh, PAM_USER_PROMPT, BSK_USER_PROMPT);
    if(r != PAM_SUCCESS)
        fail("Unable to set PAM config: %d", r);

    r = pam_authenticate(pamh, 0);
    if(r != PAM_SUCCESS)
        fail("Unable to authenticaete: %d", r);

    run(stdin, stdout);

    pam_end(pamh, PAM_SUCCESS);
    return 0;
}
