#sinclude	./../../rules.make	

TOPLPATH = ./../lib
TOPHPATH = ./../include

CFLAGS +=   -I$(MEDIASDKINCPATH)  -I$(TOPHPATH) 
	   
	   
	   

MAKE_DIR=$(PWD)


LIBNAME = $(ISILFILESDKSLIBNAME)
AR_FLAGS = 


SRCS :=$(wildcard *.c)
SRC_FILES:=$(notdir $(SRCS))
OBJ_FILES :=$(patsubst %c,%o,$(SRC_FILES))

#OUT=$(OUTPUT_FILE)

#$(OUT): $(OBJ_FILES)
#	@echo -e "building: $(notdir $@) \n\t please wait ...\n"	
	

#%.o : %.c
#	$(CC) $(CFLAGS)  -c $^	


#	@echo -e "building: $(notdir $@) \n\t please wait ...\n"

#$(OBJS):$(OBJ_FILES)
#	@echo -e "building: $(notdir $@) \n\t please wait ...\n"
	
#$(LIBNAME): $(OBJS)
#	@echo -e "building: $(notdir $@) \n\t please wait ...\n"
#	-rm -f $(TOPLPATH)$(LIBNAME)
#	$(AR) $(AR_FLAGS) $(LIBNAME) $(OBJS)
#	$(RANLIB) $(LIBNAME)

%.o : %.c
	@echo -e "building: $(notdir $^) \n\t please wait ...\n"
	@echo -e "MEDIASDKINCPATH: $(MEDIASDKINCPATH) TOPHPATH : $(TOPHPATH)\n\t"
	$(CC) $(CFLAGS) -c $^ 

 
all: $(OBJ_FILES)
	$(AR) cr $(LIBNAME) $(OBJ_FILES)
	@mv $(LIBNAME) $(TOPLPATH)
#-rm -f *.o
	
.PHONY:clean
clean:
	-rm -f *.o
	
	
	
