CC = g++
CFLAGS ?= -g `pkg-config --cflags gtk+-3.0` -std=c++11
LDFLAGS = `pkg-config --libs gtk+-3.0` # -fsanitize=leak
CODE_STYLE_PATH = .clang-format

LIB = libgui3.a
SRCS = button.cc edit.cc GUI.cc image.cc list.cc scroll.cc text.cc window.cc
HEADERS = button.h context.h edit.h GUI.h image.h list.h mytypes.h scroll.h text.h window.h
OBJS = $(addprefix $(OUT_O_DIR)/,$(SRCS:.cc=.o))

OUT_O_DIR ?= build
EXE = myprog

MYPROG_SRCS = myprog.cc port_monitor.cc cube.cc
MYPROG_HDRS = port_monitor.h cube.h
MYPROG_OBJS = $(addprefix $(OUT_O_DIR)/,$(MYPROG_SRCS:.cc=.o))

.PHONY: all
all: $(EXE)

$(LIB): $(OBJS)
	ar -r -s $(LIB) $(OBJS)

$(OUT_O_DIR)/%.o: source/%.cc $(addprefix source/,$(HEADERS))
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXE): $(LIB) $(MYPROG_OBJS) $(addprefix source/,$(MYPROG_HDRS))
	$(CC) $(MYPROG_OBJS) $(LIB) -o $(EXE) $(LDFLAGS)

.PHONY: cif
cif:
	clang-format -i --style=file:$(CODE_STYLE_PATH) $(addprefix source/,$(MYPROG_SRCS)) $(addprefix source/,$(MYPROG_HDRS))

clean:
	rm -rf $(LIB) $(EXE) $(OUT_O_DIR)/*
