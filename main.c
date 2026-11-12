#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>

#define LOG_TAG "InjetorInterno"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Função executada automaticamente assim que o sistema carrega o .so na memória
__attribute__((constructor))
void entry_point() {
    LOGI("=== BIBLIOTECA INJETADA COM SUCESSO ===");
    
    // Exemplo de leitura do mapa de memória do próprio processo para encontrar o endereço base
    FILE *fp = fopen("/proc/self/maps", "r");
    if (fp != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), fp)) {
            // Procura pela própria biblioteca nos mapas de memória
            if (strstr(line, "libcustom.so") != NULL) {
                LOGI("Mapa encontrado: %s", line);
                break;
            }
        }
        fclose(fp);
    } else {
        LOGE("Erro ao abrir /proc/self/maps");
    }
}

// Padrão obrigatório para o Runtime ART do Android reconhecer o .so via JNI
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("JNI_OnLoad chamado com sucesso.");
    return JNI_VERSION_1_6;
}
