CFLAGS +=  -I$(ISILSDKPATH)include  \
			-I$(EVENTCOREINCPATH) \
			-I$(MEDIASDKINCPATH) \
			-I$(ISILSDKINCPATH) \
			-I$(LIBEVENTINCPATH) 
           
	       


H264_MAIN_GET_FRAME_NAME = h264_main_get_frame
H264_MAIN_FILE_STORE_NAME =h264_main_file_store


	   
	   
#CFLAGS += -static	

	   

MAKE_DIR=$(PWD)


LIBPATH = -L$(ISILSDKLIBPATH) \
		  -L$(LIBEVENTLIBPATH) 
	  


DVRLIB = -l$(LINKISILMEDIASLIBNAME) \
         -lpthread \
         -levent 
         


SRCS :=$(wildcard *.c)
SRC_FILES:=$(notdir $(SRCS))
OBJ_FILES :=$(patsubst %c,%o,$(SRC_FILES))





 
all: $(OBJ_FILES)
	@echo -e "lib $(DVRDIR)"
	$(CC) $(CFLAGS) -o $(H264_MAIN_GET_FRAME_NAME) $(H264_MAIN_GET_FRAME_NAME).o $(LIBPATH) $(DVRLIB)
	$(CC) $(CFLAGS) -o $(H264_MAIN_FILE_STORE_NAME) $(H264_MAIN_FILE_STORE_NAME).o $(LIBPATH) $(DVRLIB)
	
    
#	$(AR) cr $(LIBNAME) $(OBJ_FILES)
#	@mv $(LIBNAME) $(TOPLPATH)
#-rm -f *.o
	
.PHONY:clean
clean:
	-rm -f *.o $(H264_MAIN_GET_FRAME_NAME) $(H264_MAIN_FILE_STORE_NAME)
	
	
	
