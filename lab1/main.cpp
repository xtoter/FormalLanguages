#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
using namespace std;
vector<char> noterminals;
vector<char> terminals;
void read(vector<char> *noterminals,  vector<char> *terminals,  map <char,vector<string>> *data){ //чтение данных
    char c;
    scanf("nonterminals = %c", &c);
    noterminals->push_back(c);
    while (true) {
        scanf("%c", &c);
        if (c == '\n') {
            break;
        }
        if (c == ' ' || c == ','){
            continue;
        }
        noterminals->push_back(c);
    }

    scanf("terminals = %c", &c);
    terminals->push_back(c);
    while (true) {
        scanf("%c", &c);
        if (c == '\n') {
            break;
        }
        if (c == ' ' || c == ','){
            continue;
        }
        terminals->push_back(c);
    }

    while (true) {
        string temp;
        scanf("%c", &c);
        if (c == 10) {
            break;
        }
        scanf(" -> ");
        getline(cin, temp);
        (*data)[c].push_back(temp);
    }
}
bool IsNonterminal(char in){ //Проверка на нетерминал
    for (int i=0;i< noterminals.size();i++){
        if (in == noterminals[i]){
            return true;
        }
    }
  return false;
}
bool IsTerminal(char in){ //проверка на терминал
    for (int i=0;i< terminals.size();i++){
        if (in == terminals[i]){
            return true;
        }
    }
    return false;

}
void outvec(vector<char> a){ //Вывод вектора char (дебаг)
    for (int i=0;i<a.size();i++){
        cout << a[i] << " ";
    }
    cout << "\n";
}
string delete_(string in) { //Замена нетерминалов на _
    for (int i=0;i<in.length();i++) {
       if ( IsNonterminal(in[i])) {
         in[i]='_';
       } else if (!IsTerminal(in[i])) {
           cout << "ERROR: Strange symbol";
       }
    }
    return in;
}
vector<string> deleteduplicate(vector<string> in){ //Функция удаления дупликатов
    vector<string> out;
    map <string ,bool> was;
    for (int i=0;i<in.size();i++){
        if (!was[in[i]]){
            was[in[i]]= true;
            out.push_back(in[i]);
        }
    }
    return out;
}
map <char,vector<string>> delete_frommap(   map <char,vector<string>> a){ //Удаление нетерминалов в 1 шаг
    map <char,vector<string>> stepone;
    map <char, vector<string>> :: iterator it = a.begin();
    for (int i = 0; it != a.end(); it++, i++) {
        for (int i=0;i<it->second.size();i++){
            stepone[it->first].push_back(delete_(it->second[i]));
        }
        stepone[it->first]=deleteduplicate(stepone[it->first]);
    }
    return stepone;
}
void outmap(   map <char,vector<string>> a){ //Вывод map (дебаг)
    map <char, vector<string>> :: iterator it = a.begin();
    for (int i = 0; it != a.end(); it++, i++) {
        cout << it->first << " ";
        for (int i=0;i<it->second.size();i++){
            cout << it->second[i] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}
map <vector<string>,vector<char>> getmap_(   map <char,vector<string>> a){ //Смена формата map
    map  <vector<string>,vector<char>> b;
    map <char, vector<string>> :: iterator it = a.begin();
    for (int i = 0; it != a.end(); it++, i++) {
        b[it->second].push_back(it->first);
    }
    return b;
}
vector<vector<char>> firstpartition(map <vector<string>,vector<char>> firstelems){ //поиск начальных разбиений
    vector<vector<char>> coset;
    map <vector<string>,vector<char>> :: iterator it = firstelems.begin();
    for (int i = 0; it != firstelems.end(); it++, i++) {
        for (int i=0;i<it->second.size();i++){
            if (i==0){
                coset.push_back(vector<char> (1, it->second[i]));
            } else {
                coset[coset.size()-1].push_back( it->second[i]);
            }
        }

    }
    return coset;
}
int searchelem(vector<vector<char>> in,char a){ //возврат номера для элемента из разбиений
    for (int i=0;i<in.size();i++){
        for (int j=0;j<in[i].size();j++){
            if (in[i][j] == a){
                return i;
            }
        }
    }
    return -1;
}
vector<vector<string>> out;
bool checkposl(vector<vector<char>> *in,int index,map <char,vector<string>> data){ //Проверка верно ли разбиение

    map <vector<string>,vector<char>> newdata;
    for (int i=0;i<(*in)[index].size();i++){ //Буквы
        char symb = ((*in)[index][i]);
        vector<string> curvector = data[symb];
        for (int j=0;j<curvector.size();j++) {

            for (int k = 0; k < curvector[j].size(); k++) {
                if (IsNonterminal(curvector[j][k])) {
                    curvector[j][k] = '0' + searchelem(*in, curvector[j][k]);
                }
            }
        }
        curvector = deleteduplicate(curvector);
        newdata[curvector].push_back(symb);
      //  cout << "string " << symb << "\n";
    };

    map <vector<string>,vector<char>> :: iterator it = newdata.begin();
    std::string s;
    s += it->second[0];
    out.push_back(vector<string> (1,s));
    if (newdata.size()<2){
        //cout << it->second[0] << "axaxx";
        for (int j=0;j<it->first.size();j++){
            out[out.size()-1].push_back( it->first[j] );
        }
        return true;
    }
    //it++;
    for (int i = 0; it != newdata.end(); it++, i++) {
        for (int k=0;k<it->second.size();k++){
            if (i==0){
                if (k==0){
                    (*in)[index] = vector<char> (1,it->second[k]);
                } else {
                    (*in)[index].push_back(it->second[k]);
                }
            } else {
                if (k==0){
                    (*in).push_back(vector<char> (1,it->second[k]));
                } else {
                    (*in)[(*in).size()-1].push_back(it->second[k]);
                }
            }
        }
    }
    return false;

}
void addnull(vector<vector<char>> *in){ //Добавление Нетерминалов без правой части
    map <char,bool> allelems;
    for (int i=0;i<noterminals.size();i++){
        allelems[noterminals[i]]= true;
    }
    for (int i=0;i<(*in).size();i++) { //вектор с элементами разбиения
        for (int j = 0; j < (*in)[i].size(); j++) {
            allelems[(*in)[i][j]]= false;
        }
    }
    map <char,bool> :: iterator it = allelems.begin();
    bool k =true;
    for (int i = 0; it != allelems.end(); it++, i++) {
        if (it->second == true)
        {
            if (k){
                (*in).push_back(vector<char> (1,it->first));
                k= false;
            } else {
                (*in)[(*in).size()-1].push_back(it->first);
            }
        }
    }
}
string replacenum(string in,vector<vector<char>> clas){ //Замена номера на разбиение
    for (int i=0;i<in.length();i++) {
        if ( !IsTerminal(in[i]) && in[i]>='0') {
            //cout << in[i]-'0';
            in[i]=clas[in[i]-'0'][0];
        }
    }
    return in;
}
void outgramma(vector<vector<char>> in){ //Вывод результата
    for (int i=0;i<out.size();i++){
        string symbol = out[i][0];
        for (int j=1;j<out[i].size();j++) {
            cout << symbol << " -> "<< replacenum(out[i][j],in) <<"\n";
           // cout << out[i][j] << " ";
        }
        cout << "\n";
    }

}
int main() {
    map <char,vector<string>> data;
    read(&noterminals,&terminals,&data);

    map <char,vector<string>> withoutnonterminal = delete_frommap(data); //Данные с _
    vector<vector<char>> coset = firstpartition(getmap_(withoutnonterminal)); //Разбиения
    addnull(&coset);

    for (int i=0;i<coset.size();i++){ //вектор с элементами разбиения
        while (!checkposl(&coset,i,data)) { // проверка на правильность разбиений
        }
    }
    cout << "class\n"; //вывод разбиения
    for (int i=0;i<coset.size();i++) { //вектор с элементами разбиения
        for (int j = 0; j < coset[i].size(); j++) {
            cout << coset[i][j]<< " " ;
        }
        cout << "\n";
    }
    cout << "result\n";
    outgramma(coset); //Вывод итоговой грамматики
    return 0;
}
