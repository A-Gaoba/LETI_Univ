#define MAX_ELEMENTS 100

class CustomStack {
public:
// методы push, pop, size, empty, top + конструкторы, деструктор

  // Constructor 1
  CustomStack(int val){
    push(val);
  }

  // Constructor 2
  CustomStack(ListNode* new_head){
    mHead = new_head;
  }

  // Constructor 3
  CustomStack():CustomStack(nullptr){}

  // Desctructor
  ~CustomStack(){
    if (mHead!=nullptr){
      mHead->mNext = nullptr;
    }
    mHead = nullptr;
    delete mHead;
  }

  // empty method
  bool empty(){
    return (mHead==NULL);
  }

  int size(){
    int count = 0;
    ListNode* p = mHead;
    if (empty()) return 0;
    do{
      count++;
    }while((p = p->mNext)!=NULL);
    return count;
  }

  void push(int val){
    ListNode* new_node = new ListNode();
    new_node->mData = val;
    new_node->mNext = mHead;
    mHead = new_node;
  }

  void pop(){
    ListNode* p = mHead;
    if (empty()) {
        cout<<"error"<<endl;
        exit(EXIT_SUCCESS);
    }
    mHead = p->mNext;
    p->mNext = NULL;
    delete p;
  }

  int top(){
    if(empty()){
        cout<<"error\n";
        exit(EXIT_SUCCESS);
    }
    return mHead->mData;
  }

protected: // в этом блоке должен быть указатель на голову
    ListNode* mHead;
};

bool is_operator(char c){
  return (c=='+')||(c=='-')||(c=='/')||(c=='*');
}

bool is_operand(char token){
  return isdigit(token);
}

float calculate(char operation, int operand1, int operand2){
  switch(operation){
    case '+': return operand1+operand2;break;
    case '-': return operand1-operand2;break;
    case '*': return operand1*operand2;break;
    case '/': return operand1/operand2;break;
    default : cout<<"error\n"; exit(EXIT_SUCCESS);
  }
  return -1;
}

int evaluate_expression(string expression){
  char c;
  int elements = 0;
  bool neg = false, is_number = false;
  CustomStack* stack = new CustomStack();
  int result = 0;
  for (int i=0; i<(int)(expression.size()) && elements<MAX_ELEMENTS; i++){
    c = expression[i];
    if (!neg && c=='-') {
        if (i+1 < (int)expression.size() && is_operand(expression[i+1])){
            is_number = true;
        }else{
            i--;
            is_number = false;
        }
        neg = true;
        continue;
    }

    if (is_operand(c)){
      int number = 0;
      i--;
      while(is_operand(c)){
        i++;
        number = number*10 + (c-'0');
        c = expression[i+1];
      }
      if (neg) number = -number;
      elements++;
      stack->push(number);
    }else if(is_operator(c)){
      int op2 = stack->top();stack->pop();
      int op1 = stack->top();stack->pop();
      result = calculate(c, op1, op2);
      elements++;
      stack->push(result);
    }
    if ((c==' ' || c==',' || c=='\t') && neg && !is_number) {
            i--;
    }
    if (neg) neg = false;
  }
  if (stack->size()!=1){
      cout<<"error\n";
      exit(EXIT_SUCCESS);
  }
  result = stack->top();
  delete stack;
  return result;
}

int main() {
    string expression;
    getline(cin, expression);
    int result = evaluate_expression(expression);
    cout<<result<<endl;
    return 0;
}

