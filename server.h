// server.h
/**
 * Header file defining the server's data structures, constants, and function prototypes
 * necessary for managing a multiplayer game server.
 */

#ifndef SERVER_H
#define SERVER_H

// Maximum number of concurrent clients the server can handle
#define MAX_CLIENTS 10

// Maximum length for a client's name
#define MAX_NAME_LEN 30

// General purpose buffer size for messages
#define BUFFER_SIZE 256

// Timeout duration for client responses in seconds
#define TIMEOUT 40

#include <pthread.h>

// Structure representing a player in the server
typedef struct {
    int order;                          // Order in which the player connected
    int socket_id;                      // Socket file descriptor for communication
    char name[MAX_NAME_LEN];            // Player's name
    char choice[32];                    // Player's game choice ('rock', 'paper', or 'scissors')
    char client_response[32];           // Client's response for post-game actions
    int in_game;                        // Player's game status (0 = not in game, 1 = in game, 2 = disconnected)
    pthread_t thread;                   // Thread handling player's communication
    int is_winner;                      // Game outcome for the player (0 = draw, 1 = win, 2 = lost)
    char opponent_name[MAX_NAME_LEN];   // Name of the player's current opponent
    char opponent_choice[32];           // Opponent's game choice
} player;

// Structure for passing multiple arguments to game session thread
struct arg_struct {
    int client_index;   // Index of the client in the global clients array
    int opponent_index; // Index of the opponent in the global clients array
} *args;

// Function prototypes
void error(const char* msg);
int add_client(int socket_id);
int find_opponent(int client_index);
int determine_winner(player* player1, player* player2);
void* game_session(void* arg);
int wait_for_disconected_client(int client_index, int timeout);
int ping_client(int client_socket);
int find_opponent_index_by_name(const char* opponent_name);
void print_player(int client_index);
void list_players();
char* get_client_data(int client_index, char* buffer);
void end_game(int client_index, int socked_already_closed);
void return_client_to_loby(int client_index);
void make_result(int client_index, int opponent_index);
int fill_value_if_valid_data(int client_index, char* received_data);
int do_stuff_if_needed(int client_index);
void send_result(int client_index);
void* listen_for_whatever(void* arg);
void* listen_for_clients(void* arg);
int is_number(char* str);

// Global array of client structures
player clients[MAX_CLIENTS];

// Current number of connected clients
int num_clients = 0;

// Mutex for synchronizing access to the clients array
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// Mutex and condition variable for adding players
pthread_mutex_t add_player_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t player_added;

#endif // SERVER_H
