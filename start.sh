set -e

docker build . -t strace_run
docker run -v `pwd`:/app -it strace_run bash