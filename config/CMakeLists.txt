####
# Project configuration overrides:
#
# Overrides selected F Prime default configuration files (lib/fprime/default/config) with project-specific versions.
# See: lib/fprime/docs/user-manual/framework/configuring-fprime.md
####
register_fprime_config(
    CONFIGURATION_OVERRIDES
        "${CMAKE_CURRENT_LIST_DIR}/PlatformCfg.fpp"
        "${CMAKE_CURRENT_LIST_DIR}/TlmChanImplCfg.hpp"
    INTERFACE
)
