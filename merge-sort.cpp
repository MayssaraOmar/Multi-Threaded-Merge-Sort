#include <bits/stdc++.h>
#include <pthread.h>

using namespace std;

class MergeSort{
    vector<int>* list;
    struct arguments{
        vector<int>* list;
        int start;
        int end;
        arguments(vector<int> &list, int start, int end) : list(&list), start(start), end(end) {}
    };
public:
    MergeSort(vector<int> &list){
        this->list = &list;
    }
    void execute(){
        arguments args(*list, 0, (*list).size()-1);
        return (void) mergeSort((void *) &args);
    }
private:
    static void merge(vector<int> &list, int start, int end){
        int mid = (start+end)/2;
        vector<int> a, b;
        for(int i=start; i<=mid; i++){
            a.push_back(list[i]);
        }
        for(int i=mid+1; i<=end; i++){
            b.push_back(list[i]);
        }
        int ai = 0, bi = 0, i = start;
        while(ai < a.size() && bi < b.size()){
            if(a[ai] < b[bi]){
                list[i++] = a[ai++];
            }
            else {
                list[i++] = b[bi++];
            }
        }
        while(ai < a.size()){
            list[i++] = a[ai++];
        }
        while(bi < b.size()){
            list[i++] = b[bi++];
        }
    }
    static void* mergeSort(void* args){
        arguments* curr_args = (arguments*) args;
        int start = curr_args->start;
        int end = curr_args->end;
        if(start >= end){
            return NULL;
        }
        pthread_t threads[2];
        int mid = (start+end)/2;
        int rc;
        arguments args1(*curr_args->list, start, mid);
        rc = pthread_create(&threads[0], NULL, mergeSort, (void *) &args1);
        if (rc){
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
        arguments args2(*curr_args->list, mid+1, end);
        rc = pthread_create(&threads[1], NULL, mergeSort, (void *) &args2);
        if (rc){
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
        for (int i = 0; i < 2; i++){
            int err = pthread_join(threads[i], NULL); // suspends execution of the calling thread until the target thread terminates
            if(err != 0){
                cerr << err << endl;
            }
        }
        merge(*curr_args->list, start, end);
    }
};



int main () {

    ifstream in("input.txt");
    int sz;
    in >> sz;
    vector<int> list(sz);
    for(int i=0; i<sz; i++){
        in >> list[i];
    }
    MergeSort ms(list);
    ms.execute();
    for(int &num : list){
        cout << num << ' ';
    }
   return 0;
}
