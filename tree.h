
struct Node
{
    char word[46];
    struct Node* left;
    struct Node* right;
    int counter;
};

struct Node* new_node(char* word_name);

struct Node* insert_word(struct Node* tree, char* word_name);

void free_tree(struct Node* tree);

void convert_buffer_to_lower(char* buffer);

void write_tree_to_file(struct Node* tree, int fout, int longest_word, int most_digits);

void add_words_in_buffer_to_tree(struct Node* trees[26], char buffer[1024]);

void add_word_to_tree(struct Node* trees[26], char word[45]);

void longest_word(struct Node* tree, int* length_word);

int get_word_length(struct Node* tree);

void most_digits(struct Node* tree, int* digits);
