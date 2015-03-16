//
//  main.cpp
//  btree
//
//  Created by oskimura on 2015/03/15.
//  Copyright (c) 2015年 oskimura. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <vector>
#include <set>

using namespace std;

int t;

struct node {
public:
    vector<int> keys;
    bool is_leaf;
    vector<node*> nodes;
    int n;
};

struct btree {
public:
    node *root;
};


static set<node*> storage;
void disk_write(node *x)
{
    storage.insert(x);
}

void disk_read(node *x)
{
    
}

void btree_create(btree& T)
{
    node *x = new node;
    x->is_leaf = true;
    x->n = 0;
    x->keys.resize(2*t-1);
    x->nodes.resize(2*t);
    
    disk_write(x);
    T.root = x;
}

node* btree_search(node* x, int k)
{
    int i = 0;
    
    while (i < x->n && k > x->keys[i]) {
        i++;
    }
    if (i < x->n && k == x->keys[i]) {
        return x;
    }
    else if (x->is_leaf) {
        return NULL;
    }
    else {
        disk_read(x->nodes[i]);
        return btree_search(x->nodes[i],k);
    }
}


void
btree_split_child(node *x, int i)
{
    node *z = new node;
    z->nodes.resize(2*t);
    z->keys.resize(2*t-1);
    node *y = x->nodes[i];
    
    z->is_leaf = y->is_leaf;
    z->n = t - 1;
    
    for (int j=0; j<t-1;j++) {
        z->keys[j] = y->keys[j+t];
    }
    
    if (!y->is_leaf) {
        for (int j=0; j<t;j++) {
            z->nodes[j] = y->nodes[j+t];
        }
    }
    
    
    y->n = t-1;
    for (int j= x->n; j>=i+1 ;j--) {
        x->nodes[j+1] = x->nodes[j];
    }
    x->nodes[i+1] = z;
    
    for (int j = x->n-1; j>=i; j--) {
        x->keys[j+1] = x->keys[j];
    }
    x->keys[i] = y->keys[t-1];
    x->n++;
    disk_write(y);
    disk_write(z);
    disk_write(x);
}

void btree_insert_notfull(node* x, int k)
{
    int i = x->n-1;
    if (x->is_leaf) {
        while (i>=0 && k < x->keys[i]) {
            x->keys[i+1] = x->keys[i];
            i--;
        }
        x->keys[i+1] = k;
        x->n++;
        disk_write(x);
    }
    else {
        while (i>=0 && k < x->keys[i]) {
            i--;
        }
        i++;
        disk_read(x->nodes[i]);
        if (x->nodes[i]->n == 2*t-1) {
            btree_split_child(x,i);
            if (k > x->keys[i]) {
                i++;
            }
        }
        btree_insert_notfull(x->nodes[i],k);
    }
}

void
btree_insert(btree *T, int k)
{
    node *r = T->root;
    if (r->n == 2*t-1) {
        node *s = new node;
        T->root = s;
        s->n=0;
        s->is_leaf=false;
        
        s->nodes.resize(2*t);
        s->keys.resize(2*t-1);
        s->nodes[0] = r;
        
        
        
        btree_split_child(s,0);
        btree_insert_notfull(s, k);
    }
    else {
        printf("tree_insert_notfull\n");
        btree_insert_notfull(r,k);
    }
}



int main(int argc, const char * argv[])
{

    printf("start\n");
    t = 2;
    btree T;
    printf("create\n");
    btree_create(T);
    printf("insert\n");
    btree_insert(&T,'F');
    btree_insert(&T,'S');
    btree_insert(&T,'Q');
    btree_insert(&T,'K');
    btree_insert(&T,'C');
    btree_insert(&T,'L');
    btree_insert(&T,'H');
    btree_insert(&T,'T');
    btree_insert(&T,'V');
    btree_insert(&T,'W');
    btree_insert(&T,'M');
    btree_insert(&T,'R');
    btree_insert(&T,'N');
    btree_insert(&T,'P');
    btree_insert(&T,'A');
    btree_insert(&T,'B');
    btree_insert(&T,'X');
    btree_insert(&T,'Y');
    btree_insert(&T,'Z');
    btree_insert(&T,'E');
    printf("end\n");
    return 0;
}

