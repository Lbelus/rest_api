#include <rest_api.hpp>

int main(void)
{
    crow::simpleApp app;

    app.port(18080).multithreaded().run();

    return EXIT_SUCCESS;
}
