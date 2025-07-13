#!/bin/bash

rest_api_build_dev()
{
    docker network create sqlRest
    docker build -t img_llvm_mysql_crow .
    docker pull mysql:latest
}

rest_api_run_dev()
{
    sudo docker run --name mysqlserver --network sqlRest -e MYSQL_ROOT_PASSWORD=your_root_password -e MYSQL_DATABASE=test_rest_DB -e MYSQL_USER=dev_admin -e MYSQL_PASSWORD=dev_admin -v mysql_data_test_rest:/var/lib/mysql -p 3306:3306 -d mysql:8.0
    docker run -it --network sqlRest -v .:/workspace --name cont_llvm_mysql_crow img_llvm_mysql_crow /bin/bash
}

rest_api_start_dev()
{
    docker start mysqlserver
    docker start -ai cont_llvm_mysql_crow
}

rest_api_stop_dev()
{
    docker stop mysqlserver
    docker stop cont_llvm_mysql_crow
}

rest_api_rm_dev()
{
    docker rm mysqlserver
    docker rm cont_llvm_mysql_crow
}

rest_api_get_ip()
{
    docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' mysqlserver
}

rest_api_init_db()
{
    docker exec -i mysqlserver mysql -u dev_admin -pdev_admin test_rest_DB <<EOF
    CREATE TABLE users (
        id INT AUTO_INCREMENT PRIMARY KEY,
        name VARCHAR(100) NOT NULL
    );

    INSERT INTO users (name) VALUES ('Alice'), ('Bob');
    EOF
}


re()
{
    cd build/
    rm -r *
    cmake ..
    make
    cd ..
    ./build/rest_api
}
