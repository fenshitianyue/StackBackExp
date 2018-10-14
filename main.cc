#include <iostream>
#include <string>
#include <deque>
#include <cassert>
using namespace std;

#define SIZE(arr) ((sizeof(arr)) / sizeof(arr[0]))

//辅助函数
int find(char arr[], int size, char ch){
	for (int i = 0; i < size; ++i){
		if (arr[i] == ch){
			return i + 1;
		}
	}//end for
  return 404;
}//end find()

bool shell_gt(char ch1, char ch2){
	char priority_table[] = { '+', '-', '*', '/', '(', ')'};
	int left = find(priority_table, SIZE(priority_table), ch1);
	int right = find(priority_table, SIZE(priority_table), ch2);
	return left > right ? true : false;
}

//表达式类
class ExpResult{
public:
	ExpResult() :_exp(""), _result(0.0) {}
	ExpResult(string exp) : _exp(exp), _result(0.0) {};
	//void PutExp(string exp); //设置中缀表达式 TODO
	//void Print(); //打印中缀表达式 TODO
	void ChangeExp(); //将中缀表达式转换成后缀表达式
	void CalcResult(); //计算表达式的值
	double GetResult(); //得到表达式的值
private:
	deque<char> _BackExp; //后缀表达式栈
	deque<char> _OperStack; //临时存放操作符的栈
	string _exp; //表达式
	double _result; //中缀表达式的结果
};

void ExpResult::ChangeExp(){
	int size = _exp.size();
	for (int i = 0; i < size; ++i){
		//如果当前字符为数字(0-9)，则压入数字栈
		if (_exp[i] >= 48 || _exp[i] <= 57){ 
			_BackExp.push_back(_exp[i]);
		}
		else{
			//如果当前操作符栈为空，则直接压入栈中
			if (_OperStack.empty()){
				_OperStack.push_back(_exp[i]);
			}
			//如果不为空，则考虑几种情况：
			else{
				//如果栈顶元素为'(', 或者当前字符比栈顶操作符优先级高，则将
				//当前字符直接压入操作符栈中
				if (_OperStack.back() == '(' || shell_gt(_exp[i], _OperStack.back()) ){
					_OperStack.push_back(_exp[i]);
				}
				//如果当前字符为')',则将除了'('之后的除了')'的所有操作符压入表达式栈中
				//然后将'('从操作符栈中弹出
				else if (_exp[i] == ')'){
					while (_OperStack.back() != '('){
						_BackExp.push_back(_OperStack.back());
						_OperStack.pop_back();
					}
					_OperStack.pop_back(); //弹出操作符栈中的 '('
				}
				//如果栈顶元素不为'('并且当前字符比栈顶元素优先级高，则不断将操作符
				//栈中的元素压入到表达式栈中，直到不满足上面的两个条件之一，此时
				//将当前字符压入到操作符栈中
				else{
					while (_OperStack.back() != '(' && !shell_gt(_exp[i], _OperStack.back())){
						_BackExp.push_back(_OperStack.back());
						_OperStack.pop_back();
					}
					_OperStack.push_back(_exp[i]);
				} //end else_3
			} //end else_2
		} //end else_1
	} //end for()
	//已经遍历完了中缀表达式，此时将操作符栈中的所有元素都压入到表达式栈中
	while (!_OperStack.empty()){
		_BackExp.push_back(_OperStack.back());
		_OperStack.pop_back();
	}
	//此时表达式栈中放的就是转换好的后缀表达式
} //end CalcResult()

void ExpResult::CalcResult(){
	deque<int> NumStack;
	//从头到尾遍历后缀表达式
	for (int i = 0; i < static_cast<int>(_BackExp.size()); ++i){
		if (_exp[i] >= 48 || _exp[i] <= 57){
			NumStack.push_back('0' + _exp[i]); //将0~9字符转换成数字压入数字栈中
			continue;
		}
		int right = NumStack.back();
		NumStack.pop_back();
		int left = NumStack.back();
		NumStack.pop_back();
		switch (_exp[i]){ //如果是运算符，则从数字栈中弹出两个数据进行运算，将结果压入数字栈中
		case '+':
			NumStack.push_back(left + right);
			break;
		case '-':
			NumStack.push_back(left - right);
			break;
		case '*':
			NumStack.push_back(left * right);
			break;
		case '/':
			NumStack.push_back(left / right);
			break;
		default:
			assert(0);
			break;
		} //end switch(_exp[i])
	} //end for
	_result = NumStack.back();
}

double ExpResult::GetResult(){
	return _result;
}

int main(){
	
	getchar();
	return 0;
}
