
## GLAD Version
https://glad.dav1d.de/#language=c&specification=gl&api=gl%3D3.3&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&profile=core&loader=on

Сборка 

```bash
g++ src/main.cpp src/GLWrap.cpp glad/src/glad.c \
    -Iglad/include -Iinc/ -Iglm/ \
    -o out -lglfw -ldl -lGL
```
