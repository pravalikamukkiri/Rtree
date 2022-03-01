#include<bits/stdc++.h>
#include<fstream>
using namespace std;
#define intl long long

struct rtree{
	vector<intl> mbr;
	rtree * parent;
	vector<rtree*> child;
};
rtree * newnode(){
	rtree * temp=new rtree;
	temp->mbr.clear();
	temp->parent=NULL;
	temp->child.clear();
	return temp;
}
void error(string msg="Error"){
	cout<<msg<<endl;
	exit(-1);
}
string preprocess(string inputstr){
	try{
		string q;
		for(intl i=0;i<inputstr.size();i++)
			if(inputstr[i]!=' ')
				q+=inputstr[i];
		return q;
	}
	catch(...){
		error("Error");
	}
	return "";
}

vector<intl> splitfunnums(string pointldata){
	try{
		string temp;
		vector<intl> pointls;
		for(intl i=0;i<pointldata.size();i++){
			if(pointldata[i]!=',')
				temp+=pointldata[i];
			else{
				intl a=stoi(temp);
				pointls.push_back(a);
				temp="";
			}
		}
		intl a=stoi(temp);
		pointls.push_back(a);
		return pointls;
	}
	catch(...){
		error("Error in prasing query");
	}
	return {};
}
void printrtree(rtree *root, intl l=0){
	// for(intl i=0;i<l;i++)
	// 	cout<<"-";
	// cout<<root->mbr[0]<<" "<<root->mbr[1]<<" "<<root->mbr[2]<<" "<<root->mbr[3]<<" ---"<<root->child.size()<<endl;
	for(intl i=0;i<root->child.size();i++)
		printrtree(root->child[i],l+1);
}
bool ispresent(vector<intl>mbr,vector<intl>br){
	// cout<<mbr[0]<<" "<<mbr[1]<<" "<<mbr[2]<<" "<<mbr[3]<<" "<<br[0]<<" "<<br[1]<<endl;
	if(mbr[0]<=br[0] && br[0]<=mbr[2] && mbr[1]<=br[1] && br[1]<=mbr[3])
		return true;
	return false;
}
bool search(rtree *root, vector<intl>br){

	intl a=root->mbr[0];
	intl b=root->mbr[1];
	intl c=root->mbr[2];
	intl d=root->mbr[3];

	bool res=false;

	// cout<<a<<" "<<b<<" "<<c<<" "<<d<<" " <<br[0]<<" "<<br[1]<<" "<<root->child.size()<<endl;
	if(br[0]==a && br[1]==b && br[2]==c && br[3]==d)
		return true;
	for(intl i=0;i<root->child.size();i++){
		if(ispresent(root->child[i]->mbr,br)){
			res= res || search(root->child[i],br);
			if(res)
				return true;
		}
	}
	return res;
	
}

intl area(vector<intl> a1){
	int l1=a1[2]-a1[0];
	int l2=a1[3]-a1[1];
	return l1*l2;
}

bool isleaf(rtree *temp){
	bool x=true;
	for(intl i=0;i<temp->child.size();i++)
		x = x && (temp->child[i]->mbr[0]==temp->child[i]->mbr[2] && temp->child[i]->mbr[1]==temp->child[i]->mbr[3]);
	return x;
}

rtree *traversal(rtree *root, vector<intl>br){
	if(isleaf(root))
		return root;

	intl a=root->mbr[0];
	intl b=root->mbr[1];
	intl c=root->mbr[2];
	intl d=root->mbr[3];

	rtree *temp=NULL;
	intl minareadiff=LONG_MAX;
	for(intl i=0;i<root->child.size();i++){
		vector<intl> newmbr={min(br[0],a),min(br[1],b),max(br[2],c),max(br[3],d)};
		intl areadiff=area(newmbr) - area(root->mbr);
		if(areadiff<minareadiff){
			minareadiff=areadiff;
			temp=root->child[i];
		}
	}
	return traversal(temp,br);
}
vector<intl> mergembr(rtree *a, rtree *b){
	vector<intl> x;
	for(intl i=0;i<2;i++)
		x.push_back(min(a->mbr[i],b->mbr[i]));
	for(intl i=2;i<4;i++)
		x.push_back(max(a->mbr[i],b->mbr[i]));
	return x;
}
intl countofpoints(rtree *temp){
	if(temp->mbr[0]==temp->mbr[2] && temp->mbr[1]==temp->mbr[3] && temp->child.size()==0)
		return 1;
	intl x=0;
	for(intl i=0;i<temp->child.size();i++)
		x+=countofpoints(temp->child[i]);
	return x;
}
vector<intl> updatembr(rtree *temp){
	// cout<<temp->mbr[0]<<" "<<temp->mbr[1]<<" "<<temp->mbr[2]<<" "<<temp->mbr[3]<<endl;
	// cout<<"updatembr"<<endl
	vector<intl> vmbr={LONG_MAX,LONG_MAX,LONG_MIN,LONG_MIN};
	for(intl i=0;i<temp->child.size();i++){
		vmbr[0]=min(vmbr[0],temp->child[i]->mbr[0]);
		vmbr[1]=min(vmbr[1],temp->child[i]->mbr[1]);
		vmbr[2]=max(vmbr[2],temp->child[i]->mbr[2]);
		vmbr[3]=max(vmbr[3],temp->child[i]->mbr[3]);
	}
	// cout<<temp->mbr[0]<<" "<<temp->mbr[1]<<" "<<temp->mbr[2]<<" "<<temp->mbr[3]<<endl;
	return vmbr;
}
void entireupmbr(rtree *temp){
	for(intl i=0;i<temp->child.size();i++)
		entireupmbr(temp->child[i]);
	if(!temp->parent)
		temp->mbr=updatembr(temp);

}

void splitfun(rtree *temp){
	if(!temp)
		return;
	// cout<<temp->mbr[0]<<" "<<temp->mbr[1]<<" "<<temp->mbr[2]<<" "<<temp->mbr[3]<<endl;
	if(temp->child.size()>12){
		vector<rtree *> n1,n2;
		for(intl i=0;i<6;i++)
			n1.push_back(temp->child[i]);
		for(intl i=6;i<temp->child.size();i++)
			n2.push_back(temp->child[i]);

		rtree *p=temp->parent;

		if(p){
			rtree *nr = newnode();
			nr->child=n1;
			for(intl i=0;i<n1.size();i++)
				n1[i]->parent=nr;

			nr->parent=p;
			nr->mbr=updatembr(nr);

			temp->child.clear();
			temp->child=n2;
			for(intl i=0;i<n2.size();i++)
				n2[i]->parent=temp;

			temp->mbr=updatembr(temp);

			p->child.push_back(nr);
			p->mbr=updatembr(p);

			splitfun(p);
		}
		else{
			rtree *nr1 = newnode();
			nr1->child=n1;
			for(intl i=0;i<n1.size();i++)
				n1[i]->parent=nr1;

			nr1->parent=temp;
			nr1->mbr=updatembr(nr1);

			rtree *nr2 = newnode();
			nr2->child=n2;
			for(intl i=0;i<n2.size();i++)
				n2[i]->parent=nr2;

			nr2->parent=temp;
			nr2->mbr=updatembr(nr2);

			temp->child.clear();
			temp->child.push_back(nr1);
			temp->child.push_back(nr2);
		}

	
	}
	temp->mbr=updatembr(temp);
	splitfun(temp->parent);
}

void insert(vector<intl> br, rtree * root){
	rtree * temp=traversal(root,br);
	rtree *nmbr = newnode();
	nmbr->mbr=br;
	nmbr->parent=temp;
	temp->child.push_back(nmbr);
	splitfun(temp);
}

bool doesintersect(vector<intl>mbr1 , vector<intl> mbr2){
	 // for(intl i=0;i<mbr2.size();i++)
	 // 	cout<<mbr2[i]<<" ";
	 // cout<<endl;
	if(mbr2[3]<mbr1[1] || mbr2[2]<mbr1[0] || mbr2[0]>mbr1[2] || mbr2[1]>mbr1[3])
		return false;
	return true;
}

bool ispoint(rtree *temp){
	if(temp->mbr[0]==temp->mbr[2] && temp->mbr[1]==temp->mbr[3] && temp->child.size()==0)
		return true;
	return false;
}

 intl range(vector<intl> br, rtree* root){
 	if(!root)
 		return 0;
 	// cout<<root->mbr[0]<<" "<<root->mbr[1]<<" "<<root->mbr[2]<<" "<<root->mbr[3]<<endl;
 	if(ispoint(root) && doesintersect(br,root->mbr))
 		return 1;
 	intl c=0;
 	for(intl i=0;i<root->child.size();i++){
 		if(doesintersect(br,root->child[i]->mbr))
 			c+=range(br,root->child[i]);
 	}

 	return c;
 }

int main(int argc, char* argv[]){
	if(argc<2)
		error("input file not provided");

	string inp_file=argv[1];
	ifstream file;
	file.open(inp_file);
	string inputstr;

	intl maxn=1000000000;
	rtree * root=newnode();
	root->mbr ={-maxn,-maxn,maxn,maxn};
	intl ii=0;
	
	while(file >> inputstr){
		// printrtree(root);
		// cout<<ii++<<endl;

		inputstr=preprocess(inputstr);
		// cout<<inputstr<<endl;

		if(inputstr[inputstr.size()-1]!=')')
			continue;
		if(inputstr.substr(0,4)=="FIND"){
			// printrtree(root);
			// intl c=countofpoints(root);
			// cout<<c<<endl;
			// cout<<"search"<<endl;
			if(inputstr[4]!='(')
				continue;
			string pointldata=inputstr.substr(5,inputstr.size()-6);
			// cout<<pointldata<<endl;
			vector<intl> pointls=splitfunnums(pointldata);

			if(pointls.size()!=2)
				error("POintls are not given correctly");
			// cout<<pointls[0]<<" "<<pointls[1]<<endl;
			vector<intl> br={pointls[0],pointls[1],pointls[0],pointls[1]};
			bool res=search(root,br);
			if(res)
				cout<<"YES"<<endl;
			else
				cout<<"NO"<<endl;
			continue;
		}
		else if(inputstr.substr(0,5)=="RANGE"){
			if(inputstr[5]!='(')
				continue;
			string pointldata=inputstr.substr(6,inputstr.size()-7);
			// cout<<pointldata<<endl;
			vector<intl> pointls=splitfunnums(pointldata);

			if(pointls.size()!=4)
				error("POintls are not given correctly");
			// cout<<pointls[0]<<" "<<pointls[1]<<" "<<pointls[2]<<" "<<pointls[3]<<endl;
			intl res=range(pointls,root);
			cout<<res<<endl;
			
		}
		else if(inputstr.substr(0,6)=="INSERT"){
			// cout<<"inse"<<endl;
			if(inputstr[6]!='(')
				continue;
			string pointldata=inputstr.substr(7,inputstr.size()-8);
			// cout<<pointldata<<endl;
			vector<intl> pointls=splitfunnums(pointldata);

			if(pointls.size()!=2)
				error("POintls are not given correctly");
			// cout<<pointls[0]<<" "<<pointls[1]<<endl;
			vector<intl> br={pointls[0],pointls[1],pointls[0],pointls[1]};
			insert(br,root);
		}
	}
}