#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define max_len 100
#define max_num 1000

void word_check(char *word) // Chuyển đổi chữ hoa thành chữ thường và xoá kí tự lạ
{
    int i, j;
    for (i = 0; i < strlen(word); i++)
    {
        if (!((word[i] >= 'A' && word[i] <= 'Z') || (word[i] >= 'a' && word[i] <= 'z')))
        {
            for (j = i; j < strlen(word); j++)
            {
                word[j] = word[j + 1];
            }
            i--;
            continue;
        }
        if (word[i] >= 'A' && word[i] <= 'Z')
        {
            word[i] -= ('A' - 'a');
        }
    }
}
int isName(char *word, char *word_prev)
{
    char c = word[0];
    if (c >= 'A' && c <= 'Z')
    {
        if (word_prev[strlen(word_prev) - 1] == '.')
        {
            return 1;
        }
    }
    return 0;
}

void updaterow(FILE *ptr, int *row)
{
    char c;
    fscanf(ptr, "%c", &c); // Check kí tự tiếp theo có phải là '\n' không
    if (c == '\n')
    { // Neu c == '\n' thì tăng biến đếm dòng lên 1
        (*row)++;
    }
}

int main()
{
    FILE *ptr, *ptr_stop;
    char word_prev[max_len];
    char word[max_len];
    char words[max_num][max_len];
    char words_stop[max_num][max_len];
    char words_prev[max_num][max_len];
    char c;
    int words_count[max_num] = {0};
    int words_index[max_num][max_len] = {0};
    int i, j, index;
    int count = 0, count_stop = 0, row = 1;
    ptr = fopen("vanban.txt", "r");
    if (ptr == NULL)
    {
        printf("Can not open file vanban.txt!\n");
        return 1;
    }
    ptr_stop = fopen("stopw.txt", "r");
    if (ptr_stop == NULL)
    {
        printf("Can not open file stopw.txt!\n");
        return 1;
    }

    // Đọc file stopw.txt và lưu danh sách từ vào mảng words_stop
    while (fscanf(ptr_stop, "%s", word) == 1)
    {
        strcpy(words_stop[count_stop++], word);
    }
    //

    // Thiết lập word_prev
    fscanf(ptr, "%s", word_prev);
    fseek(ptr, 0, SEEK_SET);
    //

    while (fscanf(ptr, "%s", word) == 1)
    {
        // Check tên riêng
        if (isName(word, word_prev) == 1)
        {
            updaterow(ptr, &row);
            strcpy(word_prev, word);
            continue;
        }
        //

        strcpy(word_prev, word);
        // Chuẩn hoá word
        word_check(word);
        //
        if(strlen(word) ==0) {          //Nếu word là 1 số hoặc 1 dãy full kí tự lạ thì strlen = 0
            updaterow(ptr, &row);
            continue;
        }
        // Check words_stop
        j = 0;
        for (i = 0; i < count_stop; i++)
        {
            if (strcmp(words_stop[i], word) == 0)
            {
                j = 1;
                break;
            }
        }
        if (j == 1)
        {
            updaterow(ptr, &row);
            continue;
        }

        i = -1;
        // Check trong words da co word hay chua
        for (j = 0; j < max_num; j++)
        {
            if (strcmp(words[j], word) == 0)
            {
                i = j;
                break;
            }
        }
        if (i == -1) // Neu chua co thi i = -1
        {
            words_count[count]++;
            strcpy(words[count], word);
            words_index[count][0] = row;
            count++;
        }
        else
        {
            words_count[i]++; // Neu da co thi words_count++
            for (j = 0; j < max_len; j++)
            {
                if (words_index[i][j] == 0) // Mảng word_index ban đầu toàn là số 0. => số khác 0 đầu tiên chính là index đã được ghi trước đó
                {
                    if (words_index[i][j - 1] == row) // Nếu 1 word xuất hiện nhiều hơn 1 lần trên cùng 1 dòng thì
                    {                                 // không lưu index của những lần xuất hiện sau.
                        break;
                    }
                    words_index[i][j] = row;
                    break;
                }
            }
        }
        updaterow(ptr, &row);
    }

    // Sắp xếp lại mảng theo thứ tự từ điển bằng bubble sort
    //Mảng words_prev sẽ lưu mảng words khi chưa sắp xếp
    for (i = 0; i < count; i++)
    {
        strcpy(words_prev[i], words[i]);
    }
    
    //Sắp xếp words
    for (i = 0; i < count; i++)
    {
        for (j = 0; j < count - 1 - i; j++)
        {
            if (strcmp(words[j], words[j + 1]) > 0)
            {
                // swap words[j] and words[j+1]
                strcpy(word_prev, words[j]);
                strcpy(words[j], words[j + 1]);
                strcpy(words[j + 1], word_prev);
            }
        }
    }
    //In words và index
    for (i = 0; i < count; i++)
    {
        printf("%s ", words[i]);
        for(index = 0; index < count; index++){         //Tìm index của words[i] trong mảng words_index thông qua words_prev
            if(strcmp(words[i], words_prev[index])==0){
                break;
            }
        }
        printf("%d", words_count[index]);
        j = 0;
        while (words_index[index][j] != 0)
        {
            printf(",%d", words_index[index][j++]);
        }
        printf("\n");
    }
    fclose(ptr);
    fclose(ptr_stop);
    return 0;
}