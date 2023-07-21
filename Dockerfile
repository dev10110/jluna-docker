FROM julia 

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends git cmake build-essential g++ ca-certificates vim



WORKDIR /root
RUN git clone https://github.com/clemapfel/jluna.git
WORKDIR /root/jluna/build
RUN cmake .. 
RUN make install -j

WORKDIR /root



