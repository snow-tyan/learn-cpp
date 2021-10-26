/**
 * 手撕一个红黑树吧
 **/

#include <assert.h>
#include <iostream>
using namespace std;

typedef enum
{
    RED = 0,
    BLACK = 1
} Color;

template <class T>
struct RBTNode
{
    T _key; // 存数据
    Color _color;
    RBTNode *_parent;
    RBTNode *_left;
    RBTNode *_right;
};

template <class T>
class RBTree
{
public:
    RBTree();
    ~RBTree();
    bool insert(const T &);
    void remove(T);
    void inorder() const { inorderPrint(_root); }
    void preorder() const { preorderPrint(_root); }
    void print() const;

private:
    RBTNode<T> *buyNode(const T & = T()); // 默认值声明一下即可，实现无需再次声明
    void destory(RBTNode<T> *);
    void leftRotate(RBTNode<T> *);             // 左旋
    void rightRotate(RBTNode<T> *);            // 右旋
    void insertFixup(RBTNode<T> *);            // 插入后向上调整
    RBTNode<T> *search(RBTNode<T> *, T) const; // 查找节点
    void replace(RBTNode<T> *, RBTNode<T> *);  // 把A节点换成B节点
    RBTNode<T> *minimum(RBTNode<T> *) const;   // 最左节点 不改变输入指针
    RBTNode<T> *maximum(RBTNode<T> *) const;
    void removeNode(RBTNode<T> *);
    void removeFixup(RBTNode<T> *);
    void inorderPrint(RBTNode<T> *) const;
    void preorderPrint(RBTNode<T> *) const;
    void print(RBTNode<T> *, T, int) const; //详细打印

private:
    RBTNode<T> *_Nil; // Nil节点，黑色不存数据
    RBTNode<T> *_root;
};

/* 左旋，对x结点左旋
 *       px                    px
 *       /                     /
 *      x                     y
 *     / \     --(左旋)-->    / \
 *    lx  y                  x  ry
 *       / \                / \
 *      ly  ry             lx  ly
 *
 * 不变   x->lx   y->ry
 * 变动   x-y 变为 x-ly
 *       y-ly 变为 y-x
 */
template <class T>
void RBTree<T>::leftRotate(RBTNode<T> *x)
{
    // 大体分为三个部分 1 对x动手 2 对x的父节点动手 3 对y动手
    // 将x的右孩子记作y
    // 1 x-y ==> x-ly
    RBTNode<T> *y = x->_right;
    x->_right = y->_left;
    if (y->_left != _Nil) // y的左孩子ly不一定有
        y->_left->_parent = x;
    // 2 x->parent ==> y->_parent
    y->_parent = x->_parent;
    if (_root == x) // x->_parent == _Nil
        _root = y;
    else if (x->_parent->_left == x)
        x->_parent->_left = y;
    else
        x->_parent->_right = y;
    // 3 y-ly ==> y-x
    y->_left = x;
    x->_parent = y;
}
/* 右旋，对y结点右旋
 *
 *            px                           px
 *           /                             /
 *          x                             y
 *         /  \      --(右旋)-->         /  \
 *        y   rx                       ly   x
 *       / \                               / \
 *      ly  ry                            ry  rx
 *
 * 只需将左旋代码中的left/right互换即可
 */
template <class T>
void RBTree<T>::rightRotate(RBTNode<T> *x)
{
    RBTNode<T> *y = x->_left;
    x->_left = y->_right;
    if (y->_right != _Nil)
        y->_right->_parent = x;

    y->_parent = x->_parent;
    if (_root == x) // x->_parent == _Nil
        _root = y;
    else if (x->_parent->_right == x)
        x->_parent->_right = y;
    else
        x->_parent->_left = y;
    y->_right = x;
    x->_parent = y;
}

template <class T>
RBTree<T>::RBTree()
    : _Nil(buyNode()), _root(_Nil)
{
    // 采坑1: 初始化表达式顺序只和类成员声明时顺序有关，和在表达式内顺序无关！！
    // _Nil = buyNode();
    // _root = _Nil;
    _Nil->_color = BLACK;
}

template <class T>
RBTree<T>::~RBTree()
{
    destory(_root);
    delete _Nil;
    _Nil = nullptr;
}

template <class T>
void RBTree<T>::destory(RBTNode<T> *_root)
{
    if (_root != _Nil)
    {
        if (_root->_left != _Nil)
        {
            destory(_root->_left);
        }
        if (_root->_right != _Nil)
        {
            destory(_root->_right);
        }
        delete _root;
        _root = nullptr;
    }
}

template <class T>
RBTNode<T> *RBTree<T>::buyNode(const T &key)
{
    RBTNode<T> *node = new RBTNode<T>();
    assert(node != nullptr);
    node->_color = RED;
    node->_left = node->_right = node->_parent = _Nil;
    node->_key = key;
    return node;
}

// 以BST方式插入
template <class T>
bool RBTree<T>::insert(const T &key)
{
    RBTNode<T> *pr = _Nil;   // pr记作父节点
    RBTNode<T> *cur = _root; // 从根节点找插入位置
    while (cur != _Nil)
    {
        if (key == cur->_key)
            return false; //已存在key相同的节点，直接返回
        pr = cur;
        if (key < cur->_key)
        {
            cur = cur->_left;
        }
        else
        {
            cur = cur->_right;
        }
    }
    //循环结束后,cur找到插入位置了,pr也指向了插入位置的父节点
    cur = buyNode(key);
    if (pr == _Nil)
    { // pr还指向_Nil说明此时是空树
        _root = cur;
        cur->_parent = _Nil;
    }
    else
    {
        if (key < pr->_key)
        {
            pr->_left = cur;
        }
        else
        {
            pr->_right = cur;
        }
        cur->_parent = pr;
    }
    insertFixup(cur);
    return true;
}

template <class T>
void RBTree<T>::insertFixup(RBTNode<T> *node)
{
    RBTNode<T> *uncle, *gparent;
    RBTNode<T> *parent = node->_parent;
    while (parent != _Nil && parent->_color == RED)
    { // 父节点是黑色直接插入即可
        gparent = node->_parent->_parent;
        if (parent == gparent->_left) // 父节点是祖父节点的左孩子
        {
            uncle = gparent->_right; // 叔叔就是右孩子咯
            // 叔叔是红色时
            // 1 祖父由黑变红 2 父亲和叔叔由红变黑
            if (uncle->_color == RED)
            {
                gparent->_color = RED;
                parent->_color = BLACK;
                uncle->_color = BLACK;
                node = gparent; // node指向祖父节点，循环向上调整
            }
            else
            { // 叔叔是黑色
                // 如果node是父亲的右孩子，先左旋变成下一种情况
                if (node == parent->_right)
                {
                    RBTNode<T> *temp = parent;
                    leftRotate(parent); // 旋转后原父亲变成了左孩子
                    parent = node;
                    node = temp;
                }
                // node是父亲的左孩子
                // 1 父亲由红变黑 2 祖父由黑变红 3 右旋
                parent->_color = BLACK;
                gparent->_color = RED;
                rightRotate(gparent);
            }
        }
        else //镜像操作
        {
            uncle = gparent->_left;
            if (uncle->_color == RED)
            {
                uncle->_color = BLACK;
                parent->_color = BLACK;
                gparent->_color = RED;
                node = gparent;
            }
            else
            {
                if (node == parent->_left)
                {
                    RBTNode<T> *temp = parent;
                    rightRotate(parent);
                    parent = node;
                    node = temp;
                }
                parent->_color = BLACK;
                gparent->_color = RED;
                leftRotate(gparent);
            }
        }
        parent = node->_parent;
    }
    _root->_color = BLACK;
}

template <class T>
RBTNode<T> *RBTree<T>::search(RBTNode<T> *root, T key) const
{
    if (root == _Nil)
        return _Nil;
    if (root->_key == key)
        return root;
    if (key < root->_key)
        return search(root->_left, key);
    else
        return search(root->_right, key);
}

template <class T>
RBTNode<T> *RBTree<T>::minimum(RBTNode<T> *root) const
{
    RBTNode<T> *cur = root;
    while (cur->_left != _Nil)
    {
        cur = cur->_left;
    }
    return cur;
}
template <class T>
RBTNode<T> *RBTree<T>::maximum(RBTNode<T> *root) const
{
    if (root->_right == _Nil)
    {
        return root;
    }
    return maximum(root->_right);
}

// 删除节点
// 1 找到要删除的节点
// 2 按BST方式删除（删除后依然是BST）
// 3 RBTree调整
template <class T>
void RBTree<T>::remove(T key)
{
    RBTNode<T> *node = search(_root, key);
    if (node != _Nil)
        removeNode(node);
    else
        cout << "Key Error, Not Exist" << endl;
}
/** 复习一下BST的删除
 *  if node无左孩子  右孩子直接replace
 *  if node无右孩子  左孩子直接replace
 *  node既有左孩子，也有右孩子，这时候有两种方法任选其一：
 *      1 整个左子树 成为 右子树最左节点的左孩子，node右孩子replace
 *      2 整个右子树 成为 左子树最右节点的右孩子，node左孩子replace
 * */
template <class T>
void RBTree<T>::replace(RBTNode<T> *a, RBTNode<T> *b) // b替代a，a被干掉
{
    if (a->_parent == _Nil)
        _root = b;
    else if (a == a->_parent->_left)
        a->_parent->_left = b;
    else
        a->_parent->_right = b;
    b->_parent = a->_parent;
}

template <class T>
void RBTree<T>::removeNode(RBTNode<T> *node)
{
    RBTNode<T> *fix = _Nil; //需要调整的节点
    RBTNode<T> *leftmost = node;
    Color color = node->_color;
    if (node->_left == _Nil)
    {
        fix = node->_right;
        replace(node, node->_right);
    }
    else if (node->_right == _Nil)
    {
        fix = node->_left;
        replace(node, node->_left);
    }
    else
    {
        leftmost = minimum(node->_right); // 右子树最左节点
        color = leftmost->_color;
        fix = leftmost->_right;

        if (leftmost->_parent == node)
            fix->_parent = leftmost;
        else
        {
            replace(leftmost, leftmost->_right);
            leftmost->_right = node->_right;
            leftmost->_right->_parent = leftmost;
        }
        replace(node, leftmost);
        leftmost->_left = node->_left;
        node->_left->_parent = leftmost;
        leftmost->_color = node->_color;
    }
    if (color == BLACK)
        removeFixup(fix);
    delete node;
    node = nullptr;
}
template <class T>
void RBTree<T>::removeFixup(RBTNode<T> *node)
{
    while (node != _root && node->_color == BLACK)
    {
        if (node == node->_parent->_left)
        {
            RBTNode<T> *brother = node->_parent->_right;
            if (brother->_color == RED)
            { // case 1 兄弟是红
                brother->_color = BLACK;
                node->_parent->_color = RED;
                leftRotate(node->_parent);
                brother = node->_parent->_right;
            }
            // case 2
            if (brother->_left->_color == BLACK && brother->_right->_color == BLACK)
            {
                brother->_color = RED;
                node = node->_parent;
            }
            else
            {
                // case 3
                if (brother->_right->_color == BLACK)
                {
                    brother->_color = RED;
                    brother->_left->_color = BLACK;
                    rightRotate(brother);
                    brother = node->_parent->_right;
                }
                // case 4
                brother->_color = brother->_parent->_color;
                brother->_parent->_color = BLACK;
                brother->_right->_color = BLACK;
                leftRotate(node->_parent);
                node = _root; //结束循环
            }
        }
        else //镜像
        {
            RBTNode<T> *brother = node->_parent->_left;
            if (brother->_color == RED)
            { // case 1 兄弟是红
                brother->_color = BLACK;
                node->_parent->_color = RED;
                rightRotate(node->_parent);
                brother = node->_parent->_left;
            }
            // case 2
            if (brother->_right->_color == BLACK && brother->_left->_color == BLACK)
            {
                brother->_color = RED;
                node = node->_parent;
            }
            else
            {
                // case 3
                if (brother->_left->_color == BLACK)
                {
                    brother->_color = RED;
                    brother->_right->_color = BLACK;
                    leftRotate(brother);
                    brother = node->_parent->_left;
                }
                // case 4
                brother->_color = brother->_parent->_color;
                brother->_parent->_color = BLACK;
                brother->_left->_color = BLACK;
                rightRotate(node->_parent);
                node = _root; //结束循环
            }
        }
    }
    node->_color = BLACK;
}

template <class T>
void RBTree<T>::inorderPrint(RBTNode<T> *node) const
{
    if (node == _Nil)
        return;
    if (node->_left != _Nil)
        inorderPrint(node->_left);
    cout << node->_key << "("
         << ((node->_color == BLACK) ? "BLACK" : "RED")
         << ")"
         << " ";
    if (node->_right != _Nil)
        inorderPrint(node->_right);
}

template <class T>
void RBTree<T>::preorderPrint(RBTNode<T> *node) const
{
    if (node == _Nil)
        return;
    cout << node->_key << "("
         << ((node->_color == BLACK) ? "BLACK" : "RED")
         << ")"
         << " ";
    if (node->_left != _Nil)
        preorderPrint(node->_left);
    if (node->_right != _Nil)
        preorderPrint(node->_right);
}
template <class T>
void RBTree<T>::print() const
{
    if (_root != _Nil)
        print(_root, _root->_key, 0);
}

template <class T>
void RBTree<T>::print(RBTNode<T> *node, T key, int direction) const
{
    if (node != _Nil)
    {
        if (direction == 0)
        {
            cout << node->_key << "("
                 << (node->_color == BLACK ? "BLACK" : "RED")
                 << ") is root" << endl;
        }
        else
        {
            cout << node->_key << "("
                 << (node->_color == BLACK ? "BLACK" : "RED")
                 << ") is " << key << "'s "
                 << (direction == 1 ? "right" : "left") << "child" << endl;
        }
        print(node->_left, node->_key, -1);
        print(node->_right, node->_key, 1);
    }
}

void test0()
{
    RBTree<int> rbt;
    int arr[] = {10, 7, 8, 15, 5, 6, 11, 13, 12};
    for (int i = 0; i < 9; ++i)
    {
        rbt.insert(arr[i]);
    }
    // cout << "前序遍历：";
    // rbt.preorder();
    // cout << endl;
    cout << "中序遍历：";
    rbt.inorder();
    cout << endl;
    rbt.print();
    cout << "---------After insert 4---------" << endl;
    rbt.insert(4);
    // cout << "前序遍历：";
    // rbt.preorder();
    // cout << endl;
    cout << "中序遍历：";
    rbt.inorder();
    cout << endl;
    rbt.print();
    cout << "---------After delete 8---------" << endl;
    rbt.remove(8);
    // cout << "前序遍历：";
    // rbt.preorder();
    // cout << endl;
    cout << "中序遍历：";
    rbt.inorder();
    cout << endl;
    rbt.print();
}

int main()
{
    test0();
    return 0;
}