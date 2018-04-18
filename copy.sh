! #/bin/bash

cp main.c project
cp makefile project
cp header.h project
cp scheduler.c project
cp while1 project
cp run.sh project
scp -r '/media/andywang/1/大三下/操作系统/实践/project/ProcessScheduler/project' s15G202@co004.fit.must.edu.mo:~

exit

echo "19970605"
sleep 1

scp -r '/media/andywang/1/大三下/操作系统/实践/project/ProcessScheduler/makefile' s15G202@co004.fit.must.edu.mo:~/project

scp -r '/media/andywang/1/大三下/操作系统/实践/project/ProcessScheduler/header.h' s15G202@co004.fit.must.edu.mo:~/project

scp -r '/media/andywang/1/大三下/操作系统/实践/project/ProcessScheduler/scheduler.c' s15G202@co004.fit.must.edu.mo:~/project

scp -r '/media/andywang/1/大三下/操作系统/实践/project/ProcessScheduler/while1' s15G202@co004.fit.must.edu.mo:~/project

