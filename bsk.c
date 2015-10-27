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
        fail(WITHOUT_ERRNO, "Unable to start PAM: %d (%s)", r, pam_strerror(pamh, r));
    if(!pamh)
        fail(WITHOUT_ERRNO, "Unable to start PAM: %d (%s)", r, pam_strerror(pamh, r));

    r = pam_set_item(pamh, PAM_USER_PROMPT, BSK_USER_PROMPT);
    if(r != PAM_SUCCESS)
        fail(WITHOUT_ERRNO, "Unable to set PAM config: %d (%s)", r, pam_strerror(pamh, r));

    r = pam_authenticate(pamh, 0);
    if(r != PAM_SUCCESS)
        fail(WITHOUT_ERRNO, "Unable to authenticaete: %d (%s)", r, pam_strerror(pamh, r));

    r = pam_acct_mgmt(pamh, PAM_SILENT | PAM_DISALLOW_NULL_AUTHTOK);
    if(r != PAM_SUCCESS)
        fail(WITHOUT_ERRNO, "Access denied: %d (%s)", r, pam_strerror(pamh, r));

    run(stdin, stdout);

    pam_end(pamh, PAM_SUCCESS);
    return 0;
}
