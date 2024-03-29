#include <stdio.h>
#include "9cc.h"


//
//  code generator
//

int label_num;

void gen_lval(Node *node) {
    if (node->kind != ND_LVAR)
        error("代入の左辺値が変数ではありません");
    
    printf("    mov rax, rbp\n");
    printf("    sub rax, %d\n", node->offset);
    printf("    push rax\n");
}

void gen(Node *node) {
    if (!node)
        return;

    switch (node->kind) {
    case ND_NUM:
        printf("    push %d\n", node->val);
        return;
    case ND_LVAR:
        gen_lval(node);
        printf("    pop rax\n");
        printf("    mov rax, [rax]\n");
        printf("    push rax\n");
        return;
    case ND_ASSIGN:
        gen_lval(node->children[0]);
        gen(node->children[1]);

        printf("    pop rdi\n");
        printf("    pop rax\n");
        printf("    mov [rax], rdi\n");
        printf("    push rdi\n");
        return;
    case ND_RETURN:
        gen(node->children[0]);
        printf("    pop rax\n");
        printf("    mov rsp, rbp\n");
        printf("    pop rbp\n");
        printf("    ret\n");
        return;
    case ND_IF:
        int num = label_num++;
        gen(node->children[0]);
        printf("    pop rax\n");
        printf("    cmp rax, 0\n");
        if (node->children[2]) {
            printf("    je .Lelse%d\n", num);
            gen(node->children[1]);
            printf("    je .Lend%d\n", num);
            printf(".Lelse%d:\n", num);
            gen(node->children[2]);
        } else {
            printf("    je .Lend%d\n", num);
            gen(node->children[1]);
        }
        printf(".Lend%d:\n", num);
        return;
    case ND_WHILE:
        num = label_num++;
        printf(".Lstart%d:\n", num);
        gen(node->children[0]);
        printf("    pop rax\n");
        printf("    cmp rax, 0\n");
        printf("    je .Lend%d\n", num);
        gen(node->children[1]);
        printf("    jmp .Lstart%d\n", num);
        printf(".Lend%d:\n", num);
        return;
    case ND_FOR:
        num = label_num++;
        gen(node->children[0]);
        printf(".Lstart%d:\n", num);
        gen(node->children[1]);
        printf("    pop rax\n");
        printf("    cmp rax, 0\n");
        printf("    je .Lend%d\n", num);
        gen(node->children[3]);
        gen(node->children[2]);
        printf("    jmp .Lstart%d\n", num);
        printf(".Lend%d:\n", num);
        return;
    case ND_BLOCK:
        for (int i = 0; i < node->child_num; i++) {
            gen(node->children[i]);
        }
        return;
    default:
        break;
    }

    gen(node->children[0]);
    gen(node->children[1]);

    printf("    pop rdi\n");
    printf("    pop rax\n");

    switch (node->kind) {
    case ND_ADD:
        printf("    add rax, rdi\n");
        break;
    case ND_SUB:
        printf("    sub rax, rdi\n");
        break;
    case ND_MUL:
        printf("    imul rax, rdi\n");
        break;
    case ND_DIV:
        printf("    cqo\n");
        printf("    idiv rdi\n");
        break;
    case ND_EQ:
        printf("    cmp rax, rdi\n");
        printf("    sete al\n");
        printf("    movzb rax, al\n");
        break;
    case ND_NE:
        printf("    cmp rax, rdi\n");
        printf("    setne al\n");
        printf("    movzb rax, al\n");
        break;
    case ND_LT:
        printf("    cmp rax, rdi\n");
        printf("    setl al\n");
        printf("    movzb rax, al\n");
        break;
    case ND_LE:
        printf("    cmp rax, rdi\n");
        printf("    setle al\n");
        printf("    movzb rax, al\n");
        break;
    default:
        break;
    }

    printf("    push rax\n");
}