#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define LOG_TAG "RioRP_Hook"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Função para encontrar o endereço base de uma biblioteca carregada no jogo
uintptr_t get_module_base(constenciar char *module_name) {
    FILE *fp = fopen("/proc/self/maps", "r");
    if (!fp) return 0;

    char line[512];
    uintptr_t base_addr = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, module_name) != NULL) {
            char *end_ptr;
            base_addr = (uintptr_t)strtoull(line, &end_ptr, 16);
            break;
        }
    }
    fclose(fp);
    return base_addr;
}

// Função para aplicar alterações de código na memória com segurança
void patch_memory(void *destination, void *source, size_t size) {
    long page_size = sysconf(_SC_PAGESIZE);
    void *page_start = (void *)((uintptr_t)destination & ~(page_size - 1));

    if (mprotect(page_start, page_size, PROT_READ | PROT_WRITE | PROT_EXEC) == 0) {
        memcpy(destination, source, size);
        LOGI("Patch aplicado com sucesso no endereco: %p", destination);
    } else {
        LOGE("Erro ao alterar permissao de memoria com mprotect.");
    }
}

__attribute__((constructor))
void init_injection() {
    LOGI("=== INJETOR INTERNO CARREGADO NO RIO RP ===");

    // Exemplo de uso ao carregar:
    // Aguarda o modulo principal carregar e pega o endereco base
    uintptr_t base = 0;
    while (base == 0) {
        base = get_module_base("libsamp.so"); // Substitua pelo nome correto da lib do jogo se necessário
        usleep(100000); // Tenta a cada 100ms até o módulo subir
    }
    
    LOGI("Endereco base encontrado: %p", (void *)base);
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    return JNI_VERSION_1_6;
}
