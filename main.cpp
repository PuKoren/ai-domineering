#include <iostream>
#include <vector>
#define MINMAX_RECURSIVITY 3

std::vector<std::vector<char> > board;

int row_count = 8;
int col_count = 8;
enum direction{
	VERTICAL = 'V',
	HORIZONTAL = 'H'
};

void draw_game(){
	std::cout << " _";
	for(int i = 0; i < col_count; i++){
		std::cout << "_" << i+1;
	}
	std::cout << std::endl;
	for(int i = 0; i < row_count; i++){
		std::cout << i+1 << "|";
		for(int j = 0; j < col_count; j++){
			std::cout << " " << board[i][j];
		}
		std::cout << std::endl;
	}
}

void input(int &command, const char* command_name){
	std::cout << "Press " << command_name << ". Use 0 to exit." << std::endl;
	std::cin >> command;
}

bool place_item(int row, int col, direction dir, bool silent){
	int col_m = 0;
	int row_m = 0;
	if(dir == VERTICAL){
		row_m = 1;
	}else{
		col_m = 1;
	}

	if(row+row_m >= row_count || col+col_m >= col_count || board[row+row_m][col] != '0' || board[row][col+col_m] != '0'){
		if(!silent)
			std::cout << "Please check your input and try again." << std::endl;
		return false;
	}else{
		board[row][col] = dir;
		board[row+row_m][col+col_m] = dir;
	}

	return true;
}

void remove_item(int row, int col, direction dir){
	if(dir == VERTICAL){
		board[row][col] = '0';
		board[row+1][col] = '0';
	}else{
		board[row][col] = '0';
		board[row][col+1] = '0';
	}
}

int get_possibilities(direction dir){
	int sum = 0;
	int row_m = 0;
	int col_m = 0;
	if(dir == VERTICAL){
		row_m = 1;
	}else{
		col_m = 1;
	}

	for(int i = 0; i < row_count-row_m; i++){
		for(int j = 0; j < col_count-col_m; j++){
			if(board[i][j] == '0' && board[i+row_m][j+col_m] == '0'){
				sum += 1;
			}
		}
	}

	return sum;
}
int max(int recursivity, direction dir, int &ri, int &rj);


int alphabeta(int recursivity, direction dir, int &ri, int &rj, int alpha, int beta){
	if(recursivity == 0)
		return get_possibilities(dir);

	int fi = 0;
	int fj = 0;
	for(int i = 0; i < row_count; i ++){
		for(int j = 0; j < col_count; j ++){
			if(place_item(i, j, dir, true)){
				int l = -alphabeta(recursivity-1, ((dir==HORIZONTAL)?VERTICAL:HORIZONTAL), fi, fj, -beta, -alpha);
				remove_item(i, j, dir);
				if(l > alpha){
					alpha = l;
					ri = i;
					rj = j;
					if(alpha >= beta){
						return beta;
					}
				}
			}
		}
	}
	return alpha;
}

int megamax(int recursivity, direction dir, int &ri, int &rj){
	if(recursivity == 0)
		return get_possibilities(dir);

	int eval = -row_count*col_count;
	for(int i = 0; i < row_count; i ++){
		for(int j = 0; j < col_count; j ++){
			if(place_item(i, j, dir, true)){
				int l = -megamax(recursivity-1, ((dir==HORIZONTAL)?VERTICAL:HORIZONTAL), ri, rj);
				remove_item(i, j, dir);
				if(l > eval){
					eval = l;
					ri = i;
					rj = j;
				}
			}
		}
	}
	return eval;
}

int min(int recursivity, direction dir){
	if(recursivity == 0)
		return get_possibilities(HORIZONTAL);

	int eval = row_count*col_count;
	int ri;
	int rj;
	for(int i = 0; i < row_count; i ++){
		for(int j = 0; j < col_count; j ++){
			if(place_item(i, j, dir, true)){
				int l = max(recursivity-1, ((dir==HORIZONTAL)?VERTICAL:HORIZONTAL), ri, rj);
				remove_item(i, j, dir);
				if(l < eval)
					eval = l;
			}
		}
	}
	return eval;
}

int max(int recursivity, direction dir, int &ri, int &rj){
	if(recursivity == 0)
		return get_possibilities(HORIZONTAL);

	int eval = -col_count * row_count;
	for(int i = 0; i < row_count; i ++){
		for(int j = 0; j < col_count; j ++){
			if(place_item(i, j, dir, true)){
				int l = min(recursivity-1, ((dir==HORIZONTAL)?VERTICAL:HORIZONTAL));
				remove_item(i, j, dir);
				if(l > eval){
					eval = l;
					ri = i;
					rj = j;
				}
			}
		}
	}
	return eval;
}

void minmax(int recursivity, direction player){
	std::cout << "Computer turn using MinMax..." << std::endl;
	int i = 0;
	int j = 0;
	max(recursivity, player, i, j);
	std::cout << "Placed item on column " << j+1 << ", row " << i+1 << std::endl;
	place_item(i, j, player, true);
}

void computer(){
	std::cout << "Computer turn..." << std::endl;

	int maxI, maxJ = 0;
	int max = -1;

	for(int i = 0; i < row_count; i ++){
		for(int j = 0; j < col_count; j ++){
			if(place_item(i, j, HORIZONTAL, true)){
				int sP = get_possibilities(HORIZONTAL) - get_possibilities(VERTICAL);
				remove_item(i, j, HORIZONTAL);
				if(sP > max){
					max = sP;
					maxI = i;
					maxJ = j;
				}
			}
		}
	}

	std::cout << "Placed item on column " << maxJ+1 << ", row " << maxI+1 << std::endl;
	place_item(maxI, maxJ, HORIZONTAL, true);
}

int main(int argc, char** argv){
	std::cout << "Domineering !" << std::endl;
	board.resize(row_count);
	for(int i = 0; i < row_count; i++){
		board[i].assign(col_count, '0');
	}
	
	int command = -1;
	bool validated = false;
	while(command != 0 || !validated){
		draw_game();
		std::cout << "Player turn..." << std::endl;
		if(get_possibilities(VERTICAL) == 0){
			std::cout << "You lost." << std::endl;
			break;
		}
		input(command, "column to place your first box");
		if(command > 0 && command <= row_count){
			int col = command;
			input(command, "row to place your first box");
			if(command > 0 && command <= col_count){
				validated = place_item(command-1, col-1, VERTICAL, false);
				if(validated){
					if(get_possibilities(HORIZONTAL) == 0){
						std::cout << "You win." << std::endl;
						break;
					}
					//REGULAR
					//computer();
					//MINIMAX
					//minmax(MINMAX_RECURSIVITY, HORIZONTAL);
					//MEGAMAX & ALPHABETA
					int i = 0;
					int j = 0;
					std::cout << "Computer turn using alphabeta..." << std::endl;
					//megaxmax(MINMAX_RECURSIVITY, HORIZONTAL, i, j);
					alphabeta(MINMAX_RECURSIVITY, HORIZONTAL, i, j, -row_count*col_count, row_count*col_count);
					std::cout << "Placed item on column " << j+1 << ", row " << i+1 << std::endl;
					place_item(i, j, HORIZONTAL, true);
				}
			}
		}
	}

	return 0;
}