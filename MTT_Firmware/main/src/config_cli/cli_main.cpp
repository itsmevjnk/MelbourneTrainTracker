#include "config.h"

#include "subsystems/fs.h"
#include "subsystems/uart.h"

#include "esp_check.h"

/* defaults */
#ifndef CONFIG_PROMPT
#define CONFIG_PROMPT                           "config>"
#endif

#ifndef CONFIG_MAX_CMDLINE_LENGTH
#define CONFIG_MAX_CMDLINE_LENGTH               128
#endif

const esp_console_cmd_t* Config::kCommands[] = {
    &kExitCommand, &kShowConfigCommand, &kShowCertificateCommand, &kSetWiFiCommand, &kSetNTPCommand, &kSetMQTTCommand
};

esp_err_t Config::cliRegisterCommands() {
    size_t numCommands = sizeof(kCommands) / sizeof(esp_console_cmd_t*);
    for (size_t i = 0; i < numCommands; i++) {
        const esp_console_cmd_t* command = kCommands[i];
        ESP_RETURN_ON_ERROR(esp_console_cmd_register(command), kTag, "cannot register %s command", command->command);
    }
    
    ESP_LOGI(kTag, "registered all commands");
    return ESP_OK;
}

esp_console_repl_t* Config::m_repl = nullptr;
esp_err_t Config::cli() {
    ESP_RETURN_ON_ERROR(UART::deinit(), kTag, "cannot de-initialise UART driver"); // so that we don't crash when esp_console does its thing

    esp_console_repl_config_t replConfig = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    replConfig.prompt = CONFIG_PROMPT;
    replConfig.max_cmdline_length = CONFIG_MAX_CMDLINE_LENGTH;

    /* register commands */
    ESP_RETURN_ON_ERROR(esp_console_register_help_command(), kTag, "cannot register help command");
    ESP_RETURN_ON_ERROR(cliRegisterCommands(), kTag, "error encountered registering commands");

    /* bind repl with serial console on UART */
    esp_console_dev_uart_config_t hwConfig = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    ESP_RETURN_ON_ERROR(esp_console_new_repl_uart(&hwConfig, &replConfig, &m_repl), kTag, "cannot set up repl on UART");
    ESP_RETURN_ON_ERROR(esp_console_start_repl(m_repl), kTag, "cannot start repl");

    return ESP_OK;
}