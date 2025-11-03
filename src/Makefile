CXX = g++
CPPFLAGS = -O3 -DUSE_VOLK
LDFLAGS = -lboost_program_options
OBJDIR := ../build

common_hdrs = Adc.h
OBJS = $(OBJDIR)/main.o $(OBJDIR)/Adc.o

all: lab1

lab1: $(common_hdrs) $(OBJDIR) $(OBJS)
	$(CXX) -o AIBL $(OBJS) $(LDFLAGS)

$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: %.cpp $(common_hdrs)
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf lab1 $(OBJDIR)
