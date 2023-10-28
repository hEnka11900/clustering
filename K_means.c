#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h> //乱数を扱う際にコンピュータの時間を使う
#include <stdbool.h> //ブーリアンを使う

#define MATRIX_DIM 20
#define NUM_OF_CLUSTERS 9
#define LIMIT 20

struct cordinate {
    int x;
    int y;
};

void init_map(int received_matrix[MATRIX_DIM][MATRIX_DIM]);
void init_vec(int received_vector[NUM_OF_CLUSTERS]);

void generate_random_map(int received_matrix[MATRIX_DIM][MATRIX_DIM]);
void generate_cluster_points(int received_matrix[MATRIX_DIM][MATRIX_DIM], struct cordinate cluster_point_cordinate[NUM_OF_CLUSTERS]);

void grouping_points(int map_matrix[MATRIX_DIM][MATRIX_DIM], int grouped_matrix[MATRIX_DIM][MATRIX_DIM], struct cordinate cluster_point_cordinate[NUM_OF_CLUSTERS]);

void calc_center_of_gravity(int grouped_matrix[MATRIX_DIM][MATRIX_DIM], struct cordinate cluster_point_cordinate[NUM_OF_CLUSTERS]);

void display_matrix(int received_matrix[MATRIX_DIM][MATRIX_DIM]);
void display_cordinate(struct cordinate cluster_point_cordinate[NUM_OF_CLUSTERS]);


int main(){
    int random_matrix[MATRIX_DIM][MATRIX_DIM];
    int clusters_point_matrix[MATRIX_DIM][MATRIX_DIM];
    int grouped_matrix[MATRIX_DIM][MATRIX_DIM];

    struct cordinate cluster_point_cordinate[NUM_OF_CLUSTERS] = {0, 0};

    int before_updated_x[NUM_OF_CLUSTERS];
    int before_updated_y[NUM_OF_CLUSTERS];

    //重心計算前後で、重心の位置に変化があるかどうかを調べる
    init_vec(before_updated_x);
    init_vec(before_updated_y);
    

    //ランダムな行列を生成して、表示
    generate_random_map(random_matrix);
    printf("ランダムな行列を生成\n");
    display_matrix(random_matrix);
    printf("\n\n");

    //クラスタ点を生成し、点の行列と座標を表示
    init_map(clusters_point_matrix);
    generate_cluster_points(clusters_point_matrix, cluster_point_cordinate);
    printf("クラスタ点の行列\n");
    display_matrix(clusters_point_matrix);
    printf("クラスタの座標\n");
    display_cordinate(cluster_point_cordinate);
    printf("\n\n");

    //クラスタリングを実行！！！
    int counter = 0;
    while(counter < LIMIT){
        counter = counter + 1;
        for(int i = 0; i < NUM_OF_CLUSTERS; i++){
            before_updated_x[i] = cluster_point_cordinate[i].x;
            before_updated_y[i] = cluster_point_cordinate[i].y;
        }


        //グループを表示する行列を生成し、counter回目のクラスタリングの処理をする。
        init_map(grouped_matrix);
        grouping_points(random_matrix, grouped_matrix, cluster_point_cordinate);
        printf("%d回クラスタリングされたマップ\n", counter);
        display_matrix(grouped_matrix);

        //重心を求めて、あたらしいクラスタ点とする。
        calc_center_of_gravity(grouped_matrix, cluster_point_cordinate);
        printf("重心を計算した新しいクラスタ点\n");
        display_cordinate(cluster_point_cordinate);
        printf("\n\n");

        bool convergence = true;
        for(int i = 0; i < NUM_OF_CLUSTERS; i++){
            if(before_updated_x[i] != cluster_point_cordinate[i].x || before_updated_y[i] != cluster_point_cordinate[i].y){
                convergence = false;
            }
        }
        if(convergence == true) return 0;
        
    }

    return 0;
}


//ランダムに点を作成
void generate_random_map(int received_matrix[MATRIX_DIM][MATRIX_DIM]){
    srand(time(NULL)); // 乱数発生器を初期化するために現在の時間を使用    
    for(int i = 0; i < MATRIX_DIM; i++){
        for(int j = 0; j < MATRIX_DIM; j++){
            int randomValue = rand() % 2;
            received_matrix[i][j] = randomValue;
        }
    }
}

//真っ白な0だけの行列を生成
void init_map(int received_matrix[MATRIX_DIM][MATRIX_DIM]){
    for(int i = 0; i < MATRIX_DIM; i++){
        for(int j = 0; j < MATRIX_DIM; j++){
             received_matrix[i][j] = 0;
       }
    }
}

void init_vec(int received_vector[NUM_OF_CLUSTERS]){
    for(int i = 0; i < NUM_OF_CLUSTERS; i++){
        received_vector[i] = 0;
    }
}



//行列の中身を表示
void display_matrix(int received_matrix[MATRIX_DIM][MATRIX_DIM]){
    for(int i = 0; i < MATRIX_DIM; i++){
        for(int j = 0; j < MATRIX_DIM; j++){
            //printf(" [%d][%d] = %d ", i, j, received_matrix[i][j]);
            printf(" %d ", received_matrix[i][j]);
        }
        printf("\n");
    }
}

//クラスタの点の座標表示
void display_cordinate(struct cordinate cluster_point_cordinate[NUM_OF_CLUSTERS]){
    for(int count = 0; count < NUM_OF_CLUSTERS; count++){
        printf("cluster_point[%d] = [%d, %d] \n", count + 1, cluster_point_cordinate[count].x + 1, cluster_point_cordinate[count].y + 1);
    }
}


//行列に任意のポイントだけランダムに点を生成
void generate_cluster_points(int received_matrix[MATRIX_DIM][MATRIX_DIM], struct cordinate cluster_point_cordinate[NUM_OF_CLUSTERS]){
    srand(time(NULL));

    for(int count = 0; count < NUM_OF_CLUSTERS; count++){
        
        int i = rand() % MATRIX_DIM;
        int j = rand() % MATRIX_DIM; 

        received_matrix[i][j] = 1;
        cluster_point_cordinate[count].x = j;
        cluster_point_cordinate[count].y = i; 
    }
}

//距離を計算し、グループ分け
void grouping_points(int map_matrix[MATRIX_DIM][MATRIX_DIM], int grouped_matrix[MATRIX_DIM][MATRIX_DIM], struct cordinate cluster_point_cordinate[NUM_OF_CLUSTERS]){    
    for(int i = 0; i < MATRIX_DIM; i++){
        for(int j = 0; j < MATRIX_DIM; j++){
            if(map_matrix[i][j] == 1){
                float distance[NUM_OF_CLUSTERS];
                for(int point_num = 0; point_num < NUM_OF_CLUSTERS; point_num++){
                    distance[point_num] = sqrt(pow(cluster_point_cordinate[point_num].x - i, 2) + pow(cluster_point_cordinate[point_num].y - j, 2));
                }

                float min_distance = distance[0];
                int min_index = 0;

                for(int count = 0; count < NUM_OF_CLUSTERS; count++){
                    if(min_distance > distance[count]){
                        min_distance = distance[count];
                        min_index = count;
                    }    
                }
                grouped_matrix[i][j] = min_index + 1; 

            }
        }
    }
}


void calc_center_of_gravity(int grouped_matrix[MATRIX_DIM][MATRIX_DIM], struct cordinate cluster_point_cordinate[NUM_OF_CLUSTERS]){
    for(int point_num = 0; point_num < MATRIX_DIM; point_num++){

        float float_x_g = 0;
        float float_y_g = 0;
        int counter = 0;        
        
        for(int i = 0; i < MATRIX_DIM; i++){
            for(int j = 0; j < MATRIX_DIM; j++){
                
                if(grouped_matrix[i][j] == point_num + 1){ //1, 2, 3...成分を発見した場合。
                    float_x_g = float_x_g + i;
                    float_y_g = float_y_g + j;
                    counter = counter + 1;
                }
            }
        }

        if(counter == 0) continue; //重心の分母が0だとエラーが起こるのでスキップする        

        float_x_g = float_x_g / counter;
        float_y_g = float_y_g / counter;

        cluster_point_cordinate[point_num].x = (int)round(float_x_g);
        cluster_point_cordinate[point_num].y = (int)round(float_y_g);
    }
}