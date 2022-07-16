#include <stdbool.h>

// トークンの種類
typedef enum {
    TK_RESERVED,  // 記号
    TK_NUM,       // 整数トークン
    TK_EOF,       // 入力の終わりを表すトークン
} Tokenkind;

typedef struct Token Token;

// トークン型
struct Token {
    Tokenkind kind;  // トークンの型
    Token *next;     // 次の入力トークン
    int val;         // kindがTK_NUMのとき、その数値
    char *str;       // トークン文字列
    int len;         // トークンの長さ
};

// 抽象構文木のノードの種類
typedef enum {
    ND_ADD,  // +
    ND_SUB,  // -
    ND_MUL,  // *
    ND_DIV,  // /
    ND_EQ,   // ==
    ND_NE,   // !=
    ND_LT,   // <
    ND_LE,   // <=
    ND_NUM,  // 整数
} NodeKind;

typedef struct Node Node;

// 抽象構文木のノードの型
struct Node {
    NodeKind kind;  // ノードの型
    Node *lhs;      // 左辺
    Node *rhs;      // 右辺
    int val;        // kindがND_NUMの場合のみ使う
};

// 現在着目しているトークン
extern Token *token;

// 入力プログラム
extern char *user_input;

void error_at(char *loc, char *fmt, ...);
void error(char *fmt, ...);
bool consume(char *op);
void expect(char *op);
int expect_number();
bool at_eof();
Token *new_token(Tokenkind kind, Token *cur, char *str, int len);
Token *tokenize(char *p);

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *expr();

void gen(Node *node);