#include "config.h"

esp_console_cmd_t Config::kExitCommand = {
    .command = "exit",
    .help = "exit from the config CLI and reboot",
    .hint = NULL,
    .func = &cliExit
};

esp_err_t Config::cliExit(int argc, char** argv) {
    printf("Rebooting.\r\n");
    esp_restart();
}
