FROM ubuntu:20.04

RUN apt-get update && apt-get install file binutils make gcc gcc-multilib strace python3 -y

WORKDIR /app

CMD tail -f /dev/null