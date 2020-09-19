# nettrs
ネットワーク対戦型テトリスを作りたい

## Usage
### run with Host
1. buildディレクトリ作る
2. cd ./build
3. cmake ../
4. make

### run with Docker
1. Build Docker Image
```shell
$ cd $<PROJECT_TOPDIR>
$ docker build -t nettrs .
```
2. Execute python script in Docker Container
```shell
$ docker run --rm -u nettrs -v $PWD:/work:ro -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=unix$DISPLAY --name nettrs -it nettrs python3 Nettrs.py
```

### run with Docker-Compose (For debug)
1. Build Docker Image to use Docker-Compose
```shell
$ cd $<PROJECT_TOPDIR>
$ docker-compose up -d
```
2. Execute python script in Docker Container
```shell
$ docker-compose exec client1 python3 Nettrs.py
```
:beginner: Docker-compose create 3 containers. You can switch `client1` to `server`, `client2`

関数の中身何も実装シてないからまだ動かないヨ！！
