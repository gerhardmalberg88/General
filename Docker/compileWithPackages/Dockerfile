FROM ubuntu:latest
RUN apt-get -y update
RUN apt-get -y install gcc
COPY ProgramFolder /ProgramFolder
WORKDIR /ProgramFolder/
RUN gcc -o HelloWorld helloworld.c
CMD ["./HelloWorld"]i
