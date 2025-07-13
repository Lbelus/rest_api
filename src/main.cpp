#include <mysql++/mysql++.h>

int main() {
    mysqlpp::Connection conn("test_rest_DB", "mysqlserver", "dev_admin", "dev_admin");
    mysqlpp::Query query = conn.query("SELECT * FROM users");
    if (auto result = query.store()) {
        for (const auto& row : result) {
            std::cout << "Name: " << row["name"] << std::endl;
        }
    }
    return 0;
}
