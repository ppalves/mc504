#include <linux/syscalls.h>
#include <linux/linkage.h>
#include <linux/unistd.h>


#define MAX_USER_SIZE 1024

static int user_weights[MAX_USER_SIZE] = { 0 };

SYSCALL_DEFINE2(setuserweight, int, uid, int, weight){

    if (uid >= MAX_USER_SIZE){
        return -1;
    }

    user_weights[uid] = weight;
    
    return weight;
}

SYSCALL_DEFINE1(getuserweight, int, uid){

    if (uid >= MAX_USER_SIZE){
        return -1;
    }

    return user_weights[uid];
}
