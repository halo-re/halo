INPUT_DIR = halo-patched
INPUT_XBE = $(INPUT_DIR)/cachebeta.xbe

OUTPUT_DIR = $(INPUT_DIR)
OUTPUT_XBE = $(OUTPUT_DIR)/default.xbe
OUTPUT_ISO = halo-patched.iso

REIMPLEMENTATION_EXE = src/main.exe

$(OUTPUT_ISO): $(OUTPUT_XBE)
	extract-xiso -c $(shell dirname $(OUTPUT_XBE))

$(REIMPLEMENTATION_EXE): .FORCE
	$(MAKE) -C src

$(OUTPUT_XBE): $(INPUT_XBE) $(REIMPLEMENTATION_EXE)
	./patch.py $(INPUT_XBE) $(REIMPLEMENTATION_EXE) $(OUTPUT_XBE)

.PHONY: .FORCE
