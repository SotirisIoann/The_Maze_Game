#include "lib.h"

struct Player{
    int Points;
    float Health;
    int Accuracy;
    float Armor;
    float Attack;
    int Level;
};

struct Monsters{
    float Attack;
    int Accuracy;
    float Health;
    float Armor;
};

struct Boss{
    float Attack;
    int Accuracy;
    float Health;
    float Armor;
};

int stage = 1;
int xi = 0,xj = 1;
int w,h;

char game(char **maze,char mv,struct Player *p1);
void print_maze(char** maze);
char move();
void init_Player(struct Player *p1,int stage);
void init_Monster(struct Monsters *m,int stage);
void init_Boss(struct Boss *b,int stage);
char** init_Map();
void Starting_Screen();
void Shop(struct Player *p1);
int fight_Monsters(struct Player *p1);
int fight_Dat_Boss(struct Player *p1);
void print_Table(struct Player *p1,char** maze);

int main(){
    Starting_Screen();
    char mv;
    char temp;
    struct Player p1;
    char **maze;
    
    init_Player(&p1,stage); 
    
    while( stage <=10 ){
        maze = init_Map();
        maze[0][1] = 'S';
        xi = 0,xj = 1;
        
        while(1){
            print_Table(&p1,maze);
            mv = move();
            temp = game(maze,mv,&p1);
            if( mv == 'p' ) Shop(&p1);
            if( temp == 'n' ) {
                FREE_ALLOCATED_SPACE(maze);
                stage++;    
                break;
            }
            if( mv == '.' ) break;
            if( temp == 'e' ) break;  
        }
        if( mv == '.' ) break;
        if( temp == 'e' ) init_Player(&p1,stage);
    }

    if( stage == 10 ){
        printf("You won\n");
    }
    FREE_ALLOCATED_SPACE(maze);
    printf("\n");
    return 0;
}

char** init_Map(){
    char** maze;
    if( stage <= 10 ){
        w = 19+(stage - 1)*2,h = 29+stage*4;
    }

    maze = Generate_Maze(w,h);
    Generate_Objectives(maze,w,h,stage);
    return maze;
}

void init_Boss(struct Boss *b,int stage){
    b->Health = 200.0+ stage*35.0;
    b->Armor = 40.0 + stage*2;
    b->Attack = 45 + stage*5.0;
    b->Accuracy = 65;
}

void init_Monster(struct Monsters *m,int stage){
    m->Health = 100.0 + stage*20.0;
    m->Armor = 35.0 + stage*1.5;
    m->Attack = 35.0 + stage*2.0;
    m->Accuracy = 55;
}

void init_Player(struct Player *p1,int stage){
    p1->Points = 50;
    p1->Health = 650.0;
    p1->Armor = 55.0;
    p1->Attack = 75.0;
    p1->Accuracy = 75;
    p1->Level = 1;
}

void print_maze(char** maze){
    for(int i = 0; i < w; i++){
        for(int j = 0; j < h; j++){
            printf("%c ",maze[i][j]); 
        }
        printf("\n");
    }
}

char game(char **maze,char mv,struct Player *p1){
    int i,j,hit = -1;
    maze[xi][xj] = ' ';
    switch (mv){
        case 'w':{
            i = xi - 1;
            if( i < 0 ){
                maze[xi][xj] = 'S';
                return 'k';
            }else if( maze[i][xj] == '#' ){
                xi = xi;
            }else{
                xi = i;
            }
            if( maze[i][xj] == 'T' ) p1->Points += 10;
            if( maze[i][xj] == 'M' ) {
                hit = fight_Monsters(p1);
                if(hit == 0) return 'e';
                else p1->Points += 10;
            }
            break;
        }case 's':{
            i = xi + 1;
            if( maze[i][xj] == '#' ){
                xi = xi;
            }else if( maze[i][xj] == 'G' ){
                return 'n';
            }else{  
                xi = i;
            }
            if( maze[i][xj] == 'T' ) p1->Points += 10;
            if( maze[i][xj] == 'M' ) {
                hit = fight_Monsters(p1);
                if(hit == 0) return 'e';
                else p1->Points += 10;
            }
            if( maze[i][xj] == 'B' ) {
                hit = fight_Dat_Boss(p1);
                if(hit == 0) return 'e';
                else p1->Points += 10;
            }
            break;
        }case 'a':{
            j = xj - 1;
            if( maze[xi][j] == '#' ){
                xj = xj;
            }else{
                xj = j;
            }
            if( maze[xi][j] == 'T' ) p1->Points += 10;
            if( maze[xi][j] == 'M' ) {
                hit = fight_Monsters(p1);
                if(hit == 0) return 'e';
                else p1->Points += 10;
            }
            break;
        }case 'd':{
            j = xj + 1;
            if( maze[xi][j] == '#' ){
                xj = xj;
            }else{
                xj = j;
            }
            if( maze[xi][j] == 'T' ) p1->Points += 10;
            if( maze[xi][j] == 'M' ) {
                hit = fight_Monsters(p1);
                if(hit == 0) return 'e';
                else p1->Points += 10;
            }
            if( maze[xi][j] == 'B' ) {
                hit = fight_Dat_Boss(p1);
                if(hit == 0) return 'e';
                else p1->Points += 10;
            }
            break;
        }default:
            break;
    }
    maze[xi][xj] = 'S';
    return 'k';
}

char move(){
    char mv;
    system ("/bin/stty raw");
    mv = getchar();
    system ("/bin/stty cooked");
    return mv;
}

void Shop(struct Player *p1){
    system("clear");
    int i;
    printf("Shop\n");
    printf("Your Points : %d\n",p1->Points);
    printf("1.Buy 10 Armor for 20 Points\n2.Buy 15 Attack for 20 Points\n3.Buy 150 Health for 40 Points\n4.Not now...\n");
    scanf("%d",&i);
    getchar();
    switch (i){
    case 1:{
        if( p1->Points >= 20 ){
            p1->Points -= 20;
            p1->Armor += 10.0;
            printf("New Armor : %4.3f\n",p1->Armor);
        }else{
            printf("You don't have the necessary points\n");
        }
        printf("Press Enter to get back to the field\n");
        getchar();
        break;
    }case 2:{
        if( p1->Points >= 20 ){
            p1->Points -= 20;
            p1->Attack += 15.0;
            printf("New Attack : %4.3f\n",p1->Attack);
        }else{
            printf("You don't have the necessary points\n");
        }
        printf("Press Enter to get back to the field\n");
        getchar();
        break;
    }case 3:{
        if( p1->Points >= 40 ){
            p1->Points -= 40;
            p1->Health += 150.0;
            printf("New Health : %4.3f\n",p1->Health);
        }else{
            printf("You don't have the necessary points\n");
        }
        printf("Press Enter to get back to the field\n");
        getchar();
        break;
    }case 4:{
        break;
    }default:
        break;
    }   
}

int fight_Monsters(struct Player *p1){
    struct Monsters m;
    init_Monster(&m,stage);

    time_t t;
	srand((unsigned) time(&t));
    float odd = 0.0;
    float damage;
    int hit = -1;

    do{
        damage = 0.0;
        odd = (rand() % 10001) / 10000.0;
        if( odd <= p1->Accuracy ){
            damage = p1->Attack*(100.0/(100.0 + m.Armor));
            m.Health = m.Health - damage;
        }
        damage = 0.0;
        odd = (rand() % 10001) / 10000.0;
        if( odd <= m.Accuracy ){
            damage = m.Attack*(100.0/(100.0 + p1->Armor));
            p1->Health = p1->Health - damage;
        }
    }while( (m.Health >= 0.0 ) && (p1->Health >= 0.0) );

    if( m.Health <= 0.0 ) hit = 1;
    if( p1->Health <= 0.0 ) hit = 0;

    return hit;
}

int fight_Dat_Boss(struct Player *p1){
    struct Boss b;
    init_Boss(&b,stage);

    time_t t;
	srand((unsigned) time(&t));
    float odd = 0.0;
    float damage;
    int hit = -1;

    do{
        damage = 0.0;
        odd = (rand() % 10001) / 10000.0;
        if( odd <= p1->Accuracy ){
            damage = p1->Attack*(100.0/(100.0 + b.Armor));
            b.Health = b.Health - damage;
        }
        damage = 0.0;
        odd = (rand() % 10001) / 10000.0;
        if( odd <= b.Accuracy ){
            damage = b.Attack*(100.0/(100.0 + p1->Armor));
            p1->Health = p1->Health - damage;
        }
    }while( (b.Health >= 0.0 ) && (p1->Health >= 0.0) );

    if( b.Health <= 0.0 ) hit = 1;
    if( p1->Health <= 0.0 ) hit = 0;

    return hit;
}

void print_Table(struct Player *p1,char** maze){
    system("clear");
    printf("\t\t\tDat Game\n");
    printf("Stage %d\n",stage);
    printf("Level : %d\n",p1->Level);
    printf("Points : %d \t Health : %4.3f \t Attack : %4.3f \t Armor : %4.3f \t Accuracy : %d\n",p1->Points,p1->Health,p1->Attack,p1->Armor,p1->Accuracy);
    printf("\nPress p for Shop\n");
    print_maze(maze);
    printf("Press . to close the game\n");
}

void Starting_Screen(){
    printf("\t\tDat Game\n");
    printf("Rules\n");
    printf("Find the way out\nG is the place that you have to go\nT giving Points\nFight the Monsters and defeat the final Boss\n");
    printf("\n");
    printf("S : You\nT : Thesaurus\nM : Monsters\nB : Boss\nG : Termination\n");
    printf("\n");
    printf("Press . to close the game anytime\n");
    printf("Press Enter to start\n");
    getchar();
}