#ifndef BST_UTILS_
#define BST_UTILS_

template<typename NodePtrType>
NodePtrType bst_min(NodePtrType n)
{
  assert(n);
  while (n->left)
    n = n->left;
  return n;
}

template<typename NodePtrType>
NodePtrType bst_max(NodePtrType n)
{
  assert(n);
  while (n->right)
    n = n->right;
  return n;
}

#endif
