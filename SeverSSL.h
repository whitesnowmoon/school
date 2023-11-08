#ifndef _SEVERSSL__H_
#define _SEVERSSL__H_



#include<iostream>
#include<openssl/ssl.h>
#include<openssl/err.h>

class _SSL_
{
public:
    static SSL_CTX* ctx;
    static _SSL_* instance;
    bool initSSL(const char* crt_file_path, const char* key_file_path, const char* CA_file_path = 0);//加载证书和公钥
    static _SSL_* Get__SSL__Instance();//获得_SSL_单例句柄
private:
    _SSL_() = default;
    ~_SSL_();
    class clear
    {
    public:
        ~clear() {
            if (instance)
                delete instance;
        }
    };
};

SSL_CTX* _SSL_::ctx=nullptr;
_SSL_* _SSL_::instance=nullptr;

 _SSL_* _SSL_::Get__SSL__Instance() {
     static clear clr_SSL__;
     if (instance == nullptr) {
         instance = new _SSL_();
     }
     return instance;
}

bool _SSL_::initSSL(const char* crt_file_path, const char* key_file_path, const char* CA_file_path) {
    SSL_library_init();// SSL 库初始化 
    OpenSSL_add_all_algorithms();//加载算法
    this->ctx = SSL_CTX_new(SSLv23_server_method());     //创建ssl_ctx上下文
    if (!this->ctx) {
        std::cerr << "SSL_CTX_new TLS_server_method error!\n";
        return false;
    }
    bool success = SSL_CTX_use_certificate_file(this->ctx, crt_file_path, SSL_FILETYPE_PEM);//以SSL_FILETYPE_PEM（base64编码）加载证书文件
    if (success != true) {  //1成功0失败
        ERR_print_errors_fp(stderr);
        return false;
    }
    success = SSL_CTX_use_PrivateKey_file(this->ctx, key_file_path, SSL_FILETYPE_PEM);//以SSL_FILETYPE_PEM（base64编码）加载证书对应私钥文件
    if (success != true) {  //1成功0失败
        ERR_print_errors_fp(stderr);
        return false;
    }
    std::string password;
    if (password.size() != 0) {
        password.push_back(0);//密码以0作为识别结束
        SSL_CTX_set_default_passwd_cb_userdata(ctx, (char*)password.c_str());//设置私钥密码
    }
    success = SSL_CTX_check_private_key(this->ctx);//验证私钥与证书是否匹配
    if (success != true) {  //1成功0失败
        ERR_print_errors_fp(stderr);
        return false;
    }
    return true;
}
_SSL_::~_SSL_() {
    SSL_CTX_free(this->ctx);
}

class NetStreamSSL
{
private:
    int socket=-1;
    SSL* ssl=NULL;
public:
    char oldone = NULL;
    bool openstream(int socket);//开启流
    int _read_(void* buff, unsigned int size);//对流进行读取，正确返回读取数量，结束返回0,错误返回小于0
    int _write_(void* buff, unsigned int size);//对流进行写入，正确返回写入数量，结束返回0,错误返回小于0
    int _getchar_(char& ch);//从流获得一个字节,结束返回0,错误返回小于0
    int _putchar_(char ch) noexcept;//从流写入一个字节,,结束返回0,错误返回小于0
    void closestream();//关闭流
    NetStreamSSL()=default;
    NetStreamSSL(NetStreamSSL& netstreamraw) = default;//允许复制使用同一个socket
    ~NetStreamSSL()=default;
};


int NetStreamSSL::_read_(void* buff, unsigned int size) {
    return SSL_read(this->ssl, buff, size); //ssl读取
}
int NetStreamSSL::_write_(void* buff, unsigned int size) {
    return SSL_write(this->ssl, buff, size); //ssl写入
}


bool NetStreamSSL::openstream(int socket) {
    this->socket = socket;
    ssl = SSL_new(_SSL_::ctx);
    if (this->socket == -1)
        return false;
    SSL_set_fd(ssl, this->socket);
    int code = SSL_accept(ssl);
    while (SSL_ERROR_WANT_READ == SSL_get_error(this->ssl, code))
        code = SSL_accept(this->ssl);
    if (code <= 0) {
        std::cout << "ssl error code :" << SSL_get_error(ssl, code) << "\n";
        SSL_free(this->ssl);//释放SSL
        this->ssl = 0;
        return false;
    }
    return true;
}

int NetStreamSSL::_putchar_(char ch) noexcept {
    return SSL_write(this->ssl, &ch, 1);
}
int NetStreamSSL::_getchar_(char& ch) {
    this->oldone = ch;
    return SSL_read(this->ssl, &ch, 1);
}
void NetStreamSSL::closestream() {
    if (this->ssl) {
        SSL_shutdown(this->ssl);//通知客户端关闭
        SSL_free(this->ssl);//释放SSL
        this->ssl = 0;
    }
}

#endif