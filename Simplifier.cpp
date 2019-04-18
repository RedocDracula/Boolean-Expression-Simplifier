#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <bitset>
#include <sstream>
#include <set>
#include <fstream>
#include <cctype>
#include <cmath>
#include <bitset>
#include <list>
#include <stack>
#define ll long long int 
#define pb push_back
#define mp make_pair
#define pci pair <char,int> 
using namespace std;

void task2(list <int> minterms);

void convert(vector <pci> exp,set <int> &minterm, vector <char> input, int vecindex,int index,string out)
{
    if(out.size()==input.size())
    {
        reverse(out.begin(),out.end());
        long long sum=0,p=1;
        for(int i=out.size()-1;i>=0;i--)
        {
            sum += p*(out[i]-'0');
            p *= 2;
        }
        minterm.insert(sum);
        return;
    }

    if(vecindex<exp.size() && exp[vecindex].first==input[index])
    {
        out.pb(exp[vecindex].second+'0');
        convert(exp,minterm,input,vecindex+1,index+1,out);
    }

    else
    {
        out.pb('1');
        int s = out.size();
        convert(exp,minterm,input,vecindex,index+1,out);
        out[s-1] = '0';
        convert(exp,minterm,input,vecindex,index+1,out);
    }

}

void evaluatesop(vector <string> terms, vector <char> input, ofstream &ofile)
{
    set <int> minterms;
    
    
    for(int i=0; i<terms.size() ;i++) // go through all the terms
    {
        vector <pci> expression;
        string out;
        for(int j=0;j<terms[i].size();j++ ) // go through the characters of the term
        {
            if(j!=terms[i].size()-1 && terms[i][j+1] == 39) // 39 is ascii for ' character
            {
                expression.pb(mp(terms[i][j],0)); // if it's complement is present
            }
            else if(isalpha(terms[i][j]))
            expression.pb(mp(terms[i][j],1)); 
        }
         for(int j=0;j<expression.size();j++)
        {
            sort(expression.begin(),expression.end());
        }
        convert(expression,minterms,input,0,0,out);
    }
 
   
    set <int> :: iterator it;
    for(it = minterms.begin();it!=minterms.end();it++)
    {
        ofile<<*it<<" ";
    }

    list <int> mint(minterms.begin(),minterms.end());
    task2(mint);

    
}

void evaluatepos(vector <string> terms, vector <char> input, ofstream &ofile)
{
    set <int> maxterms;
    
    for(int i=0; i<terms.size() ;i++) // go through all the terms
    {
        vector <pci> expression;
        string out;
        for(int j=0;j<terms[i].size();j++ ) // go through the characters of the term
        {
            if(j!=terms[i].size()-1 && terms[i][j+1] == 39) // 39 is ascii for ' character
            {
                expression.pb(mp(terms[i][j],1)); // if it's complement is present
            }
            else if(isalpha(terms[i][j]))
            expression.pb(mp(terms[i][j],0)); 
        }
         for(int j=0;j<expression.size();j++)
        {
            sort(expression.begin(),expression.end());
        }
        convert(expression,maxterms,input,0,0,out);
    }
 
   
    set <int> :: iterator it;
    for(it = maxterms.begin();it!=maxterms.end();it++)
    {
        ofile<<*it<<" ";
    }

    int size = pow(2,input.size());

    list <int> mint;
    
    for(int i=0;i<size;i++)
    {
        it = find(maxterms.begin(),maxterms.end(),i);
        if(it==maxterms.end())
        {
            mint.push_back(i);
        }
    }
   

    task2(mint);
}

//////TASK B //////////////////


struct Implicant
{
    int *array;
    int size;
    Implicant(int _size)
    {
        size = _size;
        array = new int[size];
    }
    Implicant(int _size, int a[])
    {
        size = _size;
        array = new int[size];
        for(int i=0;i<size;i++)
        array[i] = a[i];
    }
    Implicant(int _size, Implicant a, Implicant b)
    {
        size = _size;
        array = new int[size];
        sort(a.array, a.array+a.size); 
        sort(b.array, b.array + b.size); 
        merge(a.array, a.array+a.size, b.array,b.array+b.size,array); 
    }

};

struct bucket
{
    list <Implicant> implicants;
};

struct bucketstore
{
    stack <bucket**> store;
};


ll count1s(ll v)
{
    
    ll c; 
    for (c = 0; v; c++)
    {
        v &= v - 1; 
    }
    
    return c;
}

void readinput(list <int> &minterms, string input)
{
    int num = 0;
    for(int i=0;i<input.size()+1;i++)
    {
        if(input[i]==' '||input[i]=='\0')
        {
            minterms.push_back(num);
            num = 0;
        }

        else{
            num = num*10 + input[i]-'0';
        }
    }
}

bool checkpower2(ll n)
{
    if(!(n&(n-1)))
    return 1;
    else
    return 0;
}

bool issubset(int a[],int as,int b[], int bs)
{
    int i=0;
    while(i<as)
    {
        bool found = 0;
        for(int j=0;j<bs;j++)
        {
            if(b[j]==a[i])
            {
                found = 1;
                break;
            }
        }
        if(!found)
        return 0;

        i++;
    }

    return 1;
}

bool compare(int a[],int b[],int s)
{
    int diff  = 0,prev = a[0]-b[0];
    bool flag = 0;
    for(int i = 0; i < s; i++)
    {
        diff = a[i]-b[i];
        if(diff<0&&diff==prev&&checkpower2(-diff))
        {
            flag = 1;
        }
        else
        {
            return 0;
        }
    }

    return 1;
} 

bool checkifsame(int a[],int b[],int s)
{
    for(int i=0;i<s;i++)
    {
        if(a[i]!=b[i])
        return 0;
    }

    return 1;
}

void printarr(int a[],int s)
{
    for(int i=0;i<s;i++)
    cout<<a[i]<<" ";

    cout<<endl;
}

bool mintermfound(int n,int a[],int s)
{
    for(int i=0;i<s;i++)
    {
        if(a[i]==n)
        return 1;
    }

    return 0;
}

void removeminterms(int a[],int s,list <int> &minterms)
{
    for(int i=0;i<s;i++)
    {
        list <int> :: iterator it = find(minterms.begin(),minterms.end(),a[i]);
        if(it!=minterms.end()) // if minterm is found
        {
            minterms.erase(it);
        }
    }
}

void printsop(int a[], int s,int bound)
{
    bound--;
    bitset <64> b[s];
    char v = 'a';
    

    for(int i=0;i<s;i++)
    {
        bitset <64> temp(a[i]);
        b[i] = temp;
    }

    for(int i=0;i<bound;i++)
    {
        int prev = b[0][i];
        bool flag = 1;
        for(int j=0;j<s;j++)
        {
            if(b[j][i]!=prev)
            {
                flag = 0;
                break;
            }
        }

        if(flag&&prev==0)
        cout<<v<<"'";

        else if(flag&&prev==1)
        cout<<v;
       
       v = v+1;
      
    }

}




int main()
{
    vector <char> inputvariables,outputvariables;
    vector <pci> expressions;
    set <int> minterms;
    string s,t;
    ifstream ifile("input.txt",ios::in);
    ofstream ofile("output.txt",ios::app);
    ofile<<".input ";
   
    getline(ifile,s);  // for input variables
    stringstream ss(s);
    while(ss>>t)
    {
        if(t!=".input")
        {
            inputvariables.pb(t[0]); //store input variables
        }
    }

    for(int i=0;i<inputvariables.size();i++)
    {
        ofile<<inputvariables[i]<<" ";
        
    }
    ofile<<"\n";

    
    
    getline(ifile,s);
    stringstream so(s); // for output variables
    while(so>>t)
    {
        if(t!=".output")
        {
            for(int i=0;i<t.size();i++)
                if(isalpha(t[i]))
                    outputvariables.pb(t[i]); // store output variables
        }
    }

    for(ll i =0;i<outputvariables.size();i++)
    {
        vector <string> terms;
        string current;
        bool sop = true;
        getline(ifile,s);
        ll lbracket = 0;
        for(int j=0;j<s.size();j++)
        {
            if(s[j]=='.')
            {
                j = j+2; // skip the .variable part
                continue;
            }
            else if(s[j]==' ')
            {
                continue;
            }
            else if(s[j]=='(')
            {
                sop = false;
                lbracket++;
            }
            else if(s[j]==')'&&lbracket>0)
            {
                lbracket--;
                terms.pb(current);
                current.erase();
            }
            else if(s[j]=='+')
            {
                if(sop)
                {
                    terms.pb(current);
                    current.clear();
                }
                else // if it is POS we just ignore the '+' 
                {
                    continue;
                }
            }
            else
            {
                current.pb(s[j]);
            }
        }
        if(current.size()>0)
        terms.pb(current);


        if(sop)
        {
            ofile<<outputvariables[i]<<" = m( ";
            evaluatesop(terms,inputvariables,ofile);
            ofile<<")\n";

        }
        else
        {
            ofile<<outputvariables[i]<<" = M( ";
            evaluatepos(terms,inputvariables,ofile);
            ofile<<")\n";
        }
    }

    return 0;
    
}


void task2(list <int> minterms)
{
   
    list <int> :: iterator lastelement;
    list <int> :: iterator it;
    list <Implicant> :: iterator it1,it2;
   
    minterms.sort();
    lastelement = minterms.end();
    lastelement--;
    long long int size_of_bucket = ceil(log2(*lastelement)) + 1;
    ll initsize = size_of_bucket,bound = size_of_bucket;
    struct bucket** pbucket = new bucket*[size_of_bucket];
    struct bucketstore bstore;

    for(int i=0;i<size_of_bucket;i++)
    pbucket[i] = new bucket;

    for(it=minterms.begin();it!=minterms.end();it++)
    {
        ll term = *it;
        Implicant e(1);
        e.array[0] = term;
        pbucket[count1s(term)]->implicants.push_back(e);
    }

    bstore.store.push(pbucket); // stored pbucket in bstore
    
    bool added = 1;
   
    while(added)
    {
        added = 0;
        struct bucket** nbucket = new bucket*[size_of_bucket-1];
        for(int i=0;i<size_of_bucket-1;i++)
        {
            nbucket[i] = new bucket;
        }

        for(int i = 0; i<size_of_bucket-1;i++)
        {
            
            for( it1 = pbucket[i]->implicants.begin();it1!=pbucket[i]->implicants.end();it1++)
            {
                for(it2 = pbucket[i+1]->implicants.begin();it2!= pbucket[i+1]->implicants.end();it2++)
                {
                    
                    if(compare(it1->array,it2->array,it1->size))
                    {
                        Implicant e(2*it1->size,*it1,*it2);
                        list <Implicant> :: iterator im;
                        bool same = 0;
                        if(nbucket[i]->implicants.size()==0)
                        {
                            nbucket[i]->implicants.push_back(e);
                            added = 1;
                        }
                       
                        else
                        {
                            for(im = nbucket[i]->implicants.begin();im!=nbucket[i]->implicants.end();im++)
                            {
                                if(checkifsame(e.array,im->array,e.size))
                                {
                                    same = 1;
                                    break;
                                }
                            }

                            if(!same)
                            {
                                added = 1;
                                nbucket[i]->implicants.push_back(e);
                            }
                        }
                        
                    }
                }
            }
        }
       
        if(added)
        {
             size_of_bucket -= 1;
            bstore.store.push(nbucket); // store the newly created bucket
            pbucket = nbucket;
        }
        
    }

    
    bucket primeimplicants;

    struct bucket **ptr = bstore.store.top();
    for(int i = 0;i<size_of_bucket;i++)
    {
        it1 = ptr[i]->implicants.begin();
        for(;it1!=ptr[i]->implicants.end();it1++)
        {
            Implicant e(it1->size,it1->array); // copy prime implicant
            primeimplicants.implicants.push_back(e);
        }
    }
    bstore.store.pop();
    size_of_bucket++; // size of the next bucket is one more
    

    while(!bstore.store.empty())
    {
        
        struct bucket** p = bstore.store.top();
        
        
        for(int i=0;i<size_of_bucket;i++)
        {
           it1 = p[i]->implicants.begin();
           for(;it1!=p[i]->implicants.end();it1++)
           {
               list <Implicant> :: iterator pr;
               bool check = 0;
               for(pr=primeimplicants.implicants.begin();pr!=primeimplicants.implicants.end();pr++)
               {
                   if(issubset(it1->array,it1->size,pr->array,pr->size))
                   {
                       check = 1;
                       break;
                   }
               }
               if(!check) // if it's not a subset
               {
                   Implicant e(it1->size,it1->array);//add this to prime implicant
                   primeimplicants.implicants.push_back(e);
               }
           }
        }
        
        bstore.store.pop();
        size_of_bucket++;
       
    }
    cout<<"prime implicants : "<<endl;

    for(it1 = primeimplicants.implicants.begin(); it1!=primeimplicants.implicants.end();it1++)
    {
        printarr(it1->array,it1->size);
    }

    bucket ess_primeimplicants;

    for(it = minterms.begin(); it!=minterms.end();it++) // for each minterm find ess prime implicants
    {
        int count = 0;
        bool pick = 1;
        list <Implicant> :: iterator pos;
        for(it1=primeimplicants.implicants.begin();it1!=primeimplicants.implicants.end();it1++)
        {
            if(mintermfound(*it,it1->array,it1->size))
            {
                count++;
                pos = it1;  // stores the position of the prime implicant
            }

            if(count>1)
            {
                pick = 0; // not essential
                break;
            }
        }

        if(pick&&count==1)
        {
            bool add = 1;
            Implicant e(pos->size,pos->array);
            for(it1 = ess_primeimplicants.implicants.begin(); it1!=ess_primeimplicants.implicants.end();it1++)
            {
                if(checkifsame(it1->array,pos->array,pos->size))
                {
                    add = 0;
                    break;
                }
            }

            if(add)
            ess_primeimplicants.implicants.push_back(e);
        }
    }
    //1a
    
    while(!minterms.empty())
    {
        for(it1 = ess_primeimplicants.implicants.begin(); it1!=ess_primeimplicants.implicants.end();it1++)
        {
            removeminterms(it1->array,it1->size,minterms);
        }

        vector < list <Implicant> :: iterator > vec;

        for(it1=primeimplicants.implicants.begin();it1!=primeimplicants.implicants.end();it1++)
        {
            bool remove = 1;
            for(int i=0;i<it1->size;i++)
            {
                it = find(minterms.begin(),minterms.end(),it1->array[i]);
                if(it!=minterms.end())
                {
                    remove = 0;
                }
            }

            if(remove)
            {
                list <Implicant> :: iterator rem = it1;
                vec.push_back(rem);
            }
        }

        for(int i=0;i<vec.size();i++)
        {
            primeimplicants.implicants.erase(vec[i]);
        }

        
        list <Implicant> :: iterator add;
        int previous = -10;
        bool f = 0;
        
        for(it1=primeimplicants.implicants.begin();it1!=primeimplicants.implicants.end();it1++)
        {
            int count = 0;
            
            for(int i=0;i<it1->size;i++)
            {
                it = find(minterms.begin(),minterms.end(),it1->array[i]);
                if(it!=minterms.end())
                {
                    count++;
                }
            }

            if(count>previous)
            {
                previous = count;
                add = it1;
                f =1;
            }
        }

        if(f)
        {
            Implicant e(add->size,add->array);
            printarr(add->array,add->size);;
            ess_primeimplicants.implicants.push_back(e);
        }

    }
 
    cout<<"ESSENTIAL PRIME IMPLICANTS "<<endl;
  
    for(it1 = ess_primeimplicants.implicants.begin(); it1!=ess_primeimplicants.implicants.end();it1++)
    {
        printarr(it1->array,it1->size);
    }

    cout<<"REDUCED EXPRESSION : "<<endl;
    

    for(it1 = ess_primeimplicants.implicants.begin(); it1!=ess_primeimplicants.implicants.end();it1++)
    {
        printsop(it1->array,it1->size,bound);
        cout<<"+";
    }

    cout<<"\b \b";
    cout<<endl;

    
    
}

