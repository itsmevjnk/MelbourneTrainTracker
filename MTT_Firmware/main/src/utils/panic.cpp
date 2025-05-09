#include "services.h"
#include "hwconfig/pindefs.h"

extern "C" void __real_esp_panic_handler(void*);

extern "C" void __wrap_esp_panic_handler(void* info) {
    Services::printInfoWithoutMutex();
    esp_rom_printf("available heap size: %lu bytes (min %lu bytes)", esp_get_free_heap_size(), esp_get_minimum_free_heap_size());
    // heap_caps_print_heap_info(MALLOC_CAP_DEFAULT);
    gpio_set_level(LED_ERROR, 1);
    __real_esp_panic_handler(info);
}
