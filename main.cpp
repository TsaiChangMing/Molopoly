#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
//123
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

        void setprop(int n){
            this->prop = props.Sign(n);
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
            for(int k = 0; k < 3; k++)  prop[k] = 5;
            locate = head;
            next = NULL;
        }

        void addmoney(int m){
            money += m;
        }

        void addstay(int s){
            stay += s;
        }

        void addprop(int i, int n){
            prop[i] += n;
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
    cout << endl << endl;

    flag = front;
    do{
        cout << flag->getid() << "\t";
        flag = flag->next;
    }while(flag != NULL);
    cout << "\t\t\t";
    if(out != NULL){
        flag = out;
        do{
            cout << flag->getid() << "\t";
            flag = flag->next;
        }while(flag != NULL);
    }
    cout << endl;

    flag = front;
    do{
        cout << flag->getname() << "\t";
        flag = flag->next;
    }while(flag != NULL);
    cout << "\t\t\t";
    if(out != NULL){
        flag = out;
        do{
            cout << flag->getname() << "\t";
            flag = flag->next;
        }while(flag != NULL);
    }
    cout << endl;

    flag = front;
    do{
        cout << flag->getmoney() << "\t";
        flag = flag->next;
    }while(flag != NULL);
    cout << "\t\t\t";
    if(out != NULL){
        flag = out;
        do{
            cout << flag->getmoney() << "\t";
            flag = flag->next;
        }while(flag != NULL);
    }
    cout << endl;
}

void specialPoint(int n){
    Map* specail;
    int m = n-1;
    for(int i = 1; i < m; i++){
        temp = temp->next;
        if(i % (m/4) == 0){
            switch(i/(m/4)){
                case 1:
                    specail = new Map(n++,5,"監獄");
                    break;
                case 2:
                    specail = new Map(n++,6,"商店");
                    break;
                case 3:
                    specail = new Map(n++,4,"醫院");
                    break;
            }
            specail->next = temp->next;
            temp->next = specail;
            temp = temp->next;
        }
        if(i % (m/4) == m/8){
            switch(i/(m/4) % 2){
                case 0:
                    specail = new Map(n++,2,"命運");
                    break;
                case 1:
                    specail = new Map(n++,3,"機會");
                    break;
            }
            specail->next = temp->next;
            temp->next = specail;
            temp = temp->next;
        }
    }
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
        temp->next = new Map(i,1,name);
        temp = temp->next;
        temp->info = new infomation(price,tolls);
        read.getline(in,sizeof(in),'\n');
    }
    temp->next = head;
    temp = head;
    if((i-1) % 4 != 0)  return false;
    specialPoint(i);
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
            cout << "Price:\ndrug\tLandmine\tRoadlock\n";
            cout << " 300\t  500\t\t  1000\n";
            for(int i = 0; i < 3; i++){
                cout << "How many " << prop.Sign(i) << " do you want to buy?";
                cin >> c;
                front->addprop(i,c);
                front->addmoney(prop.Price(i)*(-c));
            }
            break;
        case 5: //Prison
            cout << "Stay in prison for 3 days" << endl;
            front->addstay(3);
            break;
        case 4: //Hospital
            cout << "Spend 200 dollar to be hospitalized for 1 day" << endl;
            front->addstay(1);
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
    int i, dice, step = 0;
    cout << front->getname() << "'s turn:\n";
    cout << "Press any key to throw the dices...";
    fflush(stdin);
    getchar();
    for(i = 0; i < n; i++){
        dice = rand()%6+1;
        cout << dice << " ";
        step += dice;
    }
    cout << "=  " << step << " steps" << endl;
    cout << "Press any key to throw move...";
    getchar();
    for(i = 0; i < step; i++){
        front->locate = front->locate->next;
        if(front->locate->gettype() == 0 && step - i > 1)   //經過起點
            front->addmoney(2000);
        system("CLS");
        Display();
        Sleep(350);
    }
    cout << "Arrive: " << front->locate->getname() << endl;
    act();
}

void setprop(){
    while(true){
        if(front->getprop(0) == 0 && front->getprop(1) == 0 && front->getprop(2) == 0)  break;
        int prop, place;
        char choose;

        cout << "Do you want to set props on the map?";
        do{
            cin >> choose;
            if(choose == 'y' || choose == 'n')  break;
            cout << "Please enter again:";
        }while(true);
        if(choose == 'n')   break;

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
            cout << "You don't have this prop:";
        }while(true);

        while(temp->getnumber() != place)   temp = temp->next;
        temp->setprop(prop);
        cout << "Prop set successful!\nWould you want to set another prop?";
        do{
            cin >> choose;
            if(choose == 'y' || choose == 'n')  break;
            cout << "Please enter again:";
        }while(true);
        if(choose == 'n')   break;
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
    int m, n;
    string name;
    if(!createMap()){
        cout << "Loading map failed\n";
        return 0;
    }/*
    cout << "Set initial money:";
    cin >> */m=50000;
    cout << "How many players:";
    cin >> n;
    createPlayer(m,n);
    while(n != 1){
        system("CLS");
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
    system("CLS");
    Display();
    return 0;
}
