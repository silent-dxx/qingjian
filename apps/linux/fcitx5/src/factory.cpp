//! 青简 addon 工厂。
#include "qingjian.h"
namespace fcitx {
class QingjianFactory final : public AddonFactory {
public:
    AddonInstance *create(AddonManager *manager) override { return new QingjianEngine(manager); }
};
}
// FCITX_ADDON_FACTORY_V2 从 Fcitx5 5.1.12 起才有；更早的版本（如 Ubuntu 24.04 的 5.1.7）只认另一个符号名。
#ifdef FCITX_ADDON_FACTORY_V2
FCITX_ADDON_FACTORY_V2(qingjian, fcitx::QingjianFactory)
#else
FCITX_ADDON_FACTORY(fcitx::QingjianFactory)
#endif
