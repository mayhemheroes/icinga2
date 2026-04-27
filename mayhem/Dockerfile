FROM --platform=linux/amd64 ubuntu:22.04 as builder

RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential cmake clang libboost-all-dev gdb vim git make ccache libssl-dev bison flex default-libmysqlclient-dev libpq-dev libedit-dev monitoring-plugins

ENV CC=clang
ENV CXX=clang++
COPY . /repo
WORKDIR /repo/tools/mkembedconfig/build
RUN cmake ..
RUN make -j8

WORKDIR /repo/build
RUN cmake ..
RUN make -j8
RUN clang++ /repo/mayhem/fuzz_ToDateTimeValue.cpp /repo/build/third-party/socketpair/CMakeFiles/socketpair.dir/socketpair.c.o /repo/build/third-party/mmatch/CMakeFiles/mmatch.dir/mmatch.c.o /repo/build/third-party/execvpe/CMakeFiles/execvpe.dir/execvpe.c.o /repo/build/lib/config/CMakeFiles/config.dir/*.o /repo/build/lib/base/CMakeFiles/base.dir/*.o -fsanitize=fuzzer,address -I /repo/lib/base/ -I /repo/lib/ -I /repo/build/ -L /repo/build/Bin/Release/ -o /fuzz -lssl -lcrypto -lboost_regex -lboost_filesystem -lboost_thread -lboost_iostreams -lboost_coroutine -o /fuzz

FROM ubuntu:22.04 as package
RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y libboost-all-dev
COPY --from=builder /repo/tools/mkembedconfig/build/mkembedconfig /
COPY --from=builder /fuzz /fuzz_ToDateTimeValue
