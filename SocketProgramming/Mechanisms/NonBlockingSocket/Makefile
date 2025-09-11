SRC_FILE := $(wildcard ./*.c)
TARGET_FILE := $(patsubst %.c,%.out,$(SRC_FILE))

vpath %.c .
%.out : %.c
	gcc $^ -o $@ -pthread

all: $(TARGET_FILE)

clean:
		rm -rf ./*.out

print-%:
	@echo $($(subst print-,,$@))