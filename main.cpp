#include <bits/stdc++.h>
using namespace std;

struct card{
    int value;
    bool isDrawn;
};
card deck[52];
int game_won[4];
int game_lose[4];
void setdeck()
{
    int j=1;
    for (int i=0;i<52;i++)
    {
        deck[i].value=j+i/4; //tao ra 52 la bai co gia tri tu 1->13, moi gia tri 4 la
        deck[i].isDrawn=false;
    }

}
int starting_hand[169];
class player{
private:
    vector<int> hand;
public:
    player()
    {
        //cho boc 2 la o day cx dc
    }

    vector<int> gethand()
    {
        return hand;
    }
    void draw()
    {

       int i=rand()%52; //ngau nhien 1 so tu 0-51
       while (deck[i].isDrawn) i=rand()%52; //neu so do boc roi thi random lai
       hand.push_back(deck[i].value); //them bai vao tay
       deck[i].isDrawn=true; //danh dau la do boc roi
    }
    int calcScore()
    {

        vector<int> temp=hand;
        sort(temp.begin(),temp.end());
         //sort lai bai tren tay roi dem tu lon den be
        int score=0;
        for (int i=temp.size()-1;i>=0;i--)
            {
                if (temp[i]!=1) score+=min(temp[i],10); //ko phai con ace thi cong gia tri nhu thuong, min 10 de nhung con nhu j,k,q la 11 12 13 van tinh value la 10
                else
                    if (score+11<21 || (score+11==21 && i==0)) score+=11; //neu cong 11 ko quat thi cong 11
                    else if (score+10<21||(score+10==21 && i==0)) score+=10; //neu cong 10 ko quat thi cong 10
                    else score+=1; //ko cong 1
            }
        return score;
    }
};
int global_draw_chance=50;
//control viet cac ham dung chien thuat khac nhau trong 1 class luon
class control:public player{
public:
    control()
    {

    }
    void turn1() //chien thuat ngau nhien 50/50
    {
        int draw_count=0;
        while (this->calcScore()<16) {this->draw();draw_count++;} //boc den khi du tuoi
        do
        if (this->calcScore()<18) //xet neu diem duoi 18
        {
            int cha=rand()%2; //ngau nhien 0 hoac 1
            if (cha) {this->draw();draw_count++;} //neu 1 thi boc
        } while (this->calcScore()<16); //th tren tay co A lam boc bai moi khong du tuoi
        if (draw_count>1) global_draw_chance-=(draw_count-1)*5; //neu boc hon 2 la thi giam ti le nguoi tiep theo boc lai
    }
    void turn2() //chien thuat dan non
    {
        int draw_count=0;
        while (this->calcScore()<16) {this->draw();draw_count++;}
        if (draw_count>1) global_draw_chance-=(draw_count-1)*5;
    }
    void turn3() //chien thuat mau lieu
    {
        int draw_count=0;
        while (this->calcScore()<18) {this->draw();draw_count++;}
        if (draw_count>1) global_draw_chance-=(draw_count-1)*5;
    }
    void turn4()
    {
        int draw_count=0;
        while (this->calcScore()<16) {this->draw();draw_count++;}
        do
        if (this->calcScore()<18) //xet neu diem duoi 18
        {
            int cha=(rand()%100) +1; //ngau nhien 1 den 100
            if (cha <= global_draw_chance) {this->draw();draw_count++;} //neu 1 thi boc
        } while (this->calcScore()<16);
        if (draw_count>1) global_draw_chance-=(draw_count-1)*5;
    }
    //ham in bai
    void printhand()
    {
        cout<<"This player hand: \n";
        for (int i=0;i<this->gethand().size();i++) cout<<this->gethand()[i]<<' ';
         cout<<endl;
         cout<<"This player's score: "<<this->calcScore()<<endl;

    }

};
class dealer: public player{
private:
    float counting[11];
    float percent[11];
    int card_remaining;
public:
     dealer()
     {
         //khoi tao dem bai cho dealer
         for (int i=1;i<=9;i++) {counting[i]=4;percent[i]=counting[i]/52;} //tu 1 den 9 co 4 la
         counting[10]=16; //co 16 la gia tri 10
         percent[10]=counting[10]/52;
         card_remaining=52; //moi game co 52 la

     }
     //ham goi de so diem
     int call(player p)
     {
         //printhand();
         int a=p.calcScore();
         int b=this->calcScore();
         if (p.gethand().size()==5 && a<=21 && this->gethand().size()<5) return -1; //nguoi choi ngu linh
         if (this->gethand().size()==5 && b<=21 && p.gethand().size()<5) return 1; //cai ngu linh
         if (p.gethand().size()==5 && this->gethand().size()==5 && a<=21 && b<=21) //ca 2 ngu linh thi so 2 thap hon
         {
             if (a<b) return -1;
             else if (a>b) return 1;
             return 0;
         }
         if ((a>21 && b>21)|| a==b) return 0; //th hoa
         if (a>21) return 1; //th nguoi choi quac cai ko
         if (b>21) return -1; //th cai quac
         if (a<b) return 1; //cai lon hon return 1 ko return -1
         return -1;
     }
     //ham goi moi khi boc bai de tinh lai ti le
     void counting_card(int x)
     {
         counting[min(x,10)]--; //- sl la do
         card_remaining--; //- sl bai con lai
         for (int i=1;i<=10;i++) percent[i]=counting[i]/card_remaining; //tinh %
     }
     //ham in bai
     void printhand()
     {
         cout<<"dealer's hand: ";

        for (int i=0;i<this->gethand().size();i++) cout<<this->gethand()[i]<<' ';
         cout<<endl;
         cout<<"dealer's score: "<<this->calcScore()<<endl;
     }
     //ham luot choi cua dealer, se goi nguoi choi va tra ve 1 so nguyen la sl nguoi choi thang, co the am neu cai thua
     int turn(control plist[],int player_num)
     {

         int result=0;

         while (this->calcScore()<15) {this->draw(); counting_card(this->gethand()[this->gethand().size()-1]);} //boc bai den khi du tuoi + thuc hien dem bai cho bai moi boc dc
         //this->printhand();

          if (this->calcScore()>21) //neu quac thi goi het cac nha de tinh diem
         {
             for (int i=0;i<player_num;i++)
             {
                 int te=call(plist[i]); //thuc hien so sanh diem xem thang thua
                    //cong kq
                    result+=te;
                    if (te<0) game_won[i]++;
                    else if (te>0) game_lose[i]++;
                  /*  if (te<0) cout<<"P"<<i+1<<" win\n";
                    else if (te>0) cout<<"P"<<i+1<<" lose\n";
                    else cout<<"P"<<i+1<<" draw\n"; */
             }
         }
         else
         {
            bool called[player_num];
            for (int i=0;i<player_num;i++) called[i]=false; //bool check nhung ai goi roi
            //for dau uu tien goi nhung nguoi nhieu la vi de quac
             for (int i=0;i<player_num;i++)
             {
             if (plist[i].gethand().size()>3 && !called[i]) { //xet nguoi nhieu hon 3 la va chua goi
                called[i]=true;
                vector<int> f=plist[i].gethand(); //coi bai tren tay ho
                for (int j=0;j<f.size();j++)  counting_card(f[j]); //roi dem nhung la do

                   int te=call(plist[i]); //thuc hien so sanh diem xem thang thua
                    //cong kq
                    result+=te;
                     if (te<0) game_won[i]++;
                    else if (te>0) game_lose[i]++;
                  /*  if (te<0) cout<<"P"<<i+1<<" win\n";
                    else if (te>0) cout<<"P"<<i+1<<" lose\n";
                    else cout<<"P"<<i+1<<" draw\n"; */

                }
             }
             float chance=0;
            do{
                int t=21-this->calcScore();
                if (this->calcScore()>21) break;
                chance=0;
                for (int i=1;i<=t;i++) chance+=percent[i];
               // cout<<chance<<endl;
                if (chance>=0.5) {this->draw();counting_card(this->gethand()[this->gethand().size()-1]);}
            } while (chance>=0.5);
                   // if (this->calcScore()<=17 && this->gethand().size()==4) this->draw();
            for (int i=0;i<player_num;i++)
            {
                if (!called[i] && plist[i].gethand().size()==3)
                {
                  called[i]=true;
                    vector<int> f=plist[i].gethand();
                    for (int j=0;j<f.size();j++)  counting_card(f[j]);
                     int te=call(plist[i]);
                    result+=te;
                     if (te<0) game_won[i]++;
                    else if (te>0) game_lose[i]++;
                  /*  if (te<0) cout<<"P"<<i+1<<" win\n";
                    else if (te>0) cout<<"P"<<i+1<<" lose\n";
                    else cout<<"P"<<i+1<<" draw\n"; */
                }


            }
            do{
                int t=21-this->calcScore();
                if (this->calcScore()>21) break;
                chance=0;
                for (int i=1;i<=t;i++) chance+=percent[i];
               // cout<<chance<<endl;
                if (chance>=0.5) {this->draw();counting_card(this->gethand()[this->gethand().size()-1]);}
            } while (chance>=0.5);
            while (this->calcScore()<17) this->draw();
            if (this->calcScore()<=17 && this->gethand().size()==4) this->draw();
            for (int i=0;i<player_num;i++)
            {
                if (!called[i])
                {
                    int te=call(plist[i]);
                    result+=te;
                     if (te<0) game_won[i]++;
                    else if (te>0) game_lose[i]++;
                   // if (te<0) cout<<"P"<<i+1<<" win\n";
                   // else if (te>0) cout<<"P"<<i+1<<" lose\n";
                   // else cout<<"P"<<i+1<<" draw\n";
                }
            }
         }
         //cout<<result<<endl;
       // printhand();
         return result;
     }

};
//ham boc bai ban dau
void init_draw(dealer &d, control plist[],int pnum)
{
    for (int i=0;i<2;i++)
    {
        for (int j=0;j<pnum;j++) plist[j].draw();
        d.draw();
    }
}
int main()
{

   srand(time(0)); //de moi lan chay 1 seed ngau nhien
    int d_won=0,d_lose=0;
    for (int j=0;j<100000;j++)
    {
        setdeck(); //reset lai bo bai
        control pp[4];
    dealer d;
    init_draw(d,pp,4);
    for (int i=0;i<4;i++)
    {
        vector<int> tHand=pp[i].gethand();
        int k=0;
        for (int i=1;i<=13;i++)
        {
            for (int j=i;j<=13;j++)
            {
                if ((tHand[0]==i && tHand[1]==j) || (tHand[1]==i && tHand[0]==j)) starting_hand[k]++;
                k++;
            }

        }

    }
    for (int i=0;i<4;i++)
    {
        //turn 1 la 50/50
        //turn 2 la dan non
        //turn 3 la mau lieu
        //turn 4 la chien thuat neu nguoi trc do boc nhieu thi giam ti le boc lai
        if (i==0) pp[i].turn1();
        else if (i==1) pp[i].turn1();
        else if (i==2) pp[i].turn1();
        else pp[i].turn4();

    }
    int res=d.turn(pp,4);
      if (res>0) d_won++; //dealer thang game do thi cong 1
      else if (res<0) d_lose++;

    }
    for (int i=0;i<4;i++)
    {
        cout<<"win rate cua P"<<i+1<<": "<<float(game_won[i])/(game_lose[i]+game_won[i]) * 100<<"%\n";
    }
    cout<<"win rate cua cai: "<<float(d_won)/(d_won+d_lose) * 100<<"%\n"; //in so game dealer thang

    cout<<"thong ke 2 la ban dau boc duoc: \n";
    int k=0;
    string h[14]={"0","A","2","3","4","5","6","7","8","9","10","J","Q","K"};

    for (int i=1;i<=13;i++)
        {
            for (int j=i;j<=13;j++)
            {
                cout<<"So lan 2 la: "<<h[i]<<" va "<<h[j]<<" duoc boc la: "<<starting_hand[k]<<"\n";

                k++;
            }

        }

}
