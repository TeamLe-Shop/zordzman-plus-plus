#include "weaponList.hpp"

namespace client {
namespace weaponList {

using namespace client::weapon;

BaseWeapon *BlankWeapon = new BaseWeapon("", -1, -1, BOTH, BLANK);
Zord *zord = new Zord();

} // namespace weaponList
} // namespace client
