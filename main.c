#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define LOG_TAG "InjetorMod"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Função para modificar o valor em um endereço de memória específico
void patch_memory(void *addr, void *new_value, size_t size) {
    // Pega o tamanho da página do sistema
    long page_size = sysconf(_SC_PAGESIZE);
    
    // Alinha o endereço para o início da página de memória
    void *page_start = (void *)((uintptr_t)addr & ~(page_size - 1));

    // Altera temporariamente a permissão da página para Leitura, Escrita e Execução (RWE)
    if (mprotect(page_start, page_size, PROT_READ | PROT_WRITE | PROT_EXEC) == 0) {
        // Copia o novo código/valor para o endereço alvo
        memcpy(addr, new_value, size);
        LOGI("Memoria modificada com sucesso no endereco: %p", addr);
    } else {
        LOGE("Falha ao alterar permissao de memoria com mprotect");
    }
}

__attribute__((constructor))
void init_hook() {
    LOGI("=== INJETOR CARREGADO NO JOGO ===");

    // Exemplo conceitual:
    // 1. Você descobre o endereço base do binário do jogo em /proc/self/maps
    // 2. Soma o endereço base com o offset da função ou variável do jogo
    // 3. Aplica o patch:
    
    // uintptr_t base_addr = get_module_base("libil2cpp.so"); // Exemplo comum em jogos Unity
    // void *target_address = (void *)(base_addr + 0x123456);
    // int novo_valor = 999999;
    // patch_memory(target_address, &novo_valor, sizeof(novo_valor));
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    return JNI_VERSION_1_6;
}
