# rest_api
a basic cpp rest api 


## Installation

### Create the and containers; 

```bash
docker network create sqlRest
docker build -t img_llvm_sqlite_crow .
docker run -it --network sqlRest -v .:/workspace --name cont_llvm_sqlite_crow img_llvm_sqlite_crow /bin/bash
```



