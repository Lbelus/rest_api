# rest_api
a basic cpp rest api

## Installation

### Create the and containers; 


CHECK THE script.sh FILE /!\

```bash
docker network create sqlRest
docker build -t img_llvm_mysql_crow .
docker run -it --network sqlRest -v .:/workspace --name cont_llvm_mysql_crow img_llvm_mysql_crow /bin/bash
docker pull mysql:latest
docker run --name mysqlserver --network sqlRest -e MYSQL_ROOT_PASSWORD=your_root_password -e MYSQL_DATABASE=test_rest_DB -e MYSQL_USER=dev_admin -e MYSQL_PASSWORD=dev_admin -v mysql_data_test_rest:/var/lib/mysql -p 3306:3306 -d mysql:8.0

```


## Documentation


MySql++:
- https://tangentsoft.com/mysqlpp/wiki?name=MySQL%2B%2B&p&nsm
