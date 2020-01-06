/*
 * Author: Yunho Cho
 * Date: 2019-05-14
 * Project:
 * Purpose: 
 * Notes:
 */

#ifndef BPLUSTREE_BPLUSTREE_H
#define BPLUSTREE_BPLUSTREE_H

#include "Functions.h"


using namespace std;

template <class T>

class BPlusTree
{
public:

//CTOR
BPlusTree(bool dups = true);

//big three:
BPlusTree(const BPlusTree<T>& other);
~BPlusTree();
BPlusTree<T>& operator =(const BPlusTree<T>& RHS);

void insert(const T& entry);                                  //insert entry into the tree
void remove(const T& entry);                                  //remove entry from the tree

void clear_tree();                                            //clear this object (delete all nodes etc.)
void copy_tree(const BPlusTree<T>& other);                    //copy other into this object
//void copy_tree(const BPlusTree<T>& other, stack<BPlusTree<T>*>& s);
bool contains(const T& entry) const;                          //true if entry can be found in the array

//T& get(const T& entry);                                     //return a reference to entry in the tree
//const T& get(const T& entry)const;                          //return a reference to entry in the tree
//T& get_existing(const T& entry);                            //return a reference to entry in the tree


//int size() const;                                           //count the number of elements in the tree
//bool empty() const;                                         //true if the tree is empty

void print_tree(int level = 0,ostream &outs=cout) const;      //print a readable version of the tree
friend ostream& operator<<(ostream& outs, const BPlusTree<T>& print_me)
{
    print_me.print_tree(0, outs);
    return outs;
}

//bool is_valid();

private:

static const int MINIMUM = 1;
static const int MAXIMUM = 2 * MINIMUM;

bool dups_ok{};                                           //true if duplicate keys may be inserted
int data_count{};                                         //number of data elements
T data[MAXIMUM + 1];                                    //holds the keys
int child_count{};                                        //number of children
BPlusTree* subset[MAXIMUM + 2];                         //subtrees
BPlusTree* next;
bool is_leaf() const {return child_count==0;}           //true if this is a leaf node

//T* find_ptr(const T& entry);                          //return a pointer to this key. NULL if not there.

//insert element functions
void loose_insert(const T& entry);                //allows MAXIMUM+1 data elements in the root
void insert_entry(int idx, T entry);
void fix_excess(int idx);                         //fix excess of data elements in child i
void insert_subset(int idx);

//remove element functions:
void loose_remove(const T& entry);                //allows MINIMUM-1 data elements in the root
void delete_entry(int idx);

void fix_shortage(int idx);              //fix shortage of data elements in child i
                                                  //and return the smallest key in this subtree
//BPlusTree<T>* fix_shortage(int idx);
//BPlusTree<T>* get_smallest_node();
T get_smallest(int idx);                    //entry := leftmost leaf
//void get_biggest(T& entry);                     //entry := rightmost leaf
//void remove_biggest(T& entry);                  //remove the biggest child of this tree->entry
void transfer_left(int idx);                      //transfer one element LEFT from child i
void transfer_right(int idx);                     //transfer one element RIGHT from child i
//BPlusTree<T> *merge_with_next_subset(int i);    //merge subset i with subset i+1

};

/*
 * =====================================================================================================================
 * PUBLIC DEFINITIONS
 * =====================================================================================================================
 */

template<class T>
BPlusTree<T>::BPlusTree(bool dups)
{
    dups_ok = dups;
    data_count = 0;
    child_count = 0;
//    for(int j=0; j<MAXIMUM+1; j++)
//    {
//        data[j] = NULL;
//    }

    for(int i=0; i<MAXIMUM+2; i++)
    {
        subset[i] = nullptr;
    }

    next = nullptr;
}

template<class T>
BPlusTree<T>::~BPlusTree()
{
    clear_tree();
}

template<class T>
BPlusTree<T>::BPlusTree(const BPlusTree<T> &other)
{
    dups_ok = other.dups_ok;
    data_count = 0;
    child_count = 0;
//    for(int j=0; j<MAXIMUM+1; j++)
//    {
//        data[j] = NULL;
//    }

    for(int i=0; i<MAXIMUM+2; i++)
    {
        subset[i] = nullptr;
    }

    next = nullptr;
    copy_tree(other);
}

template<class T>
BPlusTree<T> &BPlusTree<T>::operator=(const BPlusTree<T> &RHS)
{
    if(this == &RHS)
    {
        return *this;
    }

    clear_tree();
    copy_tree(RHS);

    cout << "Assignment operator fired"<<endl;
    return *this;
}

template<class T>
void BPlusTree<T>::insert(const T &entry)
{

    loose_insert(entry);

    // When root has too many data.
    if(data_count>MAXIMUM)
    {
        BPlusTree<T> *newtree = new BPlusTree<T>(*this);
        clear_tree();
        subset[0]=newtree;
        child_count++;
        fix_excess(0);
    }
}

template<class T>
void BPlusTree<T>::remove(const T &entry)
{
    loose_remove(entry);

    // if root is short of data
    if(data_count<MINIMUM)
    {

    }
    //fix top internal node
    for(int i=0; i< data_count; i++)
    {
        data[i] = get_smallest(i+1);
    }
}

template<class T>
void BPlusTree<T>::clear_tree()
{
    if(!is_leaf())
    {
        for(int i=child_count-1; i>=0; i--)
        {
            subset[i]->clear_tree();
            delete subset[i];
            subset[i] = nullptr;
        }
    }

//    for(int j=0; j<MAXIMUM+1; j++)
//    {
//        data[j] = NULL;
//    }

    next = nullptr;
    data_count = 0;
    child_count = 0;
}

template<class T>
void BPlusTree<T>::copy_tree(const BPlusTree<T> &other)
{
    //Copy contents from the source
    data_count = other.data_count;
    child_count = other.child_count;
    copy_arr(other.data,data,MAXIMUM+1);

    //If current BTree is not a leaf
    if(!other.is_leaf())
    {
        for(int i=other.child_count-1; i>=0; i--) // Set index to last index of the subset i=(other.child_count)-1
        {
            subset[i] = new BPlusTree<T>;
            subset[i]->copy_tree(*other.subset[i]);

            // Assign next if the subset is leaf
            if(subset[i]->is_leaf() && i<MAXIMUM+2)
            {
                subset[i]->next = subset[i+1];
            }
        }
    }
}

template<class T>
bool BPlusTree<T>::contains(const T &entry) const
{
    int idx = get_idx(data, data_count, entry);
    bool found = (idx<data_count && data[idx] == entry);

    // If target is found
    if(found)
    {
        // if target found is in the leaf
        if(is_leaf())
        {
            return found;
        }
        // if target found is not in the leaf
        else
        {
            return subset[idx+1]->contains(entry);
        }
    }

    // if target is not found
    else
    {
        // if target is n ot found and is leaf
        if(is_leaf())
        {
            cout << entry << " is not found in the tree.";
            return found;
        }
        // if target is not found and is n ot leaf
        else
        {
            return subset[idx]->contains(entry);
        }
    }
}

template<class T>
void BPlusTree<T>::print_tree(int level, ostream &outs) const
{
    if(!is_leaf())
    {
        int datas = data_count-1;
        for(int i=child_count-1; i>=0; i--)
        {
            subset[i]->print_tree(level+1);
            if(datas >=0)
            {
                cout << setw(level*8);
                cout << "[" << data[datas] << "]" << endl;
                datas --;
            }
        }
    }
    else
    {
        cout << setw(level*8);
        for(int j=0; j<data_count; j++)
        {
            cout << "["<< data[j] << "]";
        }
        cout << endl;
    }
}

/*
 * =====================================================================================================================
 * PRIVATE DEFINITIONS
 * =====================================================================================================================
 */

template<class T>
void BPlusTree<T>::loose_insert(const T &entry)
{
    int idx = get_idx(data, data_count, entry);
    bool found = (idx<data_count && data[idx] == entry);

    if(found)
    {
        // overwrite
        data[idx] = entry;
    }
    else
    {
        if(is_leaf())
        {
            data_count++;
            insert_entry(idx,entry);
        }
        else
        {
            // Call next subset[i]
            subset[idx]->loose_insert(entry);

            // Fix excess if the node holds more than maximum data.
            if(subset[idx]->data_count > MAXIMUM)
            {
                fix_excess(idx);
            }
        }
    }
}

template<class T>
void BPlusTree<T>::insert_entry(int idx, T entry)
{
    T temp[MAXIMUM+1];
//    for(int i=0; i<MAXIMUM+1; i++)
//    {
//        temp[i] = NULL;
//    }

    for (int j=0; j<idx; j++)
    {
        temp[j]=data[j];
    }
    temp[idx] = entry;
    for(int k=idx; k<data_count-1; k++)
    {
        temp[k+1] = data[k];
    }

    // copy array
    copy_arr(temp,data,data_count);
}

template<class T>
void BPlusTree<T>::fix_excess(int idx)
{
    int split_idx;
    int walker=0;

    // Insert subset
    child_count++;
    insert_subset(idx);

    // If level of the node being fixed is a leaf,
    if(subset[idx]->is_leaf())
    {
        //modify the link list pointer.
        subset[idx+1]->next = subset[idx]->next;
        subset[idx]->next = subset[idx+1];

        // Split the excess between
        split_idx = subset[idx]->data_count/2;

        // copy 2nd half data to subset[i+1](Split)
        for(int k=split_idx; k<MAXIMUM+1;k++)
        {
            subset[idx+1]->data[walker] = subset[idx]->data[k];
            subset[idx]->delete_entry(k);
            subset[idx]->data_count--;
            subset[idx+1]->data_count++;

            walker++;
        }

        // Move the last element to parent
        data_count++;
        T temp[MAXIMUM+1];

        copy_arr(data,temp,idx);
        temp[idx] = subset[idx+1]->data[split_idx-1];

        for(int k=idx; k<data_count-1; k++)
        {
            temp[k+1] = data[k];
        }

        copy_arr(temp,data,data_count);
    }


    // If level of the node being fixed is NOT a leaf,
    else
    {
        // Split the excess
        double ri = (subset[idx]->data_count/2.0);
        split_idx = (int)(ri+0.5);

        // copy 2nd half data to subset[i+1](Split)
        for(int k=split_idx; k<MAXIMUM+1;k++)
        {
            subset[idx+1]->data[walker] = subset[idx]->data[k];
            subset[idx]->delete_entry(k);
            subset[idx]->data_count--;
            subset[idx+1]->data_count++;
            walker++;
        }

        // Split child
        int temp_c=0;
        double di = (subset[idx]->child_count/2);
        int child_index = (int)(di+0.5);

        for(int c=child_index; c<MAXIMUM+2; c++)
        {
            subset[idx+1]->subset[temp_c] = subset[idx]->subset[c];
            subset[idx]->subset[c] = nullptr;
            subset[idx]->child_count--;
            subset[idx+1]->child_count++;

            temp_c++;
        }

        // Move the last element to parent
        data_count++;
        T temp[MAXIMUM+1];


        copy_arr(data,temp,idx);
        temp[idx] = subset[idx]->data[split_idx-1];
        subset[idx]->delete_entry(split_idx-1);
        (subset[idx]->data_count)--;

        for(int k=idx; k<data_count-1; k++)
        {
            temp[k+1] = data[k];
        }

        copy_arr(temp,data,data_count);
    }

}

template<class T>
void BPlusTree<T>::insert_subset(int idx)
{
    BPlusTree* subset_temp[MAXIMUM + 2];
    for(int i=0; i<MAXIMUM+2; i++)
    {
        subset_temp[i] = nullptr;
    }

    // Copy data from [0]~[i-1]
    for (int j=0; j<idx+1; j++)
    {
        subset_temp[j]=subset[j];
    }

    // Insert new node in position [i]
    subset_temp[idx+1] = new BPlusTree<T>;

    // Copy data from [i+1] to last data
    for(int k=idx+1; k<MAXIMUM+2; k++)
    {
        subset_temp[k+1] = subset[k];
    }


    copy_arr(subset_temp,subset,MAXIMUM+2);
}

template<class T>
void BPlusTree<T>::loose_remove(const T &entry)
{
    int idx = get_idx(data, data_count, entry);
    bool found = (idx<data_count && data[idx] == entry);

    // If target is found
    if(found)
    {
        // if target found is in the leaf
        if(is_leaf())
        {
            // delete the item on index i;
            delete_entry(idx);
            data_count--;
        }
        // if target found is not in the leaf
        else
        {
            subset[idx+1]->loose_remove(entry);
            if(subset[idx+1]->data_count<MINIMUM)
            {
                fix_shortage(idx+1);
            }


            //fix internal node
            for(int i=0; i< data_count; i++)
            {
                data[i] = get_smallest(i+1);
            }
            //maybe delete internal entry
        }
    }

    // if target is not found
    else
    {
        // if target is n ot found and is leaf
        if(is_leaf())
        {
            cout << entry << " is not found in the tree.";
        }
            // if target is not found and is n ot leaf
        else
        {
            subset[idx]->loose_remove(entry);

            if(subset[idx]->data_count<MINIMUM)
            {
                fix_shortage(idx);
            }
            //fix top internal node
            for(int i=0; i< data_count; i++)
            {
                data[i] = get_smallest(i+1);
            }

        }
    }
}

template<class T>
void BPlusTree<T>::delete_entry(int idx)
{
    for(int i=idx; i<data_count-1; i++)
    {
        data[i] = data[i+1];
    }
}

template<class T>
void BPlusTree<T>::fix_shortage(int idx)
{
    // if subset is located at left-most index
    if(idx==0)
    {
        // if right sibling holds data>MINIMUM, borrow from right
        if(subset[idx+1]->data_count > MINIMUM)
        {
            transfer_left(idx+1);
        }
        //if right sibling doesn't have enough data to lend, merge with right
        else
        {
            // merge with right

            // Getting the insert index
            int insert_idx;
            if(subset[idx]->data_count==0)
            {
                insert_idx=0;
            }
            else
            {
                insert_idx = (subset[idx]->data_count)-1;
            }

            // Merge with right sibling
            for(int i=0; i<subset[i+1]->data_count; i++)
            {
                subset[idx]->data_count++;
                T target = subset[i+1]->data[i];
                subset[idx]->insert_entry(insert_idx,target);
                insert_idx++;
            }

            // detach the subset and modify the next pointer
            subset[idx]->next = subset[idx+1]->next;

            //deleting child subset[idx+1]
            delete subset[idx+1];
            for(int j=idx+1; j<child_count; j++)
            {
                subset[j]= subset[j+1];
            }
            child_count--;

            // adjusting data
            data_count--;
            delete_entry(idx);
        }

    }
    else if(idx==child_count-1)
    {
        if(subset[idx-1]->data_count>MINIMUM)
        {
            transfer_right(idx-1);
        }
        else
        {
            // Getting the insert index
            int insert_idx = subset[idx-1]->data_count;


            // Merge with left sibling
            for(int i=0; i<subset[idx]->data_count; i++)
            {
                subset[idx-1]->data_count++;
                T target = subset[idx]->data[i];
                subset[idx-1]->insert_entry(insert_idx,target);
                insert_idx++;
            }

            // Children relocation
            if(!subset[idx]->is_leaf())
            {
                int child_idx=subset[idx-1]->child_count; //next empty subset index
                for(int i=0; i<subset[idx]->child_count; i++)
                {
                    subset[idx-1]->subset[child_idx] = subset[idx]->subset[i];
                    subset[idx-1]->child_count++;
                    subset[idx-1]->data_count++;  //maybe delete
                    subset[idx]->subset[i] = nullptr;
                    subset[idx]->child_count--;
                }

                //fix internal node
                for(int i=0; i<subset[idx-1]->data_count; i++)
                {
                    subset[idx-1]->data[i] = subset[idx-1]->get_smallest(i+1);
                }
            }
            // detach the subset and modify the next pointer if leaf
            else
            {
                subset[idx-1]->next = subset[idx]->next;
            }

            delete subset[idx];
            for(int j=idx; j<child_count; j++)
            {
                subset[j]= subset[j+1];
            }
            child_count--;

            // adjusting data
            delete_entry(idx-1);
            data_count--;
//            // Getting the insert index
//            int insert_idx = subset[idx-1]->data_count;
//
//
//            // Merge with left sibling
//            for(int i=0; i<subset[idx]->data_count; i++)
//            {
//                subset[idx-1]->data_count++;
//                T target = subset[idx]->data[i];
//                subset[idx-1]->insert_entry(insert_idx,target);
//                insert_idx++;
//            }
//
//            // detach the subset and modify the next pointer if leaf
//            subset[idx-1]->next = subset[idx]->next;
//
//            //deleting child subset[idx]
//            delete subset[idx];
//            for(int j=idx; j<child_count; j++)
//            {
//                subset[j]= subset[j+1];
//            }
//            child_count--;
//
//            // adjusting data
//            delete_entry(idx-1);
//            data_count--;
        }
    }

    // if the subset is NOT located at left-most
    else
    {
        //check if subset[idx-1] has enough data to lend
        if(subset[idx-1]->data_count>MINIMUM)
        {
            transfer_right(idx-1);
        }
        //check if subset[idx+1] has enough data to lend
        else if(subset[idx+1]->data_count>MINIMUM)
        {
            transfer_left(idx+1);
        }
        else
        {
            // Getting the insert index
            int insert_idx = subset[idx-1]->data_count;


            // Merge with left sibling
            for(int i=0; i<subset[idx]->data_count; i++)
            {
                subset[idx-1]->data_count++;
                T target = subset[idx]->data[i];
                subset[idx-1]->insert_entry(insert_idx,target);
                insert_idx++;
            }

            // detach the subset and modify the next pointer
            subset[idx-1]->next = subset[idx]->next;

            //deleting child subset[idx]
            delete subset[idx];
            for(int j=idx; j<child_count; j++)
            {
                subset[j]= subset[j+1];
            }
            child_count--;

            // adjusting data
            data_count--;
            delete_entry(idx);
        }
    }

}

template<class T>
void BPlusTree<T>::transfer_left(int idx)
{
    T target = subset[idx]->data[0];

    // Insert to left
    int insert_idx;
    if(subset[idx-1]->data_count==0)
    {
        insert_idx=0;
    }
    else
    {
        insert_idx = (subset[idx-1]->data_count)-1;
    }
    subset[idx-1]->data_count++;
    subset[idx-1]->insert_entry(insert_idx,target);

    // Delete from right
    subset[idx]->delete_entry(0);
    subset[idx]->data_count--;

}

template<class T>
void BPlusTree<T>::transfer_right(int idx)
{
    T target = subset[idx]->data[subset[idx]->data_count-1];

    // Insert to right
    int insert_idx;
    if(subset[idx+1]->data_count==0)
    {
        insert_idx=0;
    }
    else
    {
        insert_idx = (subset[idx+1]->data_count)-1;
    }
    subset[idx+1]->data_count++;
    subset[idx+1]->insert_entry(insert_idx,target);

    // Delete from left
    subset[idx]->delete_entry(subset[idx]->data_count-1);
    subset[idx]->data_count--;
}

template<class T>
T BPlusTree<T>::get_smallest(int idx)
{
    if(subset[idx]->is_leaf())
    {
        return subset[idx]->data[0];
    }
    else
    {
        return subset[idx]->get_smallest(0);
    }
}




#endif //BPLUSTREE_BPLUSTREE_H
