SUBDIRS=Timer Pipeline Thread  ThreadMsg DataBase Server LLog
bin_PROGRAMS = CaiJiQi
CaiJiQi_SOURCES = main.c
CaiJiQi_CPPFLAGS = -IPipeline -IThread -IThreadMsg -I Include -ITimer -ILLog -IListHead -IDataBase -IServer
CaiJiQi_LDADD=Pipeline/*.o ThreadMsg/*.o LLog/*.o  Timer/*.o DataBase/*.o -lsqlite3 Server/*.o Thread/*.o  -lpthread
