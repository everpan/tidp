/*
 * File:   TL_Mysql.h
 * Author: everpan
 *
 * Created on 2011年4月4日, 下午9:33
 */

#ifndef TL_MYSQL_H
#define	TL_MYSQL_H
#include <stdio.h>
#include <mysql.h>
#include <errmsg.h>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include "TL_Pool.h"
#include "TL_Exp.h"
namespace tidp {

    class TL_Mysql {
    public:
        TL_Mysql();

        virtual ~TL_Mysql();
        //TL_MysqlRes----------------------------------------------------------------------

        class TL_MysqlRes {
        public:
            friend class TL_MysqlRow;

            typedef struct {
                MYSQL_ROW rows;
                unsigned long* lengths;
            } TL_MYSQL_ROW;

            TL_MysqlRes(MYSQL* mysql);
            TL_MysqlRes(const TL_MysqlRes& clone);
            TL_MysqlRes();
            virtual ~TL_MysqlRes();
            MYSQL_FIELD* getFieldInfo(unsigned int i);
            MYSQL_FIELD* getFieldsInfo();
            const char* operator[](const std::string & field_name);
            const char* operator[](int fieldno);
            unsigned int getFieldNum();
            void destory();
            //void setAutoDestory(bool autodestory);
            my_ulonglong getRowNum();

            my_ulonglong getAffectedRows();
            bool isSelectAction();
            MYSQL_ROW getRows();
            TL_MYSQL_ROW getRowsLengths();
            bool next();
            bool next(TL_MYSQL_ROW& rowinfo);
            void dataSeek(long long offset = 0);
            const std::map<std::string, int>& getField() const;

            TL_MysqlRes &operator = (const TL_MysqlRes & res);

        private:

            MYSQL_RES* _result;
            MYSQL_ROW _row;
            bool _is_select;
            unsigned int _fields_num;
            unsigned int _rows_num;
            std::string _table_name;
            std::map<std::string, int> _field_no;

            mutable int _counter;
        }; // end TL_MysqlRes



        bool connect(const std::string& hostname,
                const std::string& username,
                const std::string& password,
                int port = 3306, const std::string& dbname="");
        bool reConnect();

        bool isConnect();
        void close();
        MYSQL* getMysql();
        MYSQL_RES *getRawResult();
        bool freeResult();
        int query(const std::string& query);
        int query(const char * query, size_t qlen);

        unsigned long long insertId();
        TL_Mysql::TL_MysqlRes getResult();
        //std::string escapeString(const std::string &s);
    protected:
        TL_Mysql(const TL_Mysql& orig);
    private:
        //int iner_query(const char * query, unsigned qlen);
        std::string _hostname;
        std::string _username;
        std::string _password;
        int _port;
        std::string _dbname;
        MYSQL _mysql;
        bool _isconnected;
    };
#ifndef TL_MYSQLPOOL_H
    typedef TL_Pool<TL_Mysql>::Ptr TL_MysqlPtr;
#endif
}
#endif	/* TL_MYSQL_H */

