#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "9cc.h"

//
// Parser
//

// ローカル変数
LVar *locals;

Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

// 変数を名前で検索する
// 見つからなかった場合はNULLを返す
LVar *find_lvar(Token *tok) {
    for (LVar *var = locals; var; var = var->next) {
        if (var->len == tok->len && !memcmp(tok->str, var->name, var->len))
            return var;
    }
    return NULL;
}

// 新しいノードの作成
Node *new_node(NodeKind kind, ...) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    va_list ap;
    va_start(ap, kind);
    int loop = 2;
    switch (kind) {
    case ND_RETURN:
        loop = 1;
        break;
    case ND_IF:
        loop = 3;
        break;
    case ND_FOR:
        loop = 4;
        break;
    }
    for (int i = 0; i < loop; i++) {
        node->children[i] = va_arg(ap, Node*);
    }
    va_end(ap);
    return node;
}

// numノードの作成
Node *new_node_num(int val) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

// program = stmt*
void program() {
    int i = 0;
    while (!at_eof())
        code[i++] = stmt();
    code[i] = NULL;
}

// stmt = expr ";"
//      | "return" expr ";"
//      | "if" "(" expr ")" stmt ("else" stmt)?
//      | "while" "(" expr ")" stmt
//      | "for" "(" expr? ";" expr? ";" expr? ")" stmt
Node *stmt() {
    Node *node;

    if (consume_token(TK_RETURN)) {
        node = new_node(ND_RETURN, expr());
        expect(";");
    } else if (consume_token(TK_IF)) {
        expect("(");
        Node *node1 = expr();
        expect(")");
        Node *node2 = stmt();
        if (consume_token(TK_ELSE))
            node = new_node(ND_IF, node1, node2, stmt());
        else
            node = new_node(ND_IF, node1, node2, NULL);
    } else if (consume_token(TK_WHILE)) {
        expect("(");
        node = expr();
        expect(")");
        node = new_node(ND_WHILE, node, stmt());
    } else if (consume_token(TK_FOR)) {
        expect("(");
        Node *node1 = NULL;
        Node *node2 = NULL;
        Node *node3 = NULL;
        if (!consume(";")) {
            node1 = expr();
            expect(";");
        }
        if (!consume(";")) {
            node2 = expr();
            expect(";");
        }
        if (!consume(")")) {
            node3 = expr();
            expect(")");
        }
        node = new_node(ND_FOR, node1, node2, node3, stmt());
    } else {
        node = expr();
        expect(";");
    }

    return node;
}

// expr = assign
Node *expr() {
    return assign();
}

// assign = equality ("=" assign)?
Node *assign() {
    Node *node = equality();

    if (consume("="))
        node = new_node(ND_ASSIGN, node, assign());
    return node;
}

// equality = relational ("==" relational | "!=" relational)*
Node *equality() {
    Node *node = relational();

    for (;;) {
        if (consume("=="))
            node = new_node(ND_EQ, node, relational());
        else if (consume("!="))
            node = new_node(ND_NE, node, relational());
        else
            return node;
    }
}

// relational = add ("<" add | "<= add" | ">" add | ">=" add)*
Node *relational() {
    Node *node = add();

    for (;;) {
        if (consume("<"))
            node = new_node(ND_LT, node, add());
        else if (consume("<="))
            node = new_node(ND_LE, node, add());
        else if (consume(">"))
            node = new_node(ND_LT, add(), node);
        else if (consume(">="))
            node = new_node(ND_LE, add(), node);
        else
            return node;
    }
}

// add = mul ("+" mul | "-" mul)*
Node *add() {
    Node *node = mul();

    for (;;) {
        if (consume("+"))
            node = new_node(ND_ADD, node, mul());
        else if (consume("-"))
            node = new_node(ND_SUB, node, mul());
        else
            return node;
    }
}

// mul = unary ("*" unary | "/" unary)*
Node *mul() {
    Node *node = unary();

    for (;;) {
        if (consume("*"))
            node = new_node(ND_MUL, node, unary());
        else if (consume("/"))
            node = new_node(ND_DIV, node, unary());
        else
            return node;
    }
}

// unary = ("+" | "-")? primary
Node *unary() {
    if (consume("+"))
        return primary();
    else if (consume("-"))
        return new_node(ND_SUB, new_node_num(0), primary());
    return primary();
}

// primary = num | ident | "(" expr ")"
Node *primary() {
    // 次のトークンが"("なら、"(" expr ")"のはず
    if (consume("(")) {
        Node *node = expr();
        expect(")");
        return node;
    }

    // ident
    Token *tok = consume_ident();
    if (tok) {
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_LVAR;
        
        LVar *lvar = find_lvar(tok);
        if (lvar) {
            node->offset = lvar->offset;
        } else {
            lvar = calloc(1, sizeof(LVar));
            lvar->next = locals;
            lvar->name = tok->str;
            lvar->len = tok->len;
            //printf("OK!!!!\n");
            if (locals)
                lvar->offset = locals->offset + 8;
            else
                lvar->offset = 8;
            node->offset = lvar->offset;
            locals = lvar;
        }
        return node;
    }

    // そうでなければ数値のはず
    return new_node_num(expect_number());
}