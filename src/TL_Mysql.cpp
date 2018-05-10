/*
 * File:   TL_Mysql.cpp
 * Author: everpan
 *
 * Created on 2011年4月4日, 下午9:33
 */

#include "TL_Mysql.h"
#if USE_MYSQL
namespace tidp {

TL_Mysql::TL_Mysql() {
    _conf.port = 3306;
    _isconnected = false;
}

TL_Mysql::~TL_Mysql() {
    if (_isconnected) {
        mysql_close(&_mysql);
        _isconnected = false;
    }
}

bool TL_Mysql::connect(const std::string& hostname, const std::string& username,
        const std::string& password, int port, const std::string& dbname) {
    _conf.hostname = hostname;
    _conf.username = username;
    _conf.password = password;
    _conf.dbname = dbname;
    _conf.port = port;
    _isconnected = false;
    return reConnect();
}

void TL_Mysql::close() {
    if (_isconnected) {
        mysql_close(&_mysql);
        _isconnected = false;
    }
}

bool TL_Mysql::connect(const TL_MysqlConf & conf) {
    _conf = conf;
    return reConnect();
}

bool TL_Mysql::reConnect() {
    close();
    if (!mysql_init(&_mysql)) {
        char errstr[200];
        int err = mysql_errno(&_mysql);
        snprintf(errstr, 200, "TL_Mysql Error::connect:%d %s\n", err,
                mysql_error(&_mysql));
        throw TL_Exp(errstr, err);
    }
    MYSQL * _pmysql = mysql_real_connect(&_mysql, _conf.hostname.c_str(),
            _conf.username.c_str(), _conf.password.c_str(), _conf.dbname.c_str(), _conf.port, NULL,
            0);
    if (_pmysql == NULL || _pmysql != &_mysql) {
        char errstr[200];
        int err = mysql_errno(&_mysql);
        snprintf(errstr, 200, "TL_Mysql Error::connect:%d %s\n", err,
                mysql_error(&_mysql));
        throw TL_Exp(errstr, err);
    }
    _isconnected = true;
    return true;
}

bool TL_Mysql::isConnect() {
    return _isconnected;
}

MYSQL * TL_Mysql::getMysql() {
    return &_mysql;
}

int TL_Mysql::query(const std::string& query) {
    return this->query(query.c_str(), query.size());
}

int TL_Mysql::query(const char* query, size_t qlen) {
    if (!_isconnected) {
        reConnect();
    }
    int ret = mysql_real_query(&_mysql, query, qlen);
    if (ret != 0) {
        int eno = mysql_errno(&_mysql);
        if (eno == CR_SERVER_LOST || eno == CR_SERVER_GONE_ERROR) {
            try {
                reConnect();
                ret = mysql_real_query(&_mysql, query, qlen);
                if (ret != 0) {
                    char errstr[200];
                    int err = mysql_errno(&_mysql);
                    snprintf(errstr, 200, "TL_Mysql Error 001::query:%d %d %s\n", ret,
                            err, mysql_error(&_mysql));
                    throw TL_Exp(errstr, err);
                }
            } catch (TL_Exp& e) {
                throw e;
            }
        } else {
            char errstr[200];
            snprintf(errstr, 200, "TL_Mysql Error 002::query:%d %d %s\n", ret,
                    mysql_errno(&_mysql), mysql_error(&_mysql));
            throw TL_Exp(errstr);
        }
    }
    return ret;
}

/*int iner_query(const char * query, unsigned qlen){
 return mysql_real_query(&_mysql, query, qlen);
 }*/
unsigned long long TL_Mysql::insertId() {
    return mysql_insert_id(&_mysql);
}

MYSQL_RES * TL_Mysql::getRawResult() {
    return mysql_store_result(&_mysql);
}

TL_Mysql::TL_MysqlRes TL_Mysql::getResult() {
    return TL_MysqlRes(&_mysql);
}

TL_Mysql::TL_MysqlRes::TL_MysqlRes(const TL_Mysql::TL_MysqlRes & clone) {
    destory();
    _result = clone._result;
}

TL_Mysql::TL_MysqlRes::TL_MysqlRes(MYSQL * mysql) {
    //if(_result)
    _result = mysql_store_result(mysql);
    if (_result == NULL) {
        if (mysql_field_count(mysql) == 0) {
            //insert or update
            //not select
            _rows_num = mysql_affected_rows(mysql);
            //_fields_num = 0;
            _is_select = false;
        } else {
            std::ostringstream os;
            os << "TL_MysqlRes::TL_MysqlRes Error:" << mysql_errno(mysql) << " "
                    << mysql_error(mysql) << std::endl;
            throw TL_Exp(os.str());
        }
    } else {
        _fields_num = mysql_num_fields(_result);
        _rows_num = mysql_num_rows(_result);
        MYSQL_FIELD* field = mysql_fetch_field(_result);
        _field_no.clear();
        for (size_t i = 0; i < _fields_num; ++i) {
            _field_no[field[i].name] = i;
            if (_table_name.empty())
                _table_name = field[i].table;
        }
        _is_select = true;
    }
    _counter = 1;

}

TL_Mysql::TL_MysqlRes::TL_MysqlRes() {
    _result = NULL;
    _row = NULL;
    _fields_num = 0;
    _rows_num = 0;
    _counter = 0;
}

TL_Mysql::TL_MysqlRes::~TL_MysqlRes() {
    --_counter;
    if (_counter <= 0) {
        destory();
    }
}

void TL_Mysql::TL_MysqlRes::destory() {
    if (_result != NULL) {
        mysql_free_result(_result);
        _result = NULL;
        _row = NULL;
        _fields_num = 0;
        _rows_num = 0;
        _counter = 0;
    }
}

MYSQL_FIELD * TL_Mysql::TL_MysqlRes::getFieldInfo(unsigned int i) {
    return mysql_fetch_field_direct(_result, i);
}

MYSQL_FIELD * TL_Mysql::TL_MysqlRes::getFieldsInfo() {
    return mysql_fetch_field(_result);
}

unsigned int TL_Mysql::TL_MysqlRes::getFieldNum() {
    return _fields_num;
}

my_ulonglong TL_Mysql::TL_MysqlRes::getRowNum() {
    return _rows_num;
}

my_ulonglong TL_Mysql::TL_MysqlRes::getAffectedRows() {
    return _rows_num;
}

bool TL_Mysql::TL_MysqlRes::isSelectAction() {
    return _is_select;
}

const char* TL_Mysql::TL_MysqlRes::operator[](const std::string & field_name) {
    std::map<std::string, int>::iterator it = _field_no.find(field_name);
    if (it == _field_no.end()) {
        throw TL_Exp(
                "TL_Mysql:TL_MysqlRes:the [" + _table_name + "." + field_name
                + "] is not exist.");
    }
    if (_row == NULL) {
        throw TL_Exp("TL_Mysql:TL_MysqlRes:Row is end.");
    }
    const char* r = _row[it->second];
    return r == NULL ? "NULL" : r;
}

const char* TL_Mysql::TL_MysqlRes::operator[](int fieldno) {
    if (fieldno >= (int) _fields_num) {
        char c[100];
        snprintf(c, 100, "out of rang %d", fieldno);
        throw TL_Exp(c);
    }
    if (_row == NULL) {
        throw TL_Exp("TL_Mysql:TL_MysqlRes:Row is end.");
    }
    const char*r = _row[fieldno];
    return r == NULL ? "NULL" : r;
}

bool TL_Mysql::TL_MysqlRes::next() {
    _row = mysql_fetch_row(_result);
    return _row != NULL;
}

bool TL_Mysql::TL_MysqlRes::next(TL_Mysql::TL_MysqlRes::TL_MYSQL_ROW& rowinfo) {
    _row = mysql_fetch_row(_result);
    rowinfo.rows = _row;
    rowinfo.lengths = mysql_fetch_lengths(_result);
    return _row != NULL;
}

void TL_Mysql::TL_MysqlRes::dataSeek(long long offset) {
    if (offset >= _rows_num && _rows_num > 0)
        offset = _rows_num - 1;
    if (offset < 0)
        offset = 0;
    mysql_data_seek(_result, offset);
}

MYSQL_ROW TL_Mysql::TL_MysqlRes::getRows() {
    return mysql_fetch_row(_result);
}

TL_Mysql::TL_MysqlRes::TL_MYSQL_ROW TL_Mysql::TL_MysqlRes::getRowsLengths() {
    TL_MYSQL_ROW row;
    row.rows = mysql_fetch_row(_result);
    row.lengths = mysql_fetch_lengths(_result);
    return row;
}

const std::map<std::string, int>& TL_Mysql::TL_MysqlRes::getField() const {
    return _field_no;
}

TL_Mysql::TL_MysqlRes & TL_Mysql::TL_MysqlRes::operator=(
        const TL_Mysql::TL_MysqlRes & res) {
    destory();
    _result = res._result;
    ++res._counter;
    _counter = 1;
    _fields_num = res._fields_num;
    _rows_num = res._rows_num;
    _field_no = res._field_no;
    return *this;
}

}

#endif
