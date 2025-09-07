#ifndef MYSQL_CONN_POOL
#define MYSQL_CONN_POOL

#include <mysql++/mysql++.h>
#include <thread>
#include <functional>
#include <mutex>
#include <queue>
#include <vector>
#include <mysql_utils.hpp>

unsigned int GetThreadCount(unsigned int divBy);

// ConnectionPool has three methods that you need to override in a subclass to make it concrete: 
// - create(), 
// - destroy()
// - max_idle_time(). 

// These overrides let the base class delegate operations it can’t successfully do itself to its subclass.

class SimpleConnectionPool : public mysqlpp::ConnectionPool
{

public:
	// The object's only constructor
	explicit SimpleConnectionPool(const mysql_connection_t& conn_id)
        : conns_in_use_(0),
	      db_(conn_id.db),
	      server_(conn_id.server),
	      user_(conn_id.user),
	      password_(conn_id.password),
          port_(conn_id.port)
	{}
	// The destructor.  We _must_ call ConnectionPool::clear() here,
	// because our superclass can't do it for us.
	~SimpleConnectionPool()
	{
		clear();
	}
    
    mysqlpp::Connection* grab() override {
        ++conns_in_use_;
        return mysqlpp::ConnectionPool::grab();
    }
    void release(const mysqlpp::Connection* pc) override {
        mysqlpp::ConnectionPool::release(pc);
        --conns_in_use_;
    }
protected:
    // Superclass overrides
    mysqlpp::Connection* create() override
    {
        // Create connection using the parameters we were passed upon
        // creation.  This could be something much more complex, but for
        // the purposes of the example, this suffices.
        std::cout.put('C'); std::cout.flush(); // indicate connection creation
        return new mysqlpp::Connection(
                db_.empty() ? 0 : db_.c_str(),
                server_.empty() ? 0 : server_.c_str(),
                user_.empty() ? 0 : user_.c_str(),
                password_.empty() ? "" : password_.c_str(),
                port_);
    }

    void destroy(mysqlpp::Connection* cp) override
    {
        // Our superclass can't know how we created the Connection, so
        // it delegates destruction to us, to be safe.
        std::cout.put('D'); std::cout.flush(); // indicate connection destruction
        delete cp;
    }

    unsigned int max_idle_time() override
    {
        // Set our idle time at an example-friendly 3 seconds.  A real
        // pool would return some fraction of the server's connection
        // idle timeout instead.
        return 3;
    }
private:

    unsigned int conns_in_use_;
    std::string db_;
    std::string server_;
    std::string user_;
    std::string password_;
    unsigned int port_ = 0;
// static thread_return_t CALLBACK_SPECIFIER

//     worker_thread(thread_arg_t running_flag)
// {
//     // Ask the underlying C API to allocate any per-thread resources it
//     // needs, in case it hasn't happened already.  In this particular
//     // program, it's almost guaranteed that the safe_grab() call below
//     // will create a new connection the first time through, and thus
//     // allocate these resources implicitly, but there's a nonzero chance
//     // that this won't happen.  Anyway, this is an example program,
//     // meant to show good style, so we take the high road and ensure the
//     // resources are allocated before we do any queries.
//     mysqlpp::Connection::thread_start();
//     cout.put('S'); cout.flush(); // indicate thread started

//     // Pull data from the sample table a bunch of times, releasing the
//     // connection we use each time.
//     for (size_t i = 0; i < 6; ++i) {
//         // Go get a free connection from the pool, or create a new one
//         // if there are no free conns yet.  Uses safe_grab() to get a
//         // connection from the pool that will be automatically returned
//         // to the pool when this loop iteration finishes.
//         mysqlpp::ScopedConnection cp(*poolptr, true);
//         if (!cp) {
//             cerr << "Failed to get a connection from the pool!" << endl;
//             break;
//         }

//         // Pull a copy of the sample stock table and print a dot for
//         // each row in the result set.
//         mysqlpp::Query query(cp->query("select * from stock"));
//         mysqlpp::StoreQueryResult res = query.store();
//         for (size_t j = 0; j < res.num_rows(); ++j) {
//             cout.put('.');
//         }

//         // Delay 1-4 seconds before doing it again.  Because this can
//         // delay longer than the idle timeout, we'll occasionally force
//         // the creation of a new connection on the next loop.
//         sleep(rand() % 4 + 1);  
//     }

//     // Tell main() that this thread is no longer running
//     *reinterpret_cast<bool*>(running_flag) = false;
//     cout.put('E'); cout.flush(); // indicate thread ended
//     
//     // Release the per-thread resources before we exit
//     mysqlpp::Connection::thread_end();

//     return 0;
// }


};

#endif 
