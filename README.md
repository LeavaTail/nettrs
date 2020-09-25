# nettrs
ネットワーク対戦型テトリスを作りたい

* [ ] 一人用のテトリスから

## Usage
### Build with Host
1. Install requirement package
2. Execute python script
```shell
$ python3 Nettrs.py
```

### Build with Docker
1. Build Docker Image
```shell
$ cd $<PROJECT_TOPDIR>
$ docker build -t nettrs .
```
2. Execute python script in Docker Container
```shell
$ docker run --rm -u nettrs -v $PWD:/work:ro -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=unix$DISPLAY --name nettrs -it nettrs python3 Nettrs.py
```

## Requirement
* [numpy](https://numpy.org/)
* [pygame](https://www.pygame.org/news)
