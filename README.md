# rest_api
a basic cpp rest api


## PROJECT


### **1. Multi-Threaded Connection Management**

1.1 **MySQL Connection Pool**

* 1.1.1 Design thread-safe connection pool class -> ok
* 1.1.2 Implement connection acquisition and release logic -> ok
* 1.1.3 Implement idle connection timeout and auto-reconnect -> ok
* 1.1.4 Test under concurrent request load

1.2 **Redis Connection Pool**

* 1.2.1 Implement thread-safe pool for Redis connections
* 1.2.2 Integrate with async Redis client (if needed)
* 1.2.3 Benchmark performance in multi-threaded mode

---

### **2. Input Validation Layer**

2.1 **Request Parsing & Sanitization**

* 2.1.1 Validate JSON body schema
* 2.1.2 Validate query parameters and types
* 2.1.3 Escape unsafe inputs for SQL & Redis commands

2.2 **Validation Framework Integration**

* 2.2.1 Implement reusable validation utilities
* 2.2.2 Add error handling and return standardized HTTP error codes

---

### **3. Secure Connection and Validation**

3.1 **HTTPS Configuration**

* 3.1.1 Enable TLS/SSL in Crow server
* 3.1.2 Load and manage SSL certificates

3.2 **Connection Validation**

* 3.2.1 Enforce HTTPS-only connections
* 3.2.2 Validate certificate handshake on incoming connections

3.3 **Optional Security Hardening**

* 3.3.1 Implement rate limiting
* 3.3.2 Enable CORS and security headers 

---

### **4. Testing & Quality Assurance**

4.1 **Unit Tests (Google Test)**

* 4.1.1 Test connection pool logic under concurrent load
* 4.1.2 Test input validation logic with edge cases
* 4.1.3 Test HTTPS connection establishment and failures

4.2 **Integration Tests**

* 4.2.1 Test API endpoints with mocked MySQL -> ok
* 4.2.2 Test API endpoints with mocked Redis
* 4.2.3 Test multi-threaded performance and resource usage

---

### **5. Documentation & Developer Support**

5.1 **Developer Documentation**

* 5.1.1 Document API endpoints with examples
* 5.1.2 Document connection pool usage and limits
* 5.1.3 Document validation and security considerations

5.2 **Setup & Deployment Guide**

* 5.2.1 Document SSL setup and certificate management
* 5.2.2 Provide Docker or CMake build instructions



## Installation

## Documentation


### MySql++:

- https://tangentsoft.com/mysqlpp/doc/html/userman/
- https://tangentsoft.com/mysqlpp/wiki?name=MySQL%2B%2B&p&nsm

### MySql++ - connection and multi-threading:
- https://tangentsoft.com/mysqlpp/doc/html/userman/threads.html
- https://github.com/rpetrich/mysqlpp/blob/master/examples/cpool.cpp

### https/server crow documentation.

- https://crowcpp.org/master/guides/routes/

### redis-plus-plus documentation

- https://github.com/sewenew/redis-plus-plus/blob/master/src/sw/redis%2B%2B/redis.h

### googleTest documentation
- https://google.github.io/googletest/
- https://github.com/google/googletest/tree/main/googletest/samples


#### Hot to use with with curl and crow  

let's make some post and get request to our server via the load balancer 

(Don't forget to use the appropriate port)

If you have issues, try to add ``-X POST `` or ``-X GET`` in front of ``curl``, but it should ne be necessary.

### REDIS USAGE

| Category | Redis cmd    | Arguments       | Examples           |
| -------- | -------- | --------------- | --------------- |
| POST     | set      | set             | curl http://localhost:80/set \ <br> -H "Content-Type: application/json" \ <br> -d '{"key":"mykey", "value":"myvalue"}'|
| POST     | lpush    | lpush           | curl http://localhost:80/lpush \ <br> -H "Content-Type: application/json" \ <br> -d '{"key":"mykey", "value":"myvalue"}'|
| POST     | rpush    | rpush           | curl http://localhost:80/rpush \ <br> -H "Content-Type: application/json" \ <br> -d '{"key":"mykey", "value":"myvalue"}'|
| POST     | hmset    | hmset           | curl http://localhost:80/rpush \ <br> -H "Content-Type: application/json" \ <br> -d '{"key":"myhash", "fields_values":{"field1":"value1", "field2":"value2"}}'|
| POST     | hmget    | hmget           | curl http://localhost:80/hmget \ <br> -H "Content-Type: application/json" \ <br> -d '{"key":"myhash", "fields":["field1", "field2"]}'|
| GET      | key      | key/<_string_>  | curl http://localhost:80/key/key_value|
| GET      | get      | key/<_string_>  | curl http://localhost:80/get/key_value|
| GET      | lpop     | lpop/<_string_> | curl http://localhost:80/lpop/key_value|
| GET      | rpop     | rpop/<_string_> | curl http://localhost:80/rpop/key_value|
| GET      | llen     | llen/<_string_> | curl http://localhost:80/llen/key_value|
| GET      | ping     | ping            | curl http://localhost:80/ping|
| GET      | echo     | echo/<_string_> | curl http://localhost:80/echo/msg|
| GET      | flushall | flushall        | curl http://localhost:80/flushall|
| GET      | info     | info            | curl http://localhost:80/info|

Client --(HTTPS)--> HTTP SERVER - API --(local access) --> DB

Complementary information:
``to test with self signed cert use -k flag``
post request :

```sh
curl http://localhost:80/post \
    -H "Content-Type: application/json" \
    -d '{"key":"mykey", "value":"myvalue"}'
```

get request:
```sh
curl http://localhost:80/get/mykey
```


