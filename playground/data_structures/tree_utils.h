#ifndef TREE_UTILS_
#define TREE_UTILS_

template<typename NodePtrType>
NodePtrType tree_min(NodePtrType n)
{
  assert(n);
  while (n->left)
    n = n->left;
  return n;
}

template<typename NodePtrType>
NodePtrType tree_max(NodePtrType n)
{
  assert(n);
  while (n->right)
    n = n->right;
  return n;
}

#endif
