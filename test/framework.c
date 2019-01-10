/* Public domain, no copyright. Use at your own risk. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#ifndef _WIN32
  #include <netinet/in.h>
#else
  #include <unistd.h>
#endif
#include <inttypes.h>

#include <check.h>
#include <ulfius.h>

#define CERT_KEY "-----BEGIN PRIVATE KEY-----\
MIIEvwIBADANBgkqhkiG9w0BAQEFAASCBKkwggSlAgEAAoIBAQDr90HrswgEmln/\
rXeNqYq0boIvas5wu27hmeHDdGGKtkCWIWGAo9GUy45xqsI4mDl3bOWS+pmb/3yi\
+nhe+BmYHvEqUFo1JfUcVMxaNEbdd9REytMjKdOS+kkLf++BBRoZI/g8DggIu+Ri\
dOSypk+pUECyQxROsyCrB/FgXuKbyC4QNl7fqZxMSpzw7jsWCZiwFv4pu8kMqzDG\
2wTl/r/4STyK4Pj2TVa/JVzbZbH7VfcjT8MdMsXvKhlmPywjbqo70Hnmt3cnakYF\
X+07ncx/5mjYYd3eSFgiNXr7WNw2rhFKtfTUcjrqSw9FDxmHFWUU76mwJyUo02N9\
ViakSoQpAgMBAAECggEBAJp/VBwdJpzM6yxqyaJpZbXpvTeKuQw6zMjN1nIBG3SV\
DAjAZnSxziGcffGSmoQvt0CoflAT4MuxJkwXrwSPcUKWz9Sis82kwq4AH6TYIaYU\
NVmtazzUwAC1+2maJJjXXFUlpfy8Oypsy4ZjfvIxzmrPbuzI2t0Ej9kr5DDzL3BL\
CWQ/U7w7y4KC0Pnq1ueIzM+UJIfvI0ldUcXHWsAnjyQzwgFBC35qDOfDTw0YUJv+\
ElfFFcGYCA+9wlQyhM/zhAWqKgZ2mwAS6WykgbSc7j4NDjlmZwf4ZuTxbDUV1kBX\
pPH21snqO42CFpw9hRUAA0W0XydCIfUhH8/6tH9enQECgYEA+rM9f6cUk3c7aLWs\
hnauVqJuyGhgCkMyF9sSxgfcs87OVLNuGgaTIfwcT/7oxAY8G7sY44cbk1ZRhh7y\
6kf01xqiJeXxBQei1qiJxMb2gukvpeY81s2Mg9og5d9qbEhLzp8TdiRJHxLIiGwF\
xOM69CpugKN4T0Zum7EBGeSvmBECgYEA8PRG5SRTE4JwzGtLuTbMbjYTqyEjXAkB\
zo33a92znA0EXEeLCl845EUgzUkSkeN/T/uyWRjj0hrPU99UaaXHt3bc+lrDHrc7\
WFAR3QoAfFFJPIqqwiHcBDdTeAozQ8IOqFIxspl72RukuRdeQR3EdfcF9TUZyUbU\
k8SuRioggpkCgYA2scgnA3KvwYGKlKgxJc9fQ0zcGDlrw8E4BymPXsO9zs6hGAxb\
TTfoYDJlGX361kli22zQpvdTK6/ZjQL+LfiyvTLHBeWRbVsPbfGwpp+9a9ZjYVnA\
m1OeqIYo4Jc9TICNcZMzYTM6vkRVzwtrKw//mQpGsmNbGEilWvaciZHtoQKBgQDo\
FDBQtir6SJIConm9/ETtBlLtai6Xj+lYnK6qC1DaxkLj6tjF9a9jVh3g/DfRopBW\
ZnSCkpGkJcR54Up5s35ofCkdTdxPsmaLihuaje6nztc+Y8VS1LAIs41GunRkF/5s\
KzbI8kIyfAitag+Toms+v93SLwIWNo27gh3lYOANSQKBgQDIidSO3fzB+jzJh7R0\
Yy9ADWbBsLxc8u+sBdxmZBGl+l4YZWNPlQsnsafwcpJWT3le6N7Ri3iuOZw9KiGe\
QDkc7olxUZZ3pshg+cOORK6jVE8v6FeUlLnxpeAWa4C4JDawGPTOBct6bVBl5sxi\
7GaqDcEK1TSxc4cUaiiPDNNXQA==\
-----END PRIVATE KEY-----"
#define CERT_PEM "-----BEGIN CERTIFICATE-----\
MIIDhTCCAm2gAwIBAgIJANrO2RnCbURLMA0GCSqGSIb3DQEBCwUAMFkxCzAJBgNV\
BAYTAkFVMRMwEQYDVQQIDApTb21lLVN0YXRlMSEwHwYDVQQKDBhJbnRlcm5ldCBX\
aWRnaXRzIFB0eSBMdGQxEjAQBgNVBAMMCWxvY2FsaG9zdDAeFw0xNzA0MjgxNTA0\
NDVaFw0xODA0MjgxNTA0NDVaMFkxCzAJBgNVBAYTAkFVMRMwEQYDVQQIDApTb21l\
LVN0YXRlMSEwHwYDVQQKDBhJbnRlcm5ldCBXaWRnaXRzIFB0eSBMdGQxEjAQBgNV\
BAMMCWxvY2FsaG9zdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOv3\
QeuzCASaWf+td42pirRugi9qznC7buGZ4cN0YYq2QJYhYYCj0ZTLjnGqwjiYOXds\
5ZL6mZv/fKL6eF74GZge8SpQWjUl9RxUzFo0Rt131ETK0yMp05L6SQt/74EFGhkj\
+DwOCAi75GJ05LKmT6lQQLJDFE6zIKsH8WBe4pvILhA2Xt+pnExKnPDuOxYJmLAW\
/im7yQyrMMbbBOX+v/hJPIrg+PZNVr8lXNtlsftV9yNPwx0yxe8qGWY/LCNuqjvQ\
eea3dydqRgVf7TudzH/maNhh3d5IWCI1evtY3DauEUq19NRyOupLD0UPGYcVZRTv\
qbAnJSjTY31WJqRKhCkCAwEAAaNQME4wHQYDVR0OBBYEFPFfmGA3jO9koBZNGNZC\
T/dZHZyHMB8GA1UdIwQYMBaAFPFfmGA3jO9koBZNGNZCT/dZHZyHMAwGA1UdEwQF\
MAMBAf8wDQYJKoZIhvcNAQELBQADggEBAIc8Yuom4vz82izNEV+9bcCvuabcVwLH\
Qgpv5Nzy/W+1hDoqfMfKNwOSdUB7jZoDaNDG1WhjKGGCLTAx4Hx+q1LwUXvu4Bs1\
woocge65bl85h10l2TxxnlT5BIJezm5r3NiZSwOK2zxxIEyL4vh+b/xqQblBEkR3\
e4/A4Ugn9Egh8GdpF4klGp4MjjpRyAVI7BDaleAhvDSfPmm7ylHJ2y7CLI9ApOQY\
glwRuTmowAZQtaSiE1Ox7QtWj858HDzzTZyFWRG/MNqQptn7AMTPJv3DivNfDNPj\
fYxFAheH3CjryHqqR9DD+d9396W8mqEaUp+plMwSjpcTDSR4rEQkUJg=\
-----END CERTIFICATE-----"
#define CERT_CA "-----BEGIN CERTIFICATE-----\
MIIF9zCCA9+gAwIBAgIUQhoxaAC0kHeq1DNRL8fA0/WZCsYwDQYJKoZIhvcNAQEL\
BQAwgYoxCzAJBgNVBAYTAkNBMQ8wDQYDVQQIDAZRdWViZWMxDzANBgNVBAcMBlF1\
ZWJlYzEPMA0GA1UECgwGVWxmaXVzMRAwDgYDVQQLDAd0ZXN0LWNhMRIwEAYDVQQD\
DAlsb2NhbGhvc3QxIjAgBgkqhkiG9w0BCQEWE3dlYm1hc3RlckBsb2NhbGhvc3Qw\
HhcNMTgxMjAyMTkzNTA1WhcNMTkxMjAyMTkzNTA1WjCBijELMAkGA1UEBhMCQ0Ex\
DzANBgNVBAgMBlF1ZWJlYzEPMA0GA1UEBwwGUXVlYmVjMQ8wDQYDVQQKDAZVbGZp\
dXMxEDAOBgNVBAsMB3Rlc3QtY2ExEjAQBgNVBAMMCWxvY2FsaG9zdDEiMCAGCSqG\
SIb3DQEJARYTd2VibWFzdGVyQGxvY2FsaG9zdDCCAiIwDQYJKoZIhvcNAQEBBQAD\
ggIPADCCAgoCggIBALG4KdvHcAsKqaNe4RPVvGUA7P+N9+8vs8dTBQ16VEUGHCp5\
bWdqpvWdiHfxKmCZVtVUkYGwOsFmsbOsAfQgF1gcqTFyIRCwdt2PbvWso2n3S+aJ\
gCdqzYMpCoW09/1izAGqXVWrWhtBEFJiFSx0wbkzEF87ATLv1eox0g1C3y9LuIv0\
T1nK36jPzC/S7hazCd4IXlzPMy858VJn8qv09TeexYLEEcvn70vTQzoOKQKAWTDO\
BgwSDcfmez2uXktIuJR7EW3weaj6xJQWpnB8eb+A3H82TtP/EhB20ipc/X/lNBaj\
58yKj5XJwVAoteIGYDV7PBzwFhyqhbErDNsMDcR2CrNLIzpdG/BpIwVbAyjoYCUm\
OGxw8k+WXfyvTyS9TTi//sZbNRaVGAir63LFwCh0YbJ36DfKpjPrUoqDkO93iX3C\
x12Nq5Mry8v6PgQsPt+sqZsDYW2BU0ib/pb4cksgBZakJcMFLcYi5tlCqFis1ehn\
QjAZWn+kGCc4Tmy/l9mBBdRiYpP77QH6QVh4bPHzxaYiOoLfBajE6dWCXdLYOu+m\
oZQmlcRWuP5AhKi0QAaI1/RAKGpO0dEUH7Dnl5TqR8twC88ZetkZPEJYHb3Vsy7U\
aA0EvoHi1xTdh0tPN7AL0RRD8ekyh273CUMHg3vI0qjv29MWSBzCDa9X2SL5AgMB\
AAGjUzBRMB0GA1UdDgQWBBQkE2nQu0XaDkJdLSQsgjTIXULLGDAfBgNVHSMEGDAW\
gBQkE2nQu0XaDkJdLSQsgjTIXULLGDAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3\
DQEBCwUAA4ICAQASrWMYrsMtIfUsN3srEAZqutcVAN7PWbzVNAjWXEqbw/HV7m8J\
w09MTfmwmQEhw4d7fwzfax5vqKeqfRb7i8f+HzR8b9egCn8n4hWUjiAhS1Naj8OP\
RCrvPyVI1/h3iMZZXlITljtgk7rZn5Yv/WjKz/l32KIHwMzmk5aTGkmr0oyWxj1Y\
eJymZ0RYih+X4hUvAlW3a3AlKbvq7KW9bYU4LHJWixgP16tXOR8k0aPKpyVuNCzN\
7/23vfPeawQuW7C23+IuDuPyrpivw4dbcKNcy1KfGXmc8kQDIv6uPqdWa5ngvVua\
SHb23nA4HG3k5ed8iqarDmu4nU9TTu8qTaMKSuEAjRsPnehO/Fi/m3mMSnGEXxa1\
WWl1wZxvFoD4IsexNZaZcii9f0Ctn3KbL7HwlJliogc2jDVHheS5aVsVnl2UsNJf\
x4nYzPZ6bdLawcviwo9eQWXlEjoKYyBTL1LFNt9S/iEW3WUOaaLJVH7+uHOn4/6N\
wiMh3OqcyePyB+MKr3tvsmYZyYVwCW/pRmATM+F4ol23OWhXyYN+AFdXiMBjwdUX\
jAwgDammq/ymxfgKE7IX8z9+f+8UVcBaiXVUTWzgSHnjjPW8+9WExUws8BFD7+81\
G66c7c4qxP2fq5vQiYJUVEpNd4Z4+EbvMDrg4CsTVaoI1OWlHWcBfNHupw==\
-----END CERTIFICATE-----"

/**
 * decode a u_map into a string
 */
char * print_map(const struct _u_map * map) {
  char * line, * to_return = NULL;
  const char **keys, * value;
  int len, i;
  if (map != NULL) {
    keys = u_map_enum_keys(map);
    for (i=0; keys[i] != NULL; i++) {
      value = u_map_get(map, keys[i]);
      len = snprintf(NULL, 0, "key is %s, value is %s", keys[i], value);
      line = o_malloc((len+1)*sizeof(char));
      snprintf(line, (len+1), "key is %s, value is %s", keys[i], value);
      if (to_return != NULL) {
        len = o_strlen(to_return) + o_strlen(line) + 1;
        to_return = o_realloc(to_return, (len+1)*sizeof(char));
        if (o_strlen(to_return) > 0) {
          strcat(to_return, "\n");
        }
      } else {
        to_return = o_malloc((o_strlen(line) + 1)*sizeof(char));
        to_return[0] = 0;
      }
      strcat(to_return, line);
      o_free(line);
    }
    return to_return;
  } else {
    return NULL;
  }
}

int callback_function_empty(const struct _u_request * request, struct _u_response * response, void * user_data) {
  return U_CALLBACK_CONTINUE;
}

int callback_function_return_url(const struct _u_request * request, struct _u_response * response, void * user_data) {
  ulfius_set_string_body_response(response, 200, request->http_url);
  return U_CALLBACK_CONTINUE;
}

int callback_function_error(const struct _u_request * request, struct _u_response * response, void * user_data) {
  return U_CALLBACK_ERROR;
}

int callback_function_unauthorized(const struct _u_request * request, struct _u_response * response, void * user_data) {
  return U_CALLBACK_UNAUTHORIZED;
}

int callback_function_check_auth(const struct _u_request * request, struct _u_response * response, void * user_data) {
  if (o_strcmp("user", request->auth_basic_user) == 0 && o_strcmp("password", request->auth_basic_password) == 0) {
    return U_CALLBACK_CONTINUE;
  } else {
    return U_CALLBACK_UNAUTHORIZED;
  }
}

int callback_function_param(const struct _u_request * request, struct _u_response * response, void * user_data) {
  char * param3, * body;
  
  if (u_map_has_key(request->map_url, "param3")) {
    param3 = msprintf(", param3 is %s", u_map_get(request->map_url, "param3"));
  } else {
    param3 = o_strdup("");
  }
  body = msprintf("param1 is %s, param2 is %s%s", u_map_get(request->map_url, "param1"), u_map_get(request->map_url, "param2"), param3);
  ulfius_set_string_body_response(response, 200, body);
  o_free(body);
  o_free(param3);
  return U_CALLBACK_CONTINUE;
}

int callback_function_body_param(const struct _u_request * request, struct _u_response * response, void * user_data) {
  char * body;
  
  body = msprintf("param1 is %s, param2 is %s", u_map_get(request->map_post_body, "param1"), u_map_get(request->map_post_body, "param2"));
  ulfius_set_string_body_response(response, 200, body);
  o_free(body);
  return U_CALLBACK_CONTINUE;
}

int callback_function_header_param(const struct _u_request * request, struct _u_response * response, void * user_data) {
  char * body;
  
  body = msprintf("param1 is %s, param2 is %s", u_map_get(request->map_header, "param1"), u_map_get(request->map_header, "param2"));
  ulfius_set_string_body_response(response, 200, body);
  o_free(body);
  return U_CALLBACK_CONTINUE;
}

int callback_function_cookie_param(const struct _u_request * request, struct _u_response * response, void * user_data) {
  char * body;
  
  body = msprintf("param1 is %s", u_map_get(request->map_cookie, "param1"));
  ck_assert_int_eq(ulfius_set_string_body_response(response, 200, body), U_OK);
  ck_assert_int_eq(ulfius_add_cookie_to_response(response, "param2", "value_cookie", NULL, 100, "localhost", "/cookie", 0, 1), U_OK);
  ck_assert_int_eq(ulfius_add_same_site_cookie_to_response(response, "cookieSameSiteStrict", "value_cookie", NULL, 100, "localhost", "/cookie", 0, 1, U_COOKIE_SAME_SITE_STRICT), U_OK);
  ck_assert_int_eq(ulfius_add_same_site_cookie_to_response(response, "cookieSameSiteLax", "value_cookie", NULL, 100, "localhost", "/cookie", 0, 1, U_COOKIE_SAME_SITE_LAX), U_OK);
  ck_assert_int_eq(ulfius_add_same_site_cookie_to_response(response, "cookieSameSiteNone", "value_cookie", NULL, 100, "localhost", "/cookie", 0, 1, U_COOKIE_SAME_SITE_NONE), U_OK);
  ck_assert_int_ne(ulfius_add_same_site_cookie_to_response(response, "cookieSameSiteError", "value_cookie", NULL, 100, "localhost", "/cookie", 0, 1, 42), U_OK);
  o_free(body);
  return U_CALLBACK_CONTINUE;
}

int callback_function_multiple_continue(const struct _u_request * request, struct _u_response * response, void * user_data) {
  if (response->binary_body != NULL) {
    char * body = msprintf("%.*s\n%s", response->binary_body_length, (char*)response->binary_body, request->http_url);
    ulfius_set_string_body_response(response, 200, body);
    o_free(body);
  } else {
    ulfius_set_string_body_response(response, 200, request->http_url);
  }
  return U_CALLBACK_CONTINUE;
}

int callback_function_multiple_complete(const struct _u_request * request, struct _u_response * response, void * user_data) {
  if (response->binary_body != NULL) {
    char * body = msprintf("%.*s\n%s", response->binary_body_length, (char*)response->binary_body, request->http_url);
    ulfius_set_string_body_response(response, 200, body);
    o_free(body);
  } else {
    ulfius_set_string_body_response(response, 200, request->http_url);
  }
  return U_CALLBACK_COMPLETE;
}


ssize_t stream_data (void * cls, uint64_t pos, char * buf, size_t max) {
  usleep(100);
  if (pos <= 100) {
      snprintf(buf, max, "%s %" PRIu64 "\n", (char *)cls, pos + 1);
      return o_strlen(buf);
  } else {
    return MHD_CONTENT_READER_END_OF_STREAM;
  }
}

void free_stream_data(void * cls) {
  o_free(cls);
}

int callback_function_stream (const struct _u_request * request, struct _u_response * response, void * user_data) {
  ulfius_set_stream_response(response, 200, stream_data, free_stream_data, U_STREAM_SIZE_UNKOWN, 32 * 1024, o_strdup("stream test"));
  return U_OK;
}

size_t my_write_body(void * contents, size_t size, size_t nmemb, void * user_data) {
  ck_assert_int_eq(o_strncmp((char *)contents, "stream test ", o_strlen("stream test ")), 0);
  ck_assert_int_ne(strtol((char *)contents + o_strlen("stream test "), NULL, 10), 0);
  return size * nmemb;
}

int callback_check_utf8_ignored(const struct _u_request * request, struct _u_response * response, void * user_data) {
  ck_assert_int_eq(u_map_has_key(request->map_header, "utf8_param"), 0);
  ck_assert_int_eq(u_map_has_key(request->map_url, "utf8_param1"), 0);
  ck_assert_int_eq(u_map_has_key(request->map_url, "utf8_param2"), 0);
  ck_assert_int_eq(u_map_has_key(request->map_post_body, "utf8_param"), 0);
  ck_assert_int_eq(u_map_has_key(request->map_header, "utf8_param_valid"), 1);
  ck_assert_int_eq(u_map_has_key(request->map_url, "utf8_param_valid1"), 1);
  ck_assert_int_eq(u_map_has_key(request->map_url, "utf8_param_valid2"), 1);
  ck_assert_int_eq(u_map_has_key(request->map_post_body, "utf8_param_valid"), 1);
  return U_OK;
}

int callback_check_utf8_not_ignored(const struct _u_request * request, struct _u_response * response, void * user_data) {
  ck_assert_int_eq(u_map_has_key(request->map_header, "utf8_param"), 1);
  ck_assert_int_eq(u_map_has_key(request->map_url, "utf8_param1"), 1);
  ck_assert_int_eq(u_map_has_key(request->map_url, "utf8_param2"), 1);
  ck_assert_int_eq(u_map_has_key(request->map_post_body, "utf8_param"), 1);
  ck_assert_int_eq(u_map_has_key(request->map_header, "utf8_param_valid"), 1);
  ck_assert_int_eq(u_map_has_key(request->map_url, "utf8_param_valid1"), 1);
  ck_assert_int_eq(u_map_has_key(request->map_url, "utf8_param_valid2"), 1);
  ck_assert_int_eq(u_map_has_key(request->map_post_body, "utf8_param_valid"), 1);
  return U_OK;
}

#ifndef U_DISABLE_GNUTLS
int callback_auth_client_cert (const struct _u_request * request, struct _u_response * response, void * user_data) {
  char * dn;
  size_t lbuf = 0;
  
  ck_assert_ptr_ne(request->client_cert, NULL);
  gnutls_x509_crt_get_dn(request->client_cert, NULL, &lbuf);
  dn = o_malloc(lbuf + 1);
  gnutls_x509_crt_get_dn(request->client_cert, dn, &lbuf);
  dn[lbuf] = '\0';
  ck_assert_str_eq(dn, "C=CA,ST=Quebec,L=Quebec,O=Ulfius,OU=test-client,CN=localhost,EMAIL=webmaster@localhost");
  ck_assert_int_eq(ulfius_set_string_body_response(response, 200, dn), U_OK);
  o_free(dn);

  return U_CALLBACK_CONTINUE;
}

int callback_no_auth_client_cert (const struct _u_request * request, struct _u_response * response, void * user_data) {
  ck_assert_ptr_eq(request->client_cert, NULL);
  
  return U_CALLBACK_CONTINUE;
}
#endif

START_TEST(test_ulfius_simple_endpoint)
{
  struct _u_instance u_instance;
  struct _u_request request;
  struct _u_response response;
  
  ck_assert_int_eq(ulfius_init_instance(&u_instance, 8080, NULL, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "empty", NULL, 0, &callback_function_empty, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "POST", "empty", NULL, 0, &callback_function_empty, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "PUT", "empty", NULL, 0, &callback_function_empty, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "DELETE", "empty", NULL, 0, &callback_function_empty, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "url", NULL, 0, &callback_function_return_url, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "*", "url", NULL, 0, &callback_function_return_url, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "error", NULL, 0, &callback_function_error, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "unauthorized", NULL, 0, &callback_function_unauthorized, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "check_auth", NULL, 0, &callback_function_check_auth, NULL), U_OK);
  ck_assert_int_eq(ulfius_start_framework(&u_instance), U_OK);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/nope");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 404);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/empty");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_verb = o_strdup("POST");
  request.http_url = o_strdup("http://localhost:8080/empty");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_verb = o_strdup("PUT");
  request.http_url = o_strdup("http://localhost:8080/empty");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_verb = o_strdup("DELETE");
  request.http_url = o_strdup("http://localhost:8080/empty");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_verb = o_strdup("OPTION");
  request.http_url = o_strdup("http://localhost:8080/empty");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 404);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/url");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ck_assert_int_eq(o_strncmp(response.binary_body, "/url", o_strlen("/url")), 0);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_verb = o_strdup("OPTION");
  request.http_url = o_strdup("http://localhost:8080/url");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ck_assert_int_eq(o_strncmp(response.binary_body, "/url", o_strlen("/url")), 0);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_verb = o_strdup("test");
  request.http_url = o_strdup("http://localhost:8080/url");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ck_assert_int_eq(o_strncmp(response.binary_body, "/url", o_strlen("/url")), 0);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/error");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 500);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/unauthorized");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 401);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/check_auth");
  request.auth_basic_user = o_strdup("nope");
  request.auth_basic_password = o_strdup("nope");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 401);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/check_auth");
  request.auth_basic_user = o_strdup("user");
  request.auth_basic_password = o_strdup("password");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_stop_framework(&u_instance);
  ulfius_clean_instance(&u_instance);
}
END_TEST

START_TEST(test_ulfius_endpoint_parameters)
{
  struct _u_instance u_instance;
  struct _u_request request;
  struct _u_response response;
  const char * set_cookie;
  
  ck_assert_int_eq(ulfius_init_instance(&u_instance, 8080, NULL, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "param", "/:param1/@param2/", 0, &callback_function_param, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "param", "/:param1/@param2/:param3/:param3", 0, &callback_function_param, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "POST", "param", NULL, 0, &callback_function_body_param, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "header", NULL, 0, &callback_function_header_param, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "cookie", NULL, 0, &callback_function_cookie_param, NULL), U_OK);
  ck_assert_int_eq(ulfius_start_framework(&u_instance), U_OK);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/param/value1/value2");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ck_assert_int_eq(o_strncmp(response.binary_body, "param1 is value1, param2 is value2", o_strlen("param1 is value1, param2 is value2")), 0);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/param/value1/value2/value3.1/value3.2");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ck_assert_int_eq(o_strncmp(response.binary_body, "param1 is value1, param2 is value2, param3 is value3.1,value3.2", o_strlen("param1 is value1, param2 is value2, param3 is value3.1,value3.2")), 0);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_verb = o_strdup("POST");
  request.http_url = o_strdup("http://localhost:8080/param/");
  u_map_put(request.map_post_body, "param1", "value3");
  u_map_put(request.map_post_body, "param2", "value4");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ck_assert_int_eq(o_strncmp(response.binary_body, "param1 is value3, param2 is value4", o_strlen("param1 is value3, param2 is value4")), 0);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/header");
  u_map_put(request.map_header, "param1", "value5");
  u_map_put(request.map_header, "param2", "value6");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ck_assert_int_eq(o_strncmp(response.binary_body, "param1 is value5, param2 is value6", o_strlen("param1 is value5, param2 is value6")), 0);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/cookie");
  u_map_put(request.map_cookie, "param1", "value7");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ck_assert_int_eq(o_strncmp(response.binary_body, "param1 is value7", o_strlen("param1 is value7")), 0);

  set_cookie = u_map_get(response.map_header, "Set-Cookie");
  ck_assert_ptr_ne(o_strstr(set_cookie, "param2=value_cookie; Max-Age=100; Domain=localhost; Path=/cookie; HttpOnly"), NULL);
  ck_assert_ptr_ne(o_strstr(set_cookie, "cookieSameSiteStrict=value_cookie; Max-Age=100; Domain=localhost; Path=/cookie; HttpOnly; SameSite=Strict"), NULL);
  ck_assert_ptr_ne(o_strstr(set_cookie, "cookieSameSiteLax=value_cookie; Max-Age=100; Domain=localhost; Path=/cookie; HttpOnly; SameSite=Lax"), NULL);
  ck_assert_ptr_ne(o_strstr(set_cookie, "cookieSameSiteNone=value_cookie; Max-Age=100; Domain=localhost; Path=/cookie; HttpOnly"), NULL);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_stop_framework(&u_instance);
  ulfius_clean_instance(&u_instance);
}
END_TEST

START_TEST(test_ulfius_endpoint_injection)
{
  struct _u_instance u_instance;
  struct _u_request request;
  struct _u_response response;
  
  ck_assert_int_eq(ulfius_init_instance(&u_instance, 8080, NULL, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "inject1", NULL, 0, &callback_function_empty, NULL), U_OK);
  ck_assert_int_eq(ulfius_start_framework(&u_instance), U_OK);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/inject1");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/inject2");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 404);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "inject2", NULL, 0, &callback_function_empty, NULL), U_OK);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/inject2");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ck_assert_int_eq(ulfius_remove_endpoint_by_val(&u_instance, "GET", "inject2", NULL), U_OK);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/inject2");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 404);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_stop_framework(&u_instance);
  ulfius_clean_instance(&u_instance);
}
END_TEST

START_TEST(test_ulfius_endpoint_multiple)
{
  struct _u_instance u_instance;
  struct _u_request request;
  struct _u_response response;
  
  ck_assert_int_eq(ulfius_init_instance(&u_instance, 8080, NULL, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "multiple", "*", 0, &callback_function_multiple_continue, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "multiple", "/:param1/*", 1, &callback_function_multiple_continue, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "multiple", "/:param1/:param2/*", 2, &callback_function_multiple_continue, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "multiple", "/:param1/:param2/:param3", 3, &callback_function_multiple_continue, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "multiple_complete", "*", 0, &callback_function_multiple_continue, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "multiple_complete", "/:param1/*", 1, &callback_function_multiple_complete, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "multiple_complete", "/:param1/:param2/*", 2, &callback_function_multiple_continue, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "multiple_complete", "/:param1/:param2/:param3", 3, &callback_function_multiple_continue, NULL), U_OK);
  ck_assert_int_eq(ulfius_start_framework(&u_instance), U_OK);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/multiple");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ck_assert_int_eq(o_strncmp(response.binary_body, "/multiple", o_strlen("/multiple")), 0);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/multiple/value1");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ck_assert_int_eq(o_strncmp(response.binary_body, "/multiple/value1\n/multiple/value1", o_strlen("/multiple/value1\n/multiple/value1")), 0);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/multiple/value1/value2");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ck_assert_int_eq(o_strncmp(response.binary_body, "/multiple/value1/value2\n/multiple/value1/value2\n/multiple/value1/value2", o_strlen("/multiple/value1/value2\n/multiple/value1/value2\n/multiple/value1/value2")), 0);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/multiple/value1/value2/value3");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ck_assert_int_eq(o_strncmp(response.binary_body, "/multiple/value1/value2/value3\n/multiple/value1/value2/value3\n/multiple/value1/value2/value3\n/multiple/value1/value2/value3", o_strlen("/multiple/value1/value2/value3\n/multiple/value1/value2/value3\n/multiple/value1/value2/value3\n/multiple/value1/value2/value3")), 0);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/nope/value1/value2/value3/value4");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 404);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/multiple_complete/value1/value2/value3");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ck_assert_int_eq(o_strncmp(response.binary_body, "/multiple_complete/value1/value2/value3\n/multiple_complete/value1/value2/value3", o_strlen("/multiple_complete/value1/value2/value3\n/multiple_complete/value1/value2/value3")), 0);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_stop_framework(&u_instance);
  ulfius_clean_instance(&u_instance);
}
END_TEST

START_TEST(test_ulfius_endpoint_stream)
{
  struct _u_instance u_instance;
  struct _u_request request;
  struct _u_response response;
  
  ck_assert_int_eq(ulfius_init_instance(&u_instance, 8080, NULL, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "stream", NULL, 0, &callback_function_stream, NULL), U_OK);
  ck_assert_int_eq(ulfius_start_framework(&u_instance), U_OK);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("http://localhost:8080/stream");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_streaming_request(&request, &response, my_write_body, NULL), U_OK);
  ck_assert_int_eq(response.status, 200);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_clean_instance(&u_instance);
}
END_TEST

START_TEST(test_ulfius_utf8_not_ignored)
{
  char * invalid_utf8_seq2 = msprintf("value %c%c", 0xC3, 0x28);
  char * invalid_utf8_seq3 = msprintf("value %c%c%c", 0xE2, 0x28, 0xA1);
  char * invalid_utf8_seq4 = msprintf("value %c%c%c%c", 0xF0, 0x90, 0x28, 0xBC);
  char * valid_utf8 = "valid value ȸ Ɇ  ɤ ¯\\_(ツ)_/¯";
  struct _u_instance u_instance;
  struct _u_request request;
  
  ck_assert_int_eq(ulfius_init_instance(&u_instance, 8080, NULL, NULL), U_OK);
  u_instance.check_utf8 = 0;
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "POST", "utf8", ":utf8_param1/:utf8_param_valid", 0, &callback_check_utf8_not_ignored, NULL), U_OK);
  ck_assert_int_eq(ulfius_start_framework(&u_instance), U_OK);
  
  ulfius_init_request(&request);
  request.http_url = msprintf("http://localhost:8080/utf8/%s/%s/?utf8_param3=%s&utf8_param_valid2=%s&utf8_param_empty=&utf8_param_null", invalid_utf8_seq3, valid_utf8, invalid_utf8_seq2, valid_utf8);
  request.http_verb = o_strdup("POST");
  u_map_put(request.map_header, "utf8_param", invalid_utf8_seq3);
  u_map_put(request.map_header, "utf8_param_valid", valid_utf8);
  u_map_put(request.map_post_body, "utf8_param", invalid_utf8_seq4);
  u_map_put(request.map_post_body, "utf8_param_valid", valid_utf8);
  u_map_put(request.map_post_body, "utf8_param_empty", "");
  ck_assert_int_eq(ulfius_send_http_request(&request, NULL), U_OK);
  ulfius_clean_request(&request);
  
  o_free(invalid_utf8_seq2);
  o_free(invalid_utf8_seq3);
  o_free(invalid_utf8_seq4);
  ulfius_stop_framework(&u_instance);
  ulfius_clean_instance(&u_instance);
}
END_TEST

START_TEST(test_ulfius_utf8_ignored)
{
  char * invalid_utf8_seq2 = msprintf("invalid value %c%c", 0xC3, 0x28);
  char * invalid_utf8_seq3 = msprintf("invalid value %c%c%c", 0xE2, 0x28, 0xA1);
  char * invalid_utf8_seq4 = msprintf("invalid value %c%c%c%c", 0xF0, 0x90, 0x28, 0xBC);
  char * valid_utf8 = "valid value ȸ Ɇ  ɤ ¯\\_(ツ)_/¯";
  struct _u_instance u_instance;
  struct _u_request request;
  
  ck_assert_int_eq(ulfius_init_instance(&u_instance, 8080, NULL, NULL), U_OK);
  ck_assert_int_eq(u_instance.check_utf8, 1);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "POST", "utf8", ":utf8_param1/:utf8_param_valid", 0, &callback_check_utf8_ignored, NULL), U_OK);
  ck_assert_int_eq(ulfius_start_framework(&u_instance), U_OK);
  
  ulfius_init_request(&request);
  request.http_url = msprintf("http://localhost:8080/utf8/%s/%s/?utf8_param3=%s&utf8_param_valid2=%s", invalid_utf8_seq3, valid_utf8, invalid_utf8_seq2, valid_utf8);
  request.http_verb = o_strdup("POST");
  u_map_put(request.map_header, "utf8_param", invalid_utf8_seq3);
  u_map_put(request.map_header, "utf8_param_valid", valid_utf8);
  u_map_put(request.map_post_body, "utf8_param", invalid_utf8_seq4);
  u_map_put(request.map_post_body, "utf8_param_valid", valid_utf8);
  ck_assert_int_eq(ulfius_send_http_request(&request, NULL), U_OK);
  ulfius_clean_request(&request);
  
  o_free(invalid_utf8_seq2);
  o_free(invalid_utf8_seq3);
  o_free(invalid_utf8_seq4);
  ulfius_stop_framework(&u_instance);
  ulfius_clean_instance(&u_instance);
}
END_TEST

#ifndef U_DISABLE_GNUTLS
START_TEST(test_ulfius_server_ca_trust)
{
  struct _u_instance u_instance;
  
  ck_assert_int_eq(ulfius_init_instance(&u_instance, 8080, NULL, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "stream", NULL, 0, &callback_function_stream, NULL), U_OK);
  ck_assert_int_ne(ulfius_start_secure_ca_trust_framework(&u_instance, NULL, CERT_PEM, CERT_CA), U_OK);
  ck_assert_int_ne(ulfius_start_secure_ca_trust_framework(&u_instance, CERT_KEY, NULL, CERT_CA), U_OK);
  ck_assert_int_eq(ulfius_start_secure_ca_trust_framework(&u_instance, CERT_KEY, CERT_PEM, CERT_CA), U_OK);
  
  ulfius_stop_framework(&u_instance);
  ulfius_clean_instance(&u_instance);
}
END_TEST

START_TEST(test_ulfius_client_certificate)
{
  struct _u_instance u_instance;
  struct _u_request request;
  struct _u_response response;
  
  ck_assert_int_eq(ulfius_init_instance(&u_instance, 8080, NULL, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "cert_auth", NULL, 0, &callback_auth_client_cert, NULL), U_OK);
  ck_assert_int_eq(ulfius_add_endpoint_by_val(&u_instance, "GET", "nocert_auth", NULL, 0, &callback_no_auth_client_cert, NULL), U_OK);
  ck_assert_int_eq(ulfius_start_secure_ca_trust_framework(&u_instance, CERT_KEY, CERT_PEM, CERT_CA), U_OK);

  ulfius_init_request(&request);
  request.http_url = o_strdup("https://localhost:8080/nocert_auth");
  request.check_server_certificate = 0;
  request.client_key_file = o_strdup("client.key");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("https://localhost:8080/nocert_auth");
  request.check_server_certificate = 0;
  request.client_cert_file = o_strdup("client.crt");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_init_request(&request);
  request.http_url = o_strdup("https://localhost:8080/cert_auth");
  request.check_server_certificate = 0;
  request.client_cert_file = o_strdup("client.crt");
  request.client_key_file = o_strdup("client.key");
  ulfius_init_response(&response);
  ck_assert_int_eq(ulfius_send_http_request(&request, &response), U_OK);
  ck_assert_int_eq(response.status, 200);
  ulfius_clean_request(&request);
  ulfius_clean_response(&response);
  
  ulfius_stop_framework(&u_instance);
  ulfius_clean_instance(&u_instance);
}
END_TEST
#endif

static Suite *ulfius_suite(void)
{
  Suite *s;
  TCase *tc_core;

  s = suite_create("Ulfius framework function tests");
  tc_core = tcase_create("test_ulfius_framework");
  tcase_add_test(tc_core, test_ulfius_simple_endpoint);
  tcase_add_test(tc_core, test_ulfius_endpoint_parameters);
  tcase_add_test(tc_core, test_ulfius_endpoint_injection);
  tcase_add_test(tc_core, test_ulfius_endpoint_multiple);
  tcase_add_test(tc_core, test_ulfius_endpoint_stream);
  tcase_add_test(tc_core, test_ulfius_utf8_not_ignored);
  tcase_add_test(tc_core, test_ulfius_utf8_ignored);
#ifndef U_DISABLE_GNUTLS
  tcase_add_test(tc_core, test_ulfius_server_ca_trust);
  tcase_add_test(tc_core, test_ulfius_client_certificate);
#endif
  tcase_set_timeout(tc_core, 30);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(int argc, char *argv[])
{
  int number_failed;
  Suite *s;
  SRunner *sr;
  //y_init_logs("Ulfius", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_DEBUG, NULL, "Starting Ulfius core tests");
  s = ulfius_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  
  //y_close_logs();
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
