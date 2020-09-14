SRC_DIR := .
OBJ_DIR := _build
EXEC := cpp-playground
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

INC_DIRS += 
INCLUDE = $(addprefix -I,$(INC_DIRS))

LDFLAGS := 
CXXFLAGS := -MMD -std=c++17
CXX := c++

# Echo suspend
ifeq ("$(VERBOSE)","1")
NO_ECHO := 
else
NO_ECHO := @
endif

all: $(SRC_FILES) $(EXEC)

$(EXEC): $(OBJ_DIR) $(OBJ_FILES)
	@echo Linking: $(notdir $@)
	$(NO_ECHO) $(CXX) $(LDFLAGS) $(OBJ_FILES) -o $(OBJ_DIR)/$@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo Compiling: $(notdir $@)
	$(NO_ECHO) $(CXX) $(INCLUDE) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR):
	$(NO_ECHO) mkdir $@

run: $(EXEC)
	$(NO_ECHO) $(OBJ_DIR)/$(EXEC)

clean:
	rm -r $(OBJ_DIR)

print-%  : ; @echo $* = $($*)

.PHONY: all run clean print-%