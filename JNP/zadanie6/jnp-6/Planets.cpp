#include "Planets.h"

Earth& earth() {
    static Earth planet;
    return planet;
}

Qonos& qonos() {
    static Qonos planet;
    return planet;
}

Bynaus& bynaus() {
    static Bynaus planet;
    return planet;
}