FROM ubuntu:latest

RUN apt-get update && apt-get install -y cmake clang git

WORKDIR /usr/src/app

COPY . .

RUN cmake -DENABLE_COVERAGE=OFF -DCMAKE_BUILD_TYPE=Release .

RUN make -j minesweeper

ENTRYPOINT ["./minesweeper", "-a"]
