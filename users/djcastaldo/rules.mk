$(info >>> LOADING $(USER_PATH)/rules.mk <<<)
LLOCK_PATH := $(USER_PATH)/features/layer_lock.c
PROCESS_RECORD_PATH := $(USER_PATH)/process_record_userspace.c
SECRETS_PATH := $(USER_PATH)/secrets.c
SRC += $(LLOCK_PATH)
SRC += $(PROCESS_RECORD_PATH)
ifneq ("$(wildcard $(SECRETS_PATH))","")
    SRC += $(SECRETS_PATH)
endif
ifeq ($(strip $(NO_SECRETS)), yes)
    OPT_DEFS += -DNO_SECRETS
endif
