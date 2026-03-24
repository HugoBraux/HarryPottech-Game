#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/3ds_rules

#---------------------------------------------------------------------------------
BUILD       :=  build
SOURCES     :=  source
DATA        :=  data
INCLUDES    :=  include
GRAPHICS    :=  gfx

ARCH        :=  -marm -mcpu=mpcore -mfloat-abi=hard -mfpu=vfpv2 -mtp=soft

CFLAGS      :=  -g -Wall -O2 -mword-relocations \
            -fomit-frame-pointer -ffunction-sections \
            $(ARCH)

CFLAGS      +=  $(INCLUDE) -D__3DS__

CXXFLAGS    := $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++11

ASFLAGS     :=  -g $(ARCH)
LDFLAGS     =   -specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

# Ajout des librairies graphiques ici :
LIBS        := -lcitro2d -lcitro3d -lctru -lm

LIBDIRS     := $(PORTLIBS) $(CTRULIB)

#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT   :=  $(CURDIR)/HarryPottech
export TOPDIR   :=  $(CURDIR)

export VPATH    :=  $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
                    $(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
                    $(foreach dir,$(GRAPHICS),$(CURDIR)/$(dir))

export DEPSDIR  :=  $(CURDIR)/$(BUILD)

CFILES      :=  $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES    :=  $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES      :=  $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
export T3SFILES :=  $(foreach dir,$(GRAPHICS),$(notdir $(wildcard $(dir)/*.t3s)))

ifeq ($(strip $(CPPFILES)),)
    export LD   :=  $(CC)
else
    export LD   :=  $(CXX)
endif

export OFILES_BIN   :=  $(addsuffix .o,$(BINFILES))
export OFILES_SRC   :=  $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export OFILES   :=  $(OFILES_BIN) $(OFILES_SRC)
export HFILES   :=  $(addsuffix .h,$(subst .,_,$(BINFILES)))

export INCLUDE  :=  $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
                    $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
                    -I$(CURDIR)/$(BUILD)

export LIBPATHS :=  $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

export APP_TITLE        := HarryPottech
export APP_AUTHOR       := BDE Polytech
export APP_DESCRIPTION  := Jeu clicker BDE
export APP_ICON         := $(TOPDIR)/icon.png

#---------------------------------------------------------------------------------
# romfs handling
#---------------------------------------------------------------------------------
export ROMFS_DIR := $(CURDIR)/romfs

.PHONY: $(BUILD) clean all

all: $(BUILD)

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@[ -d $(ROMFS_DIR) ] || mkdir -p $(ROMFS_DIR)
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(OUTPUT).3dsx $(OUTPUT).elf $(OUTPUT).smdh $(ROMFS_DIR)

#---------------------------------------------------------------------------------
else

DEPENDS :=  $(OFILES:.o=.d)

ifeq ($(strip $(ROMFS_DIR)),)
    ROMFS_ARG :=
else
    ROMFS_ARG := --romfs=$(ROMFS_DIR)
endif

export _3DSXFLAGS += $(ROMFS_ARG) --smdh=$(OUTPUT).smdh

$(OUTPUT).3dsx  :   $(OUTPUT).elf $(OUTPUT).smdh

$(OUTPUT).elf   :   $(OFILES)

#---------------------------------------------------------------------------------
# Regle explicite pour compiler le .smdh avec l'icone
#---------------------------------------------------------------------------------
$(OUTPUT).smdh	:	$(APP_ICON)
	@echo Creation de $(notdir $@)...
	@smdhtool --create "$(APP_TITLE)" "$(APP_DESCRIPTION)" "$(APP_AUTHOR)" $(APP_ICON) $@

#---------------------------------------------------------------------------------
# Regles pour compiler les images .t3s en .t3x
#---------------------------------------------------------------------------------
$(ROMFS_DIR)/gfx/%.t3x  :   %.t3s
	@echo $(notdir $<)
	@mkdir -p $(ROMFS_DIR)/gfx
	@tex3ds -i $< -H $(BUILD)/$*.h -d $(BUILD)/$*.d -o $@

%.o : %.t3s
	@echo $(notdir $<)
	@mkdir -p $(ROMFS_DIR)/gfx
	@tex3ds -i $< -H $*.h -d $*.d -o $(ROMFS_DIR)/gfx/$*.t3x
	@touch $@

$(OFILES_SRC) : $(T3SFILES:.t3s=.o)

%.bin.o %_bin.h :   %.bin
	@echo $(notdir $<)
	@$(bin2o)

-include $(DEPENDS)
-include $(T3SFILES:.t3s=.d)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------