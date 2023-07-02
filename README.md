# jluna + Docker

After cloning this repo, 
```
docker compose build
docker compose up
```

`docker exec -it <container name> bash` to get into the container

Build:
```
cd /root/hello
mkdir build
cd build
cmake ..
make
```

Run the demo executable
```
cd /root/hello/build
./hello
```

the expected output is something like
```
hello from cpp
[JULIA][LOG] initialization successful (1 thread(s)).
hello from julia
3
```
