# Scrapbook

## Game object test
```c
typedef struct
{
    void (*update)(void*);
    void (*draw)(void*);
    void* data;

} game_object;

game_object make_object(void* data, void (*update)(void*), void (*draw)(void*))
{
    game_object obj;
    obj.data = data;
    obj.update = update;
    obj.draw = draw;
    return obj;
}

typedef struct
{
    int number;
    const char * name;
} test_obj_data;

void test_update(test_obj_data* Data)
{
    Data->number ++;
}

void test_draw(test_obj_data* Data)
{
    printf("I'm %s and my number is %d\n", Data->name, Data->number);
}

game_object test_new(test_obj_data* data, const char * name)
{
    data->name = name;
    data->number = 0;
    return make_object(data, &test_update, &test_draw);
}
```