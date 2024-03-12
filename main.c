#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

#define NUM_OF_NODES 10

typedef enum test_result {PASSED, FAILED} test_result;
AVLNodePtr tree_with_num(int num,int start);
test_result insert_test();
test_result search_test();
test_result delete_test();
test_result LCA_test();
test_result merge_test();
test_result sum_test();

int main()
{

    printf("Insert test... %s",insert_test()==PASSED?"PASSED\n":"FAILED\n");
    printf("Search test... %s",search_test()==PASSED?"PASSED\n":"FAILED\n");
    printf("Delete test... %s",delete_test()==PASSED?"PASSED\n":"FAILED\n");
    printf("LCA test...... %s",LCA_test()==PASSED?"PASSED\n":"FAILED\n");
    printf("Merge test.... %s",merge_test()==PASSED?"PASSED\n":"FAILED\n");
    printf("Sum test...... %s",sum_test()==PASSED?"PASSED\n":"FAILED\n");


}
AVLNodePtr tree_with_num(int num,int start)
{
    AVLNodePtr root=NULL;
    for(int i=start;i<num+start;i++)
    {
       root = avl_insert(root,i);
    }
    return root;
}
test_result insert_test()
{
    AVLNodePtr root = tree_with_num(NUM_OF_NODES,0);
    avl_delete_all(root);
    return PASSED;
}
test_result search_test()
{
    AVLNodePtr root = tree_with_num(NUM_OF_NODES,0);
    for(int i=0;i<NUM_OF_NODES+1;i++)
    {
        AVLNodePtr result;
        result = avl_search(root,i);
        if(!result)
        {
            if(i==NUM_OF_NODES)
            {
                avl_delete_all(root);
                return PASSED;
            }
            avl_delete_all(root);
            return FAILED;
        }
    }
    avl_delete_all(root);
    return FAILED;
}
test_result delete_test()
{
    AVLNodePtr root = tree_with_num(NUM_OF_NODES,0);
    root = avl_delete(root,1);
    root = avl_delete(root,5);
    for(int i=0;i<NUM_OF_NODES;i++)
    {
        AVLNodePtr result;
        result = avl_search(root,i);
        if(!result)
        {
            if(i!=1 && i!=5){
                avl_delete_all(root);
                return FAILED;
            }
        }
    }
    avl_delete_all(root);
    return PASSED;
}
test_result LCA_test()
{
    AVLNodePtr root = tree_with_num(NUM_OF_NODES,0);
    if(avl_LCA(root,0,9)->key!=3)
    {
        avl_delete_all(root);
        return FAILED;
    }
    if(avl_LCA(root,0,2)->key!=1)
    {
        avl_delete_all(root);
        return FAILED;
    }
    if(avl_LCA(root,4,9)->key!=7)
    {
        avl_delete_all(root);
        return FAILED;
    }
    avl_delete_all(root);
    return PASSED;
}
test_result merge_test()
{
    AVLNodePtr root1 = tree_with_num(NUM_OF_NODES,0);
    AVLNodePtr root2 = tree_with_num(NUM_OF_NODES,5);
    AVLNodePtr merged = avl_merge(root1,root2);
    if(merged->tree_size!=15)
    {
        avl_delete_all(root1);
        avl_delete_all(root2);
        avl_delete_all(merged);
        return FAILED;
    }
    if(merged->tree_sum!=105)
    {
        avl_delete_all(root1);
        avl_delete_all(root2);
        avl_delete_all(merged);
        return FAILED;
    }
    avl_delete_all(root1);
    avl_delete_all(root2);
    avl_delete_all(merged);
    return PASSED;
}
test_result sum_test()
{
    AVLNodePtr root = tree_with_num(NUM_OF_NODES,0);
    if(avl_sum(root,0,5)!=15)
    {
        avl_delete_all(root);
        return FAILED;
    }
    if(avl_sum(root,5,10)!=35)
    {
        avl_delete_all(root);
        return FAILED;
    }
    avl_delete_all(root);
    root = tree_with_num(NUM_OF_NODES*NUM_OF_NODES,1);
    for(int i=0;i<NUM_OF_NODES*NUM_OF_NODES;i+=3)
    {
        root = avl_delete(root,i);
    }
    if(avl_sum(root,20,50)!=740)
    {
        avl_delete_all(root);
        return FAILED;
    }
    avl_delete_all(root);
    return PASSED;
}