#include <Geode/Geode.hpp>
#include <dlfcn.h>
#include <filesystem>
#include <system_error>

using namespace geode::prelude;
namespace fs = std::filesystem;

namespace {
    // Recorre el directorio de recursos del mod y registra todo lo que encuentra,
    // para poder distinguir "no se empaquetó" de "se empaquetó pero en otra ruta".
    void logResourceTree(fs::path const& dir, int depth = 0) {
        std::error_code ec;
        if (!fs::exists(dir, ec) || ec) {
            log::warn("[DiscordSDKTest] El directorio no existe o no es accesible: {}", dir.string());
            return;
        }
        for (auto const& entry : fs::directory_iterator(dir, ec)) {
            std::string indent(static_cast<size_t>(depth) * 2, ' ');
            log::info("[DiscordSDKTest] {}{}{}", indent, entry.path().filename().string(),
                      entry.is_directory() ? "/" : "");
            if (entry.is_directory()) {
                logResourceTree(entry.path(), depth + 1);
            }
        }
        if (ec) {
            log::warn("[DiscordSDKTest] Error al iterar {}: {}", dir.string(), ec.message());
        }
    }

    // Test 1: intenta cargar libdiscord_partner_sdk.so vía dlopen explícito.
    // NO llama ninguna función de discordpp/cdiscord. Solo verifica que el
    // binario ARM64 pueda ser localizado y cargado por el linker de Android
    // dentro del proceso de Geometry Dash.
    void runDiscordSdkSmokeTest() {
        auto resourcesDir = Mod::get()->getResourcesDir();
        log::info("[DiscordSDKTest] Resources dir: {}", resourcesDir.string());
        logResourceTree(resourcesDir);

        auto soPath = resourcesDir / "libdiscord_partner_sdk.so";

        std::error_code ec;
        if (!fs::exists(soPath, ec)) {
            log::error(
                "[DiscordSDKTest] EMPAQUETADO: NO se encontró el archivo en la ruta esperada: {}",
                soPath.string()
            );
            log::error(
                "[DiscordSDKTest] Revisa el árbol de recursos logueado arriba para ver dónde "
                "terminó realmente el archivo dentro del .geode."
            );
            return;
        }
        log::info("[DiscordSDKTest] EMPAQUETADO: OK. Archivo encontrado en {}", soPath.string());

        dlerror(); // limpiar cualquier error previo pendiente
        void* handle = dlopen(soPath.string().c_str(), RTLD_NOW);

        if (!handle) {
            const char* err = dlerror();
            log::error(
                "[DiscordSDKTest] CARGA: dlopen() FALLÓ. Mensaje de dlerror(): {}",
                err ? err : "(dlerror() no devolvió mensaje)"
            );
            return;
        }

        log::info("[DiscordSDKTest] CARGA: dlopen() exitoso. La biblioteca quedó cargada en el proceso.");
        log::info(
            "[DiscordSDKTest] Test 1 completado con EXITO. No se llamó ninguna función de Discord."
        );

        // Dejamos el handle abierto intencionalmente: no llamamos dlclose() para
        // evitar disparar cualquier rutina de descarga/limpieza del SDK en este
        // test puramente de smoke-loading.
    }
}

$on_mod(Loaded) {
    log::info("Geode RPC Test loaded successfully!");
    runDiscordSdkSmokeTest();
}
