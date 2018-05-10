/* 
 * File:   TL_MysqlPool.h
 * Author: everpan
 *
 * Created on 2011年6月29日, 上午11:44
 */

#ifndef TL_MYSQLPOOL_H
#define TL_MYSQLPOOL_H
#include <set>
#include "TL_ThreadLock.h"
#include "TL_Mysql.h"
#include "TL_Singleton.h"
#include "TL_Exp.h"
#if USE_MYSQL
namespace tidp {

    class TL_MysqlPool : public TL_Singleton<TL_MysqlPool>, public TL_ThreadLock {
    public:
        TL_MysqlPool();
        //TL_MysqlPool(const TL_MysqlPool& orig);
        virtual ~TL_MysqlPool();
        void init(const std::string& hostname, const std::string& username, const std::string& password, int port = 3306,
                const std::string& dbname = "");
        TL_Mysql* get();
        void release(TL_Mysql* psql);
        void remove(TL_Mysql*psql);
        void setPoolSize(int size);
        void clear();
    private:
        //TL_ThreadDataPool<TL_Mysql> _mysql_pool;
        std::string _hostname;
        std::string _username;
        std::string _password;
        int _port;
        std::string _dbname;

        std::set<TL_Mysql*> _idle;
        std::set<TL_Mysql*> _busy;

        size_t _max;
    };
#define MYSQLPOOL_GETPTR() TL_MysqlPool::getInstance()->get()
#define MYSQLPOOL_RELEASE(psql) TL_MysqlPool::getInstance()->release(psql)

    //连接池耗尽的时候，如果没有释放就会造成等待，竞争条件一直存在，死锁假象
    //如 TL_MysqlPtr[_max+n] 这里会一直hode

    class TL_MysqlPtr {
    public:

        TL_MysqlPtr() {
            psql = MYSQLPOOL_GETPTR();
        }

        ~TL_MysqlPtr() {
            MYSQLPOOL_RELEASE(psql);
        }

        TL_Mysql* operator->() {
            return psql;
        }

        TL_Mysql& operator*() {
            return *psql;
        }
    protected:
        TL_MysqlPtr(const TL_MysqlPtr& orig);
        TL_MysqlPtr& operator=(const TL_MysqlPtr& orig);
    private:
        TL_Mysql* psql;
    };

}

//20180509 多mysql实例连接池




#endif /* TL_MYSQLPOOL_H */
#endif
