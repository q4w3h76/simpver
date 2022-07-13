# HTTP server
Compilation and usage

Only Linux

```
git clone https://github.com/q4w3h76/simpver.git
cd simpver
mkdir -p build/{debug,object}
make
sudo ./build/debug/simpver start /path/to/html
```
Then open the browser and enter 127.0.0.1:8888/ in the search bar

For stop simpver
```
sudo ./build/debug/simpver stop
```
