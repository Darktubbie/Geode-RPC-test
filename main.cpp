#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace {
    // Prueba mínima absoluta: solo confirma que la llamada ocurre.
    // Sin filesystem, sin dlopen, sin Discord, sin nada más.
    void runDiscordSdkSmokeTest() {
        log::info("[DiscordSDKTest] FUNCTION ENTERED");
    }
}

$on_mod(Loaded) {
    log::info("Geode RPC Test loaded successfully!");
    runDiscordSdkSmokeTest();
}
