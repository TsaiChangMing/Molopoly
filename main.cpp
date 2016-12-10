#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>

#define origiMoney 10000
#define passStart 500
#define stayPrison 3
#define stayHospital 1

using namespace std;

struct Prop{
    private:
        char sign[3] = {'@', '*', '#'};
        string name[3] = {"Drug", "Landmine", "Roadblock"};
        int price[3] = {300, 500, 1000};
    public:
        char Sign(int i){
            return sign[i];
        }
        string Name(int i){
            return name[i];
        }
        int Price(int i){
            return price[i];
        }
};
Prop props;

struct infomation{
    private:
        int price[5];   //建設費用
        int tolls[5];   //過路費
        int level;      //建設等級
        char owner;     //擁有者
    public:
        infomation(int p[], int t[]){
            for(int i = 0; i < 5; i++){
                price[i] = p[i];
                tolls[i] = t[i];
            }
            level = -1;
            owner = '\0';
        }

        void initial(char id){
            if(owner == id){
                level = -1;
                owner = '\0';
            }
        }

        void setowner(char o){
            owner = o;
        }

        void levelup(){
            level++;
        }

        int getprice(){
            return price[level+1];
        }

        int gettolls(){
            if(level == -1)  return 0;
            return tolls[level];
        }

        int getlevel(){
            return level;
        }

        char getowner(){
            return owner;
        }
};

struct Map{
    private:
        int number;     //編號
        int type;       //起點:0 空地:1 機會:2 命運:3 醫院:4 監獄:5 商店:6
        string name;    //地名
        char prop;
    public:
        Map* next;
        infomation* info;
        Map(int num, int t, string n){
            number = num;
            type = t;
            name = n;
            prop = NULL;
            next = NULL;
            info = NULL;
        }

        int getnumber(){
            return number;
        }

        int gettype(){
            return type;
        }

        string getname(){
            return name;
        }

        char getprop(){
            return prop;
        }

        void setprop(char p){
            this->prop = p;
        }
};

Map* head = new Map(0, 0, "起點");
Map* temp = head;

struct player{
    private:
        char id;
        string name;
        int money;
        int stay;
        int prop[3];
    public:
        Map* locate;
        player* next;
        player(char i, string n, int m){
            id = i;
            name = n;
            money = m;
            stay = 0;
            for(int k = 0; k < 3; k++)  prop[k] = 0;
            locate = head;
            next = NULL;
        }

        void addmoney(int m){
            this->money += m;
        }

        void addstay(int s){
            this->stay += s;
        }

        void addprop(int i, int n){
            this->prop[i] += n;
        }

        char getid(){
            return id;
        }

        string getname(){
            return name;
        }

        int getmoney(){
            return money;
        }

        int getstay(){
            return stay;
        }

        int getprop(int i){
            return prop[i];
        }
};

player* front = NULL;
player* rear = NULL;
player* flag = NULL;
player* out = NULL;

void Display(){
    temp = head;
    system("CLS");
    cout << endl << "\t\t\t\t\t\t\t\t";
    flag = front;
    do{
        cout << flag->getid() << "\t\t";
        flag = flag->next;
    }while(flag != NULL);
    cout << "\t\t\t";
    if(out != NULL){
        flag = out;
        do{
            cout << flag->getid() << "\t\t";
            flag = flag->next;
        }while(flag != NULL);
    }
    cout << endl << "\t\t\t\t\t\t\t\t";

    flag = front;
    do{
        cout << flag->getname() << "\t\t";
        flag = flag->next;
    }while(flag != NULL);
    cout << "\t\t\t";
    if(out != NULL){
        flag = out;
        do{
            cout << flag->getname() << "\t\t";
            flag = flag->next;
        }while(flag != NULL);
    }
    cout << endl << "\t\t\t\t\t\t\t\t";

    flag = front;
    do{
        cout << flag->getmoney() << "\t\t";
        flag = flag->next;
    }while(flag != NULL);
    cout << "\t\t\t";
    if(out != NULL){
        flag = out;
        do{
            cout << flag->getmoney() << "\t\t";
            flag = flag->next;
        }while(flag != NULL);
    }
    cout << endl << "\t\t\t\t\t\t\t\t";
    flag = front;
    do{
        for(int i = 0; i < 3; i++)
            cout << props.Sign(i) << ":" << flag->getprop(i) << " ";
        cout << "\t";
        flag = flag->next;
    }while(flag != NULL);
    cout << "\t\t\t";
    if(out != NULL){
        flag = out;
        do{
            for(int i = 0; i < 3; i++)
                cout << props.Name(i) << ":" << flag->getprop(i) << " ";
            cout << "\t";
            flag = flag->next;
        }while(flag != NULL);
    }

    cout << endl << front->getname() << "'s turn:" << endl << endl;

    do{
        if(temp->gettype() == 1){
            if(temp->info->getowner() != '\0')
                cout << " " << temp->info->getowner() << temp->info->getlevel()+1 << "   ";
            else
                cout << "Lv." << temp->info->getlevel()+1 << "  ";
        }
        else    cout << "      ";
        temp = temp->next;
    }while(temp != head);
    cout << endl;

    do{
        cout << temp->getname() << "  ";
        temp = temp->next;
    }while(temp != head);
    cout << endl;

    do{
        int count = 0;
        if(temp->getprop() != NULL){
            cout << temp->getprop();
            count++;
        }

        flag = front;
        do{
            if(temp->getnumber() == flag->locate->getnumber()){
                cout << flag->getid();
                count++;
            }
            flag = flag->next;
        }while(flag != NULL);
        for(int i = 0; i < 6-count; i++)    cout << " ";
        temp = temp->next;
    }while(temp != head);
    cout << endl << endl << endl;
}

bool createMap(){
    string name;
    int i, j, price[5], tolls[5];
    char in[100];
    fstream read;
    read.open("map.txt",ios::in);
    if(!read)   return false;
    for(i = 1; read.getline(in,sizeof(in),'\t'); i++){
        name = in;
        for(j = 0; j < 5; j++){
            read.getline(in,sizeof(in),'\t');
            price[j] = atoi(in);
        }
        for(j = 0; j < 5; j++){
            read.getline(in,sizeof(in),'\t');
            tolls[j] = atoi(in);
        }
        if(price[0] != 0){
            temp->next = new Map(i,1,name);
            temp = temp->next;
            temp->info = new infomation(price,tolls);
        }
        else{
            if(name == "機會")    temp->next = new Map(i,2,name);
            if(name == "命運")    temp->next = new Map(i,3,name);
            if(name == "醫院")    temp->next = new Map(i,4,name);
            if(name == "監獄")    temp->next = new Map(i,5,name);
            if(name == "商店")    temp->next = new Map(i,6,name);
            temp = temp->next;
        }
        read.getline(in,sizeof(in),'\n');
    }
    temp->next = head;
    temp = head;
    if(i % 4 != 0)  return false;
    temp = head;
    return true;
}

void createPlayer(int money, int n){
    string name;
    cout << "Enter player's name" << endl;
    for(int i = 0; i < n; i++){
        cout << "P" << i+1 << ":";
        cin >> name;
        if(i == 0){
            front = new player(i+65,name,money);
            rear = front;
        }
        else{
            rear->next = new player(i+65,name,money);
            rear = rear->next;
        }
    }
}

void act(){
    Prop prop;
    int c;
    char choose;
    switch(front->locate->gettype()){
        case 6: //Store
            cout << "Price:\n  drug\t\tLandmine\tRoadlock\n";
            cout << "  300\t\t  500\t\t  1000\n";
            for(int i = 0; i < 3; i++){
                cout << "How many " << prop.Name(i) << " do you want to buy?";
                cin >> c;
                front->addprop(i,c);
                front->addmoney(prop.Price(i)*(-c));
            }
            break;
        case 5: //Prison
            cout << "Stay in prison for 3 days" << endl;
            front->addstay(stayPrison);
            break;
        case 4: //Hospital
            cout << "Spend 200 dollar to be hospitalized for 1 day" << endl;
            front->addstay(stayHospital);
            front->addmoney(-200);
            break;
        case 3: //Fortune
            break;
        case 2: //Chance
            break;
        case 1: //land
            //無主
            if(front->locate->info->getowner() == '\0'){
                if(front->getmoney() > front->locate->info->getprice()){
                    cout << "This land's price is " << front->locate->info->getprice() << endl;
                    cout << "After you buy it, you still have " << front->getmoney()-front->locate->info->getprice() << " dollars" << endl;
                    cout << "Wanna buy the land?(y/n):";
                    fflush(stdin);
                    cin >> choose;
                    if(choose == 'y'){
                        front->addmoney(-front->locate->info->getprice());
                        front->locate->info->setowner(front->getid());
                        front->locate->info->levelup();
                    }
                }
                else{
                    cout << "Not enough money to buy the land" << endl;
                    cout << "land price :" << front->locate->info->getprice() << endl;
                }

            }
            //為主
            else if(front->locate->info->getowner() == front->getid()){
                if(front->locate->info->getlevel() == 5){
                    cout << "The level is highest!!" << endl;
                }
                else if(front->getmoney() > front->locate->info->getprice()){
                    cout << "Upgrade this land's price is " << front->locate->info->getprice() << " dollars" << endl;
                    cout << "After you upgrade it, you still have " << front->getmoney()-front->locate->info->getprice() << " dollars" << endl;
                    cout << "Wanna upgrade the land?(y/n):";
                    fflush(stdin);
                    cin >> choose;
                    if(choose == 'y'){
                        front->addmoney(-front->locate->info->getprice());
                        front->locate->info->levelup();
                    }
                }
                else{
                    cout << "Not enough money to upgrade the land" << endl;
                    cout << "land price :" << front->locate->info->getprice() << endl;
                }
            }
            //他主
            else if(front->locate->info->getowner() != '\0'){
                front->addmoney(-front->locate->info->gettolls());
                flag = front;
                while(flag->getid() != front->locate->info->getowner())
                    flag = flag->next;
                flag->addmoney(front->locate->info->gettolls());
                cout << "The land is belong to " << front->locate->info->getowner() << endl;
                cout << "You should pay " << front->locate->info->gettolls() << " dollars" << endl;
            }
    }
}

void move(int n){
    Display();
    int i, dice, step = 0;
    cout << "Press any key to throw the dices...";
    fflush(stdin);  _getch();
    cout << endl;
    for(i = 0; i < n; i++){
        dice = rand()%6+1;
        cout << dice << " ";
        step += dice;
    }
    cout << "=  " << step << " steps" << endl;
    cout << "Press any key to throw move...";
    fflush(stdin);  _getch();
    for(i = 0; i < step; i++){
        front->locate = front->locate->next;
        if(front->locate->gettype() == 0 && step - i > 1)   //經過起點
            front->addmoney(passStart);
        Display();
        if(front->locate->getprop() == '#'){
            front->locate->setprop(NULL);
            cout << "There is a roadblock in front of you, you have no idea how to pass it\n\n";
            break;
        }
        Sleep(350);
    }
    cout << "Arrive: " << front->locate->getname() << endl;
    if(front->locate->getprop() == '@'){
        front->locate->setprop(NULL);
        while(front->locate->getname() != "監獄") front->locate = front->locate->next;
        cout << "A police saw you take a pack of drug, thus you have been sent to prison!!\n\n";
    }
    if(front->locate->getprop() == '*'){
        front->locate->setprop(NULL);
        while(front->locate->getname() != "醫院") front->locate = front->locate->next;
    }
    act();
}

void setprop(){

    int prop, place;
    char choose;
    if(front->getprop(0) != 0 || front->getprop(1) != 0 || front->getprop(2) != 0){
        cout << "Do you want to set props on the map?";
        do{
            cin >> choose;
            if(choose == 'y' || choose == 'n')  break;
            cout << "Please enter again:";
        }while(true);
        while(choose == 'y'){
            cout << "Where do you want to put?";
            do{
                cin >> place;
                if(place > -1 && place < 27)  break;
                cout << "Please enter again:";
            }while(true);

            cout << "Whitch prop do you want to put?";
            do{
                cin >> prop;
                if(prop < 0 || prop >2){
                    cout << "Please enter a number between 0 and 3:";
                    continue;
                }
                if(front->getprop(prop) != 0)  break;
                cout << "You don't have this prop, please choose another prop:";
            }while(true);

            while(temp->getnumber() != place)   temp = temp->next;
            if(temp->getprop() != NULL){
                Display();
                cout << "Prop set failed, there has a prop already!!\n";
            }
            else{
                temp->setprop(props.Sign(prop));
                front->addprop(prop, -1);
                Display();
                cout << "Prop set successful!\n";
            }
            cout << "Would you want to set another prop?";
            do{
                cin >> choose;
                if(choose == 'y' || choose == 'n')  break;
                cout << "Please enter again:";
            }while(true);
        }
    }
    move(2);
}

bool switchplayer(){
    flag = front;
    front = front->next;
    if(flag->getmoney() < 0){
        if(out == NULL){
            out = flag;
            out->next = NULL;
        }
        else{
            flag->next = out;
            out = flag;
        }
        return false;
    }
    else{
        rear->next = flag;
        rear = flag;
        rear->next = NULL;
        return true;
    }
}

void initialmap(player* o){
    temp = head;
    do{
        if(temp->gettype() == 1)
            temp->info->initial(o->getid());
        temp = temp->next;
    }while(temp != head);
}

int main(){
    srand(time(NULL));
    int n, m = origiMoney;
    string name;
    if(!createMap()){
        cout << "Loading map failed\n";
        return 0;
    }
    cout << "How many players:";
    cin >> n;
    createPlayer(m,n);
    while(n != 1){
        Display();
        if(front->getstay() != 0){
            cout << front->getname() << " still stay " << front->getstay() << " days\n";
            front->addstay(-1);
        }
        else    setprop();
        if(!switchplayer()){
            initialmap(out);
            n--;
        }
        system("pause");
    }
    Display();
    return 0;
}
