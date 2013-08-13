ifdef SIMULATION
  SIM_SRC_DIR = $(OPENMPTL_TOP)/sim

  SRCS  += $(wildcard $(addsuffix /*.cpp, $(SIM_SRC_DIR)))

  vpath %.cpp $(SIM_SRC_DIR)
endif
