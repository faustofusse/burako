# burako

## web assembly

```bash
brew install emscripten
```

guia: https://github.com/raysan6/raylib/wiki/Working-for-Web-(HTML5)

## update raylib

```bash
git clone https://github.com/raysan5/raylib raylib-temp
mv raylib-temp/src libs/raylib/src
rm -rf raylib-temp
```

## lsp shit

```bash
make clean
bear -- make PLATFORM=PLATFORM_DESKTOP
```
