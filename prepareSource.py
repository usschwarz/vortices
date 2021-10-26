import os
import fnmatch
import numpy as np

BASEDIR = os.path.dirname(os.path.abspath(__file__))

if __name__ == '__main__':

    ######################################################################################
    # Edit the local makefile:
        
    localFiles = os.listdir(BASEDIR)

    pattern = 'makefile'
    for file in localFiles:
        if (fnmatch.fnmatch(file.lower(), pattern)):
            filename = file

    with open(os.path.join(BASEDIR, filename), 'r') as file:
        filedata = file.read()

    filedata = filedata.replace('CFLAGS += -O3', 'CFLAGS += -O3 -std=c99')
    
    filedata = filedata.replace('LIBS    += -lmboard_p  -lm', 'LIBS    += -lmboard_p  -lm -pthread')

    with open(os.path.join(BASEDIR, filename), 'w') as file:
        file.write(filedata)
    
    
    ######################################################################################
    # Edit the local main.c file
    
    filename = 'main.c'
    tmpFilename = 'main_TMP.c'
    pattern = "/* Print out iteration number */"
    nSkips = 15
    
    # find line number where the above specified pattern is first detected
    # and store it in startline
    startline = -1
    with open(os.path.join(BASEDIR, filename)) as file:    
        for num, line in enumerate(file, 1):
            if pattern in line:
                startline = num
                break
    
    if (startline > 0):
        # re-read in all lines and store them in filedata
        with open(os.path.join(BASEDIR, filename)) as file:
            filedata = file.readlines()  
        
        # write a tmp-file to a file, skipping nSkip lines after the found pattern
        f = open(os.path.join(BASEDIR, tmpFilename), 'w')
        for i in range(len(filedata)):
            if i not in np.arange(startline - 1, startline + nSkips, 1):
                f.write(filedata[i])
        f.close()
    
        # clean up
        os.remove(os.path.join(BASEDIR, filename))
        os.rename(os.path.join(BASEDIR, tmpFilename), os.path.join(BASEDIR, filename))
    else:
        print "No pattern match. No changes applied."
    

    
