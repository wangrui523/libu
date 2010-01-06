#include <u/libu.h>

U_TEST_MODULE(uri);

/* shall match struct u_uri_s */
typedef struct 
{
    const char *scheme, *user, *pwd, *host, *port, *path;
} u_uri_exp_t;

static int test_u_uri_parse (void)
{
    struct vt_s
    {
        const char *in;
        u_uri_exp_t exp;
    } vt[] = {
        { 
            "tcp4://www.kame.net:http/index.html",
            {
                .scheme = "tcp4",
                .user = NULL,
                .pwd = NULL,
                .host = "www.kame.net",
                .port = "http",
                .path = "/index.html"
            }
        },
        { 
            "http://wiki.koanlogic.com/doku.php?id=libu",
            {
                .scheme = "http",
                .user = NULL,
                .pwd = NULL,
                .host = "wiki.koanlogic.com",
                .port = NULL,
                .path = "/doku.php?id=libu"
            }
        },
        { 
            "http://[2001:200::8002:203:47ff:fea5:3085]:80/index.html",
            {
                .scheme = "http",
                .user = NULL,
                .pwd = NULL,
                .host = "2001:200::8002:203:47ff:fea5:3085",
                .port = "80",
                .path = "/index.html"
            }
        },
        { 
            NULL,
            {
                .scheme = NULL,
                .user = NULL,
                .pwd = NULL,
                .host = NULL,
                .port = NULL,
                .path = NULL
            } 
        }
    };

    int i;
    u_uri_t *u = NULL;

#define CHECK_EXP_MSG(field)                                                \
    do {                                                                    \
        if (((vt[i].exp.field == NULL) ?                                    \
                (u_uri_##field(u) != NULL) :                                \
                strcmp(u_uri_##field(u), vt[i].exp.field)))                 \
        {                                                                   \
            con("%s != %s at idx %d", u_uri_##field(u), vt[i].exp.field, i);\
            goto err;                                                       \
        }                                                                   \
    } while (0)

    for (i = 0; vt[i].in; i++)
    {
        con_err_if (u_uri_parse(vt[i].in, &u));

        CHECK_EXP_MSG(scheme);
        CHECK_EXP_MSG(user);
        CHECK_EXP_MSG(pwd);
        CHECK_EXP_MSG(host);
        CHECK_EXP_MSG(port);
        CHECK_EXP_MSG(path);

        u_uri_free(u), u = NULL;
    }

    return 0;
err:
    u_uri_free(u);
    return ~0;
}

U_TEST_MODULE(uri)
{
    U_TEST_RUN( test_u_uri_parse );

    return 0;                                                
}