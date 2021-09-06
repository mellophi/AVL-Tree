/*
The structure is as follows: 
	head
	/  \
nullptr	entire tree
*/
#include <bits/stdc++.h>
#include <fstream>
#define print(x) printf("%s(%d): %d\n", __func__, __LINE__, x);
using namespace std;

class AVL_Node
{
	AVL_Node *left;
	int b; // balance factor = height(left subtree) - height(right subtree)
	int value;
	AVL_Node *right;
public:
	 AVL_Node(){

	 }
	 AVL_Node(int x){
	 	value = x;
	 	left = nullptr;
	 	right = nullptr;
	 	b = 0;
	 }
	~AVL_Node(){
		delete left;
		delete right;
	}
	friend class AVL_Tree;
};


class AVL_Tree
{
	AVL_Node *h;
	void write_to_file(AVL_Node*, ofstream&);
public:
	AVL_Tree(){
		h = new AVL_Node();
		h->left = nullptr;
	}
	AVL_Tree(const AVL_Tree &T); 
 	AVL_Tree &operator=(const AVL_Tree &T);
	void AVL_Insert(int);
	bool AVL_Search(int);
	void AVL_Print(const char*);
	void AVL_Delete(int);
	~AVL_Tree(){
		delete h;
	}
};

bool AVL_Tree::AVL_Search(int x){
	AVL_Node *p = h->right;
	if(!p) return false;
	while(p){
		if(x < p->value)
			p = p->left;
		else if(x > p->value)
			p = p->right;
		else
			return true;
	}
	return false;
}

void AVL_Tree::AVL_Insert(int x){
	AVL_Node *t = h;
	AVL_Node *s = h->right;
	AVL_Node *p = h->right;
	AVL_Node *q, *r;

	AVL_Node *temp = new AVL_Node(x);
	//if the right side of head is null the AVL_Inserting it there
	if(p == nullptr){
		h->right = temp;
		return;
	}

	while(true){

		if(x < p->value){
			q = p->left;
			if(!q) {
				p->left = temp;
				q = p->left;
				break;
			}
			else{
				if(q->b != 0){
					t = p;
					s = q;
				}
				p = q;
			}
		}
		else if(x > p->value){
			q = p->right;
			if(!q){
				p->right = temp;
				q = p->right;
				break;
			}
			else{
				if(q->b != 0){
					t = p;
					s = q;
				}
				p = q;
			}
		}
		else
			throw "Element already exists!!";
	}
	int a = x < s->value ? 1 : -1;
	auto link = [](int a, AVL_Node *s){
		return a == 1 ? s->left : s->right;
	};

	r = link(a, s);
	p = link(a, s);
	while(p != q){
		if(x < p->value){
			p->b = 1;
			p = p->left;
		}
		else if(x > p->value){
			p->b = -1;
			p = p->right;
		}
	} 
	if(s->b == 0){
		s->b = a;
		return;
	}
	else if(s->b == -1*a){
		s->b = 0;
		return;
	}
	else{
		//single rotation
		if(r->b == a){
			if(a == 1){
				p = r;
				s->left = r->right;
				r->right = s;
				s->b = 0;
				r->b = 0; 
			}
			else if(a == -1){
				p = r;
				s->right = r->left;
				r->left = s;
				s->b = 0;
				r->b = 0;
			}
		}

		//double rotation
		else if(r->b == -1*a){
			if(a == 1){
				p = r->right;
				r->right = p->left;
				p->left = r;
				s->left = p->right;
				p->right = s;
				s->b = p->b == 0 ? 0 : p->b == 1 ? -1 : 0;
				r->b = p->b == 0 ? 0 : p->b == 1 ? 0 : 1;
				p->b = 0;
			}
			else if(a == -1){
				p = r->left;
				r->left = p->right;
				p->right = r;
				s->right = p->left;
				p->left = s;
				s->b = p->b == 0 ? 0 : p->b == 1 ? 0 : 1;
				r->b = p->b == 0 ? 0 : p->b == 1 ? -1 : 0;
				p->b = 0;
			}
		}
	}

	if(s == t->right) t->right = p;
	else t->left = p;
}


void AVL_Tree::AVL_Delete(int x){
	AVL_Node *p = h->right; // pointer pointing to the location of x
	
	// s -> AVL_Node to be balanced
	// t -> parent of the AVL_Node to be balanced
	// r -> left/right child of the AVL_Node to be balanced
	AVL_Node *s, *t, *r;


	bool found = false;

	stack<AVL_Node*> path; // saves the path to the AVL_Node
	path.push(h); //pushing the head (dummy) AVL_Node

	if(p == nullptr) throw "No element exists to be deleted";

	while(p){
		if(x < p->value){
			path.push(p);
			p = p->left;
		}
		else if(x > p->value){
			path.push(p);
			p = p->right;
		}
		else{
			found = true;
			break;
		}
	}
	if(!found) throw "Element to be deleted does not exist";
	else{
		// if p has a single child or no child (leaf)
		if(!p->left || !p->right){
			AVL_Node *temp = p->left ? p->left : p->right;
			AVL_Node *par = path.top();

			if(!temp){
				temp = p;
				if(p == par->left)
					par->left = nullptr;
				else par->right = nullptr;
				p = nullptr;
			}
			else{
				p->value = temp->value;
				p->b = temp->b;
				p->left = temp->left;
				p->right = temp->right;
			}
			delete temp;
		}
		// AVL_Node to be deleted has both the children
		else{
			path.push(p);
			AVL_Node *succ = p->right; //finding the successor of p
			while(succ->left){
				path.push(succ);
			 	succ = succ->left;
			}
			p->value = succ->value;
			x = succ->value; // because the AVL_Node that is being deleted is the successor
			AVL_Node *temp = succ->left ? succ->left : succ->right;
			AVL_Node *par = path.top();

			if(!temp){
				temp = succ;
				if(succ == par->left)
					par->left = nullptr;
				else par->right = nullptr;
				succ = nullptr;
			}
			else{
				succ->value = temp->value;
				succ->b = temp->b;
				succ->left = temp->left;
				succ->right = temp->right;
			}
			delete temp;
		}
	}


	auto link = [](int a, AVL_Node *s){
		return a == 1 ? s->left : s->right; 
	};

	while(path.top() != h){
		s = path.top();
		int a = x < s->value ? 1 : -1;
		path.pop();
		t = path.top();

		if(s->b == a){
			s->b = 0;
			continue;
		}
		else if(s->b == 0){
			s->b = -1*a;
			return;
		}
		else{
			r = link(s->b, s);
			//single rotation
			if(r->b == -1*a){
				if(a == -1){
					p = r;
					s->left = r->right;
					r->right = s;
					s->b = 0;
					r->b = 0; 
				}
				else if(a == 1){
					p = r;
					s->right = r->left;
					r->left = s;
					s->b = 0;
					r->b = 0;
				}
			}
			//single rotation
			else if(r->b == 0){
				if(a == -1){
					p = r;
					s->left = r->right;
					r->right = s;
					r->b = a; 
				}
				else if(a == 1){
					p = r;
					s->right = r->left;
					r->left = s;
					r->b = a;
				}
			}
			//double rotation
			else if(r->b == a){
				if(a == -1){
					p = r->right;
					r->right = p->left;
					p->left = r;
					s->left = p->right;
					p->right = s;
					s->b = p->b == 0 ? 0 : p->b == 1 ? -1 : 0;
					r->b = p->b == 0 ? 0 : p->b == 1 ? 0 : 1;
					p->b = 0;
				}
				else if(a == 1){
					p = r->left;
					r->left = p->right;
					p->right = r;
					s->right = p->left;
					p->left = s;
					s->b = p->b == 0 ? 0 : p->b == 1 ? 0 : 1;
					r->b = p->b == 0 ? 0 : p->b == 1 ? -1 : 0;
					p->b = 0;
				}
			}
			//if after rotation the balance factor doesn't change and remain the same then
			//there is no need for further checking of the above nodes and we can return
			if(p->b == 1 || p->b == -1){
				if(s == t->right) t->right = p;
				else t->left = p;
				return;
			}
		}
		if(s == t->right) t->right = p;
		else t->left = p;		
	}

}




void AVL_Tree::write_to_file(AVL_Node *root, ofstream &myfile) {
 	if(root == NULL) return;
 	myfile << root->value << " [label = \"<f0> | <f1> " << root->value << " | <f2> " << root->b << " | <f3>\"];\n";
    if(root->left) {
    	write_to_file(root->left, myfile);
    	myfile << root->left->value << " [label = \"<f0> | <f1> " << root->left->value << " | <f2> " << root->left->b << " | <f3>\"];\n";
    	myfile << "\"" << root->value << "\":f0 -> \"" << root->left->value << "\":f1;\n";
    }
    if(root->right) {
    	write_to_file(root->right, myfile);
    	myfile << root->right->value << " [label = \"<f0> | <f1> " << root->right->value << " | <f2> " << root->right->b << " | <f3>\"];\n";
    	myfile << "\"" << root->value << "\":f3 -> \"" << root->right->value << "\":f1;\n";
    }   	
}



void AVL_Tree::AVL_Print(const char *filename){
  
    ofstream myfile;
    myfile.open ("graph.gv");
    myfile << "digraph {\n";
    myfile << "node [shape = record, height = .1];";
    AVL_Node *ptr = h->right;
    write_to_file(ptr, myfile);

    myfile << "}\n";
    myfile.close();

    string cmd = "dot -Tpng graph.gv -o " + string(filename);
    system((const char*)cmd.c_str());
}

int main(){
	// cout << "This program is authored by Ayon Chattopadhyay (214101012)\n";
	// cout << "------------------------------\n";
	AVL_Tree *t = new AVL_Tree();
	// cout << "Choice 1: Insert\n" << "Choice 2: Delete\n" << "Choice 3: Search\n";
	// cout << "Choice 4: Print tree\n" << "Choice 5: Get sample tree\n";
	// cout << "------------------------------\n";

	// char ch;
	// do{
	// 	cout << "Enter your choice: ";
	// 	int choice, x;
	// 	cin >> choice;
	// 	string filename;
	// 	switch(choice){
	// 		case 1:
	// 			cout << "Enter the value to insert: ";
	// 			cin >> x;
	// 			try{
	// 				t->AVL_Insert(x);
	// 			}
	// 			catch(const char *e){
	// 				cerr << e << "\n";
	// 			}
	// 			break;
	// 		case 2:
	// 			cout << "Enter the value to delete: ";
	// 			cin >> x;
	// 			try{
	// 				t->AVL_Delete(x);
	// 			}
	// 			catch(const char *e){
	// 				cerr << e << "\n";
	// 			}
	// 			break;
	// 		case 3:
	// 			cout << "Enter the element to search: ";
	// 			cin >> x;
	// 			if(t->AVL_Search(x)) cout << "Found " << x << "\n";
	// 			else cout << "Not found " << x << "\n";
	// 			break;
	// 		case 4:
	// 			cout << "Enter file name: ";
	// 			cin >> filename;
	// 			t->AVL_Print(filename.c_str());
	// 			break;
	// 		case 5:
	// 			try{
	// 				t->AVL_Insert(50);
	// 				t->AVL_Insert(30);
	// 				t->AVL_Insert(80);
	// 				t->AVL_Insert(20);
	// 				t->AVL_Insert(40);
	// 				t->AVL_Insert(70);
	// 				t->AVL_Insert(100);
	// 				t->AVL_Insert(10);
	// 				t->AVL_Insert(25);
	// 				t->AVL_Insert(35);
	// 				t->AVL_Insert(45);
	// 				t->AVL_Insert(60);
	// 				t->AVL_Insert(32);
	// 			}
	// 			catch(const char *e){
	// 				cerr << e << "\n";
	// 			}
	// 			t->AVL_Print("sample.png");
	// 			cout << "Sample tree is inserted and the sample file is generated for your reference\n";
	// 			break;
	// 		default:
	// 			cout << "Invalid option entered!!\n";
	// 			cout << "Choice 1: Insert\n" << "Choice 2: Delete\n" << "Choice 3: Search\n";
	// 			cout << "Choice 4: Print tree\n" << "Choice 5: Get sample tree\n";
	// 			cout << "------------------------------\n";

	// 	}
	// 	cout << "Do you want to continue (Y/N)? ";
	// 	cin >> ch;
	// 	cout << "\n";
	// }while(ch == 'y' || ch == 'Y');

	t->AVL_Insert(50);
	t->AVL_Insert(30);
	t->AVL_Insert(80);
	t->AVL_Insert(20);
	t->AVL_Insert(40);
	t->AVL_Insert(70);
	t->AVL_Insert(100);
	t->AVL_Insert(10);
	t->AVL_Insert(25);
	t->AVL_Insert(35);
	t->AVL_Insert(45);
	t->AVL_Insert(60);
	t->AVL_Insert(32);
	t->AVL_Delete(100);
	t->AVL_Print("sample.png");

	delete t;
	return 0;
}