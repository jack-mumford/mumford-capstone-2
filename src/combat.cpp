#include "combat.h"

int nearest_enemy(const Enemy enemies[], int count, Vector3 origin, float range) {
    int   best   = -1;
    float best_d = range * range;
    for (int i = 0; i < count; i++) {
        if (!enemies[i].is_alive) continue;
        float dx = enemies[i].position.x - origin.x;
        float dz = enemies[i].position.z - origin.z;
        float d2 = dx*dx + dz*dz;
        if (d2 < best_d) { best_d = d2; best = i; }
    }
    return best;
}
