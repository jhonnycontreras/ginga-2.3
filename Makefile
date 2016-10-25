#
# OpenWrt Makefile for ginga
# Copyright (C) 2006-2010 OpenWrt.org
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
# blogic@openwrt.org
#
# Fundación CENDITEL
# jcanelones@cenditel.gob.ve

include $(TOPDIR)/rules.mk

PKG_NAME:=ginga
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/uclibc++.mk
include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
        SECTION:=Cenditel-Aplicaciones
        CATEGORY:=Cenditel
        SUBMENU:=Aplicaciones
        TITLE:=ginga.ar 2.3
        DEPENDS:=+uclibcxx +make +cmake +cairo +pango +boost
endef


#define Build/Prepare
#	mkdir -p $(PKG_BUILD_DIR)/install/build/UNIX
#	$(CP) -r ./src/* $(PKG_BUILD_DIR)/
#endef

#define Build/Compile
#	cd $(PKG_BUILD_DIR)/install/build/UNIX && \
#		python $(PKG_BUILD_DIR)/build/build.py --tool ginga 
#endef


define Package/ginga/install
	$(INSTALL_DIR) $(1)/bin
        $(INSTALL_BIN) $(PKG_BUILD_DIR)/rootfs/bin/ginga $(1)/bin/
endef


$(eval $(call BuildPackage,$(PKG_NAME)))


