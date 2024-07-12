FROM ubuntu:20.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y build-essential gcc libsdl2-dev && rm -rf /var/lib/apt/lists/*
WORKDIR /usr/src/app
COPY . .
RUN gcc SDL_externalTransforms.c -o main -I/usr/include/SDL2 -lSDL2
CMD ["./main"]