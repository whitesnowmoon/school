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
    bool initSSL(const char* crt_file_path, const char* key_file_path, const char* CA_file_path = 0);//����֤��͹�Կ
    static _SSL_* Get__SSL__Instance();//���_SSL_�������
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
    SSL_library_init();// SSL ���ʼ�� 
    OpenSSL_add_all_algorithms();//�����㷨
    this->ctx = SSL_CTX_new(SSLv23_server_method());     //����ssl_ctx������
    if (!this->ctx) {
        std::cerr << "SSL_CTX_new TLS_server_method error!\n";
        return false;
    }
    bool success = SSL_CTX_use_certificate_file(this->ctx, crt_file_path, SSL_FILETYPE_PEM);//��SSL_FILETYPE_PEM��base64���룩����֤���ļ�
    if (success != true) {  //1�ɹ�0ʧ��
        ERR_print_errors_fp(stderr);
        return false;
    }
    success = SSL_CTX_use_PrivateKey_file(this->ctx, key_file_path, SSL_FILETYPE_PEM);//��SSL_FILETYPE_PEM��base64���룩����֤���Ӧ˽Կ�ļ�
    if (success != true) {  //1�ɹ�0ʧ��
        ERR_print_errors_fp(stderr);
        return false;
    }
    std::string password;
    if (password.size() != 0) {
        password.push_back(0);//������0��Ϊʶ�����
        SSL_CTX_set_default_passwd_cb_userdata(ctx, (char*)password.c_str());//����˽Կ����
    }
    success = SSL_CTX_check_private_key(this->ctx);//��֤˽Կ��֤���Ƿ�ƥ��
    if (success != true) {  //1�ɹ�0ʧ��
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
    bool openstream(int socket);//������
    int _read_(void* buff, unsigned int size);//�������ж�ȡ����ȷ���ض�ȡ��������������0,���󷵻�С��0
    int _write_(void* buff, unsigned int size);//��������д�룬��ȷ����д����������������0,���󷵻�С��0
    int _getchar_(char& ch);//�������һ���ֽ�,��������0,���󷵻�С��0
    int _putchar_(char ch) noexcept;//����д��һ���ֽ�,,��������0,���󷵻�С��0
    void closestream();//�ر���
    NetStreamSSL()=default;
    NetStreamSSL(NetStreamSSL& netstreamraw) = default;//������ʹ��ͬһ��socket
    ~NetStreamSSL()=default;
};


int NetStreamSSL::_read_(void* buff, unsigned int size) {
    return SSL_read(this->ssl, buff, size); //ssl��ȡ
}
int NetStreamSSL::_write_(void* buff, unsigned int size) {
    return SSL_write(this->ssl, buff, size); //sslд��
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
        SSL_free(this->ssl);//�ͷ�SSL
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
        SSL_shutdown(this->ssl);//֪ͨ�ͻ��˹ر�
        SSL_free(this->ssl);//�ͷ�SSL
        this->ssl = 0;
    }
}

#endif